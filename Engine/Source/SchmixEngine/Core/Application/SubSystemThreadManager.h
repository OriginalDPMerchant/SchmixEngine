#pragma once
#include <SchmixEngine/Core/Application/SubSystemThread.h>
#include <unordered_map>

namespace SchmixEngine
{
	class SubSystemThreadManager final
	{
	public:
		static constexpr const char* MainThreadName = "Main Thread";

		SubSystemThreadManager();

		SubSystemThreadManager(const SubSystemThread&) = delete;
		SubSystemThreadManager& operator=(const SubSystemThreadManager&) = delete;
		
		~SubSystemThreadManager();

		void Init();

		void StartAllThreads();

		SubSystemThread* CreateSubSystemThread(const std::string& Name);

		void DestroySubSystemThread(const std::string& Name);

		void DestroyAllSubSystemThreads();

		SubSystemThread* GetSubSystemThread(const std::string& Name);

	protected:
		std::unique_ptr<SubSystemThread> m_pMainThread;
		std::unordered_map<std::string, std::unique_ptr<SubSystemThread>> m_SubSystemThreads;
	};
}