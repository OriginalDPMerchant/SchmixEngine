#include <SchmixEngine/Core/Application/Application.h>

//TDOD: REMOVE SUBSYSTEMTHREAD CLASS
namespace SchmixEngine
{
	Application* Application::s_CurrentApplication = nullptr;

	Application::Application()
		: m_bIsInitialized(false),
		m_bIsRunning(false), 
		m_UpdateFrequency(60)
	{
	}

	Application::~Application()
	{
	}

	void Application::Start()
	{
		if (!m_bIsRunning.load())
		{
			Init();

			if (m_bIsInitialized)
			{
				m_bIsRunning.store(true);

				m_LastTime = std::chrono::high_resolution_clock::now();

				while (m_bIsRunning)
				{
					const uint16_t UpdateFrequency = m_UpdateFrequency.load();
					const float TargetFrameTime = UpdateFrequency != 0 ? 1000.0f / static_cast<float>(UpdateFrequency) : 0.0f;

					const auto DeltaTime = std::chrono::duration_cast<Milliseconds>(std::chrono::high_resolution_clock::now() - m_LastTime);

					if (DeltaTime.count() >= TargetFrameTime)
					{
						m_LastTime = std::chrono::high_resolution_clock::now();

						m_SubSystemManager.UpdateAllSubSystems(DeltaTime.count() / 1000.0f);
					}
				}
			}

			Shutdown();
		}
		else
		{
			//todo: warning
		}
	}

	void Application::Stop()
	{
		m_bIsRunning.store(false);
	}

	void Application::Init()
	{
		s_CurrentApplication = this;

		if (!m_bIsInitialized)
			m_bIsInitialized = InitApplication();

		m_SubSystemManager.InitAllSubSystems();
	}

	void Application::Shutdown()
	{
		m_SubSystemManager.DestroyAllSubSystems();

		if (m_bIsInitialized)
			m_bIsInitialized = !ShutdownApplication();

		s_CurrentApplication = nullptr;
	}

	bool Application::IsInitialized()
	{
		return m_bIsInitialized;
	}

	SubSystemManager& Application::GetSubSystemManager()
	{
		return m_SubSystemManager;
	}

	uint16_t Application::GetUpdateFrequency()
	{
		return m_UpdateFrequency.load();
	}

	void Application::SetUpdateFrequency(uint16_t Frequency)
	{
		m_UpdateFrequency.store(Frequency);
	}

	Application* Application::GetCurrentApplication()
	{
		return s_CurrentApplication;
	}
}
