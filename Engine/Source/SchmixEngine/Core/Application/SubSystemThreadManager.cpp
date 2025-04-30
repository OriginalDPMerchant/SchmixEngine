#include <SchmixEngine/Core/Application/SubSystemThreadManager.h>

namespace SchmixEngine
{
	SubSystemThreadManager::SubSystemThreadManager()
	{
	}

	SubSystemThreadManager::~SubSystemThreadManager()
	{
	}

	void SubSystemThreadManager::Init()
	{
		if (!m_pMainThread)
			m_pMainThread = std::make_unique<SubSystemThread>(true);
	}

	void SubSystemThreadManager::StartAllThreads()
	{
		for(auto& Thread : m_SubSystemThreads)
		{
			if (Thread.second)
				Thread.second->Start();
		}

		if (m_pMainThread)
			m_pMainThread->Start();
	}

	SubSystemThread* SubSystemThreadManager::CreateSubSystemThread(const std::string& Name)
	{
		SubSystemThread* ReturnValue = GetSubSystemThread(Name);

		if (!ReturnValue)
		{
			m_SubSystemThreads[Name] = std::make_unique<SubSystemThread>(false);
			ReturnValue = m_SubSystemThreads[Name].get();
		}

		return ReturnValue;
	}

	void SubSystemThreadManager::DestroySubSystemThread(const std::string& Name)
	{
		if (Name == MainThreadName)
		{
			m_pMainThread.reset();
		}
		else
		{
			auto Iterator = m_SubSystemThreads.find(Name);

			if (Iterator != m_SubSystemThreads.end())
				m_SubSystemThreads.erase(Iterator);
		}
	}

	void SubSystemThreadManager::DestroyAllSubSystemThreads()
	{
		m_SubSystemThreads.clear();
		m_pMainThread.reset();
	}

	SubSystemThread* SubSystemThreadManager::GetSubSystemThread(const std::string& Name)
	{
		if (Name == MainThreadName)
		{
			return m_pMainThread.get();
		}
		else
		{
			auto Iterator = m_SubSystemThreads.find(Name);

			if (Iterator != m_SubSystemThreads.end())
				return Iterator->second.get();
		}

		return nullptr;
	}
}