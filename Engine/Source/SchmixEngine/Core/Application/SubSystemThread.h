#pragma once
#include <SchmixEngine/Core/Application/SubSystemManager.h>
#include <thread>

namespace SchmixEngine
{
	class SubSystemThread final
	{
	public:
		SubSystemThread(bool IsMainThread = false);

		SubSystemThread(const SubSystemThread&) = delete;
		SubSystemThread& operator=(const SubSystemThread&) = delete;

		~SubSystemThread();

		void Start();
		void Stop();

		SubSystemManager* GetSubSystemManager();

		uint16_t GetUpdateFrequency();
		void SetUpdateFrequency(uint16_t Frequency);

	private:
		typedef std::chrono::high_resolution_clock::time_point TimePoint;
		typedef std::chrono::duration<float, std::milli> Milliseconds;

		void Run();

		std::thread m_Thread;

		TimePoint m_LastTime;

		SubSystemManager m_SubSystemManager;
		
		bool m_bIsMainThread;

		std::atomic<bool> m_bIsRunning;
		std::atomic<uint16_t> m_ThreadUpdateFrequency;
	};
}