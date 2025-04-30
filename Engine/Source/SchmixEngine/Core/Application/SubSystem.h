#pragma once
#include <SchmixEngine/Core/Debugging/Debug.h>
#include <string>
#include <atomic>

namespace SchmixEngine
{
	class SubSystem
	{
	public:
		SubSystem(const std::string& SubSystemName);

		SubSystem(const SubSystem&) = delete;
		SubSystem& operator=(const SubSystem&) = delete;

		virtual ~SubSystem() = default;

		void Init();
		void Update(float DeltaTime);
		void Shutdown();

		std::string GetSubSystemName() const;
		
		bool IsInitialized() const;

	protected:
		virtual bool InitSubSystem() = 0;
		virtual void UpdateSubSystem(float DeltaTime) = 0;
		virtual bool ShutdownSubSystem() = 0;

		std::string m_SubSystemName;

		bool m_bIsInitialized;
	};
}