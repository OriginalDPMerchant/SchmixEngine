#pragma once
#include <SchmixEngine/Core/Application/SubSystemManager.h>
#include <chrono>

namespace SchmixEngine
{
	class Application
	{
	public:
		Application();

		Application(const Application&) = delete;
		Application& operator=(const Application) = delete;

		virtual ~Application();

		void Start();

		void Stop();

		bool IsInitialized();

		SubSystemManager& GetSubSystemManager();

		uint16_t GetUpdateFrequency();
		void SetUpdateFrequency(uint16_t Frequency);

		static Application* GetCurrentApplication();
		
	private:
		void Init();
		void Shutdown();

	protected:
		typedef std::chrono::high_resolution_clock::time_point TimePoint;
		typedef std::chrono::duration<float, std::milli> Milliseconds;

		virtual bool InitApplication() = 0;
		virtual bool ShutdownApplication() = 0;

		bool m_bIsInitialized;

		std::thread m_Thread;

		TimePoint m_LastTime;

		SubSystemManager m_SubSystemManager;

		std::atomic<bool> m_bIsRunning;
		std::atomic<uint16_t> m_UpdateFrequency;

	private:
		static Application* s_CurrentApplication;
	};
}