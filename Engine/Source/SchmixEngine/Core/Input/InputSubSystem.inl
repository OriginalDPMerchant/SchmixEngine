#include <SchmixEngine/Core/Input/InputSubSystem.h>

namespace SchmixEngine
{
	template<typename MappingResolver>
	inline InputSubSystem<MappingResolver>::InputSubSystem(const std::string& SubSystemName)
		: BaseInputSubSystem(SubSystemName),
		m_MaxPlayers(1)
	{
		SetMaxPlayers(m_MaxPlayers);
	}

	template<typename MappingResolver>
	inline bool InputSubSystem<MappingResolver>::InitSubSystem()
	{
		return m_InputMappingResolver.Init();
	}

	template<typename MappingResolver>
	inline void InputSubSystem<MappingResolver>::UpdateSubSystem(float DeltaTime)
	{
		if(m_InputMappingResolver.GetMaxGamepadCount() != m_MaxPlayers)
			m_InputMappingResolver.SetMaxGamepadCount(m_MaxPlayers);

		m_InputMappingResolver.UpdateInputDevices();

		for (uint32_t i = 0; i < m_MaxPlayers; i++)
		{
			InputMapping* pMapping = GetInputMappingOverride(i);

			if (!pMapping)
				pMapping = &m_DefaultInputMapping;

			for (const std::pair<std::string, AxisMapping>& CurrentMapping : pMapping->GetAllAxisMappings())
			{
				const float AxisValue = m_InputMappingResolver.ResolveAxisMapping(CurrentMapping.second, i);
				m_InputMappingStates[i].SetAxisValue(CurrentMapping.first, AxisValue);
			}

			for (const std::pair<std::string, ActionMapping>& CurrentMapping : pMapping->GetAllActionMappings())
			{
				const bool IsPressed = m_InputMappingResolver.ResolveActionMapping(CurrentMapping.second, i);
				const bool WasPressed = m_InputMappingStates[i].GetActionValue(CurrentMapping.first) != ActionMappingState::RELEASED;

				const ActionMappingState ActionValue = IsPressed ? (WasPressed ? ActionMappingState::HELD : ActionMappingState::PRESSED) : ActionMappingState::RELEASED;

				m_InputMappingStates[i].SetActionValue(CurrentMapping.first, ActionValue);
			}
		}
	}

	template<typename MappingResolver>
	inline bool InputSubSystem<MappingResolver>::ShutdownSubSystem()
	{
		return m_InputMappingResolver.Shutdown();
	}

	template<typename MappingResolver>
	inline void InputSubSystem<MappingResolver>::SetMaxPlayers(uint32_t MaxPlayers)
	{
		m_MaxPlayers = MaxPlayers;
		
		m_InputMappingOverrides.resize(m_MaxPlayers);
		m_InputMappingStates.resize(m_MaxPlayers);
	}

	template<typename MappingResolver>
	inline uint32_t InputSubSystem<MappingResolver>::GetMaxPlayers()
	{
		return m_MaxPlayers;
	}

	template<typename MappingResolver>
	inline void InputSubSystem<MappingResolver>::SetDefaultInputMapping(const InputMapping& Mapping)
	{
		m_DefaultInputMapping = Mapping;

		for (uint32_t i = 0; i < m_MaxPlayers; i++)
		{
			if (!m_InputMappingOverrides[i])
				m_InputMappingStates[i].SetInputMapping(Mapping);
		}
	}

	template<typename MappingResolver>
	inline InputMapping* InputSubSystem<MappingResolver>::GetDefaultInputMapping()
	{
		return &m_DefaultInputMapping;
	}

	template<typename MappingResolver>
	inline void InputSubSystem<MappingResolver>::SetInputMappingOverride(uint32_t PlayerIndex, const InputMapping& Mapping)
	{
		if (PlayerIndex < m_MaxPlayers)
		{
			m_InputMappingOverrides[PlayerIndex].reset(new InputMapping(Mapping));
			m_InputMappingStates[PlayerIndex].SetInputMapping(Mapping);
		}
	}

	template<typename MappingResolver>
	inline InputMapping* InputSubSystem<MappingResolver>::GetInputMappingOverride(uint32_t PlayerIndex)
	{
		return PlayerIndex < m_MaxPlayers ? m_InputMappingOverrides[PlayerIndex].get() : nullptr;
	}

	template<typename MappingResolver>
	inline void InputSubSystem<MappingResolver>::ClearInputMappingOverride(uint32_t PlayerIndex)
	{
		if (PlayerIndex < m_MaxPlayers)
		{
			m_InputMappingOverrides[PlayerIndex].reset();
			m_InputMappingStates[PlayerIndex].SetInputMapping(m_DefaultInputMapping);
		}
	}

	template<typename MappingResolver>
	inline float InputSubSystem<MappingResolver>::GetAxisMappingValue(uint32_t PlayerIndex, const std::string& AxisMappingName)
	{
		return PlayerIndex < m_MaxPlayers ? m_InputMappingStates[PlayerIndex].GetAxisValue(AxisMappingName) : 0.0f;
	}

	template<typename MappingResolver>
	inline ActionMappingState InputSubSystem<MappingResolver>::GetActionMappingValue(uint32_t PlayerIndex, const std::string& ActionMappingName)
	{
		return PlayerIndex < m_MaxPlayers ? m_InputMappingStates[PlayerIndex].GetActionValue(ActionMappingName) : ActionMappingState::RELEASED;
	}
}