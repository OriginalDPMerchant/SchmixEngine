#include <SchmixEngine/Core/Application/SubSystem.h>

namespace SchmixEngine
{
	SubSystem::SubSystem(const std::string& SubSystemName)
		: m_SubSystemName(SubSystemName), m_bIsInitialized(false)
	{
	}

	void SubSystem::Init()
	{
		if (!m_bIsInitialized)
			m_bIsInitialized = InitSubSystem();
		else
			SMX_DBG_LOG_WSRC("SubSystem has already been Initialized", m_SubSystemName, LogLevel::Warning);
	}

	void SubSystem::Update(float DeltaTime)
	{
		if (m_bIsInitialized)
			UpdateSubSystem(DeltaTime);
		else
			SMX_DBG_LOG_WSRC("Failed to Initialize SubSystem", m_SubSystemName, LogLevel::Error);
	}

	void SubSystem::Shutdown()
	{
		if (m_bIsInitialized)
			m_bIsInitialized = !ShutdownSubSystem();
		else
			SMX_DBG_LOG_WSRC("SubSystem has not been Initialized or has already been Shutdown", m_SubSystemName, LogLevel::Warning);
	}

	std::string SubSystem::GetSubSystemName() const
	{
		return m_SubSystemName;
	}

	bool SubSystem::IsInitialized() const
	{
		return m_bIsInitialized;
	}
}