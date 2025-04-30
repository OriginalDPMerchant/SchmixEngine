#include <SchmixEngine/Core/Application/SubSystemThread.h>

namespace SchmixEngine
{
	SubSystemThread::SubSystemThread(bool IsMainThread)
		: m_bIsMainThread(IsMainThread), m_bIsRunning(false), m_ThreadUpdateFrequency(60)
	{
	}

	SubSystemThread::~SubSystemThread()
	{
		Stop();
	}

	void SubSystemThread::Start()
	{
		if (!m_bIsRunning.load())
		{
			if (m_bIsMainThread)
				Run();
			else
				m_Thread = std::thread(&SubSystemThread::Run, this);
		}
	}

	void SubSystemThread::Stop()
	{
		if (m_bIsRunning.load())
		{
			m_bIsRunning.store(false);

			if (!m_bIsMainThread)
				m_Thread.join();
		}
	}

	SubSystemManager* SubSystemThread::GetSubSystemManager()
	{
		return &m_SubSystemManager;
	}

	uint16_t SubSystemThread::GetUpdateFrequency()
	{
		return m_ThreadUpdateFrequency.load();
	}

	void SubSystemThread::SetUpdateFrequency(uint16_t Frequency)
	{
		m_ThreadUpdateFrequency.store(Frequency);
	}

	void SubSystemThread::Run()
	{
		m_bIsRunning.store(true);

		m_SubSystemManager.InitAllSubSystems();
	
		m_LastTime = std::chrono::high_resolution_clock::now();

		while (m_bIsRunning.load())
		{
			const uint16_t UpdateFrequency = m_ThreadUpdateFrequency.load();
			const float TargetFrameTime = UpdateFrequency != 0 ? 1000.0f / static_cast<float>(UpdateFrequency) : 0.0f;

			const auto DeltaTime = std::chrono::duration_cast<Milliseconds>(std::chrono::high_resolution_clock::now() - m_LastTime);
		
			if (DeltaTime.count() >= TargetFrameTime)
			{
				m_LastTime = std::chrono::high_resolution_clock::now();

				m_SubSystemManager.UpdateAllSubSystems(DeltaTime.count() / 1000.0f);
			}
		}

		m_SubSystemManager.ShutdownAllSubSystems();
	}
}
