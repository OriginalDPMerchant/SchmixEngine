#include "InputMappingResolver.h"

namespace SchmixEngine
{
	InputMappingResolver::InputMappingResolver()
		: m_MaxGamepadCount(1),
		m_ConnectedGamePadCount(0),
		m_bSkipAssigningPlayerOneGamepad(false)
	{
	}

	void InputMappingResolver::SetMaxGamepadCount(uint32_t MaxGamepadCount)
	{
		m_MaxGamepadCount = MaxGamepadCount;
	}

	uint32_t InputMappingResolver::GetMaxGamepadCount()
	{
		return m_MaxGamepadCount;
	}

	uint32_t InputMappingResolver::GetConnectedGamePadCount()
	{
		return m_ConnectedGamePadCount;
	}

	void InputMappingResolver::SetSkipAssigningPlayerOneGamepad(bool SkipAssigningPlayerOneGamepad)
	{
		m_bSkipAssigningPlayerOneGamepad = SkipAssigningPlayerOneGamepad;
	}

	bool InputMappingResolver::GetSkipAssigningPlayerOneGamepad()
	{
		return m_bSkipAssigningPlayerOneGamepad;
	}

	bool InputMappingResolver::Init()
	{
		SMX_DBG_LOG("The Function \"Init()\" has not been implemented in the class inheriting from \"SchmixEngine::InputMappingResolver\"", LogLevel::Warning);
		return false;
	}

	bool InputMappingResolver::Shutdown()
	{
		SMX_DBG_LOG("The Function \"Shutdown()\" has not been implemented in the class inheriting from \"SchmixEngine::InputMappingResolver\"", LogLevel::Warning);
		return false;
	}

	void InputMappingResolver::UpdateInputDevices()
	{
		SMX_DBG_LOG("The Function \"UpdateInputDevices()\" has not been implemented in the class inheriting from \"SchmixEngine::InputMappingResolver\"", LogLevel::Warning);
	}

	float InputMappingResolver::ResolveAxisMapping(const AxisMapping& Mappings, uint32_t PlayerIndex)
	{
		SMX_DBG_LOG("The Function \"ResolveAxisMapping()\" has not been implemented in the class inheriting from \"SchmixEngine::InputMappingResolver\"", LogLevel::Warning);
		return 0.0f;
	}

	bool InputMappingResolver::ResolveActionMapping(const ActionMapping& Mappings, uint32_t PlayerIndex)
	{
		SMX_DBG_LOG("The Function \"ResolveActionMapping()\" has not been implemented in the class inheriting from \"SchmixEngine::InputMappingResolver\"", LogLevel::Warning);
		return false;
	}
}