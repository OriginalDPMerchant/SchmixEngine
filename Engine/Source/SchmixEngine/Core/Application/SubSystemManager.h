#pragma once 
#include <SchmixEngine/Core/Application/SubSystem.h>
#include <vector>
#include <limits>

//TODO: NOT THREAD SAFE
namespace SchmixEngine
{
	class SubSystemManager final
	{
	public:
		SubSystemManager() = default;

		SubSystemManager(const SubSystemManager&) = delete;
		SubSystemManager& operator=(const SubSystemManager&) = delete;

		~SubSystemManager();

		void InitAllSubSystems();
		void UpdateAllSubSystems(float DeltaTime);
		void ShutdownAllSubSystems();

		template<typename T>
		T* CreateSubSystem(const std::string& SubSystemName, int32_t Position = -1);

		void DestroySubSystem(int32_t Index);
		void DestroySubSystem(const std::string& Name);

		void DestroyAllSubSystems();

		int32_t GetSubSystemIndex(const std::string& Name) const;

		SubSystem* GetSubSystem(int32_t Index) const;
		SubSystem* GetSubSystem(const std::string& Name) const;

		const std::vector<std::unique_ptr<SubSystem>>& GetAllSubSystems() const;

	private:
		std::vector<std::unique_ptr<SubSystem>> m_SubSystems;
	};

	template<typename T>
	inline T* SubSystemManager::CreateSubSystem(const std::string& SubSystemName, int32_t Position)
	{
		static_assert(std::is_base_of_v<SubSystem, T>, "Template typename T in SubSystemManager::CreateSubSystem<T> must derive from SchmixEngine::SubSystem");

		SubSystem* System = GetSubSystem(SubSystemName);

		if (!System)
		{
			std::unique_ptr<T> NewSubSystem = std::make_unique<T>(SubSystemName);
			T* ReturnValue = NewSubSystem.get();

			auto Iterator = Position >= 0 && Position < m_SubSystems.size() ? m_SubSystems.begin() + Position : m_SubSystems.end();
			m_SubSystems.insert(Iterator, std::move(NewSubSystem));

			return ReturnValue;
		}
		else
		{
			SMX_DBG_LOG("A SubSystem with the name \"" + SubSystemName + "\" already exists in the SubSystemManager", LogLevel::Warning);

			return dynamic_cast<T*>(System);
		}

		return nullptr;
	}
}