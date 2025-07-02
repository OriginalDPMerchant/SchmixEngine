#include <SchmixEngine/Core/Application/SubSystemManager.h>

namespace SchmixEngine
{
	SubSystemManager::~SubSystemManager()
	{
		DestroyAllSubSystems();
	}

	void SubSystemManager::InitAllSubSystems()
	{
		for (size_t SubSystemIndex = 0; SubSystemIndex < m_SubSystems.size(); SubSystemIndex++)
		{
			if (m_SubSystems[SubSystemIndex])
				m_SubSystems[SubSystemIndex]->Init();
		}
	}

	void SubSystemManager::UpdateAllSubSystems(float DeltaTime)
	{
		for (size_t SubSystemIndex = 0; SubSystemIndex < m_SubSystems.size(); SubSystemIndex++)
		{
			if (m_SubSystems[SubSystemIndex])
				m_SubSystems[SubSystemIndex]->Update(DeltaTime);
		}
	}

	void SubSystemManager::ShutdownAllSubSystems()
	{
		for (size_t i = 0; i < m_SubSystems.size(); i++)
		{
			size_t SubSystemIndex = m_SubSystems.size() - 1 - i;

			if (m_SubSystems[SubSystemIndex])
				m_SubSystems[SubSystemIndex]->Shutdown();
		}
	}

	void SubSystemManager::DestroySubSystem(int32_t Index)
	{
		SubSystem* SubSystemToDestroy = GetSubSystem(Index);

		if (SubSystemToDestroy)
		{
			SubSystemToDestroy->Shutdown();
			m_SubSystems.erase(m_SubSystems.begin() + Index);
		}
	}

	void SubSystemManager::DestroySubSystem(const std::string& Name)
	{
		size_t Index = GetSubSystemIndex(Name);

		DestroySubSystem(Index);
	}

	void SubSystemManager::DestroyAllSubSystems()
	{
		while (m_SubSystems.size() > 0)
			DestroySubSystem(m_SubSystems.size() - 1);
	}

	int32_t SubSystemManager::GetSubSystemIndex(const std::string& Name) const
	{
		auto Iterator = std::find_if(m_SubSystems.begin(), m_SubSystems.end(),
			[&Name](const std::unique_ptr<SubSystem>& CurrentSubSystem)
			{
				return Name == CurrentSubSystem->GetSubSystemName();
			});

		if (Iterator == m_SubSystems.end())
			SMX_DBG_LOG("The SubSystemManager has no SubSystem called \"" + Name + "\"", LogLevel::Warning);

		return Iterator != m_SubSystems.end() ? Iterator - m_SubSystems.begin() : -1;
	}

	SubSystem* SubSystemManager::GetSubSystem(int32_t Index) const
	{
		return Index >= 0 && Index < m_SubSystems.size() ? m_SubSystems[Index].get() : nullptr;
	}

	SubSystem* SubSystemManager::GetSubSystem(const std::string& Name) const
	{
		size_t Index = GetSubSystemIndex(Name);

		return GetSubSystem(Index);
	}

	const std::vector<std::unique_ptr<SubSystem>>& SubSystemManager::GetAllSubSystems() const
	{
		return m_SubSystems;
	}
}