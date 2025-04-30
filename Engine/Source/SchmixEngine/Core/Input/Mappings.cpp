#include "Mappings.h"

namespace SchmixEngine
{
	void InputMapping::SetAxisMapping(const std::string& MappingName, const AxisMapping& Mapping)
	{
		auto Iterator = std::find_if(m_AxisMappings.begin(), m_AxisMappings.end(),
			[MappingName](const std::pair<std::string,AxisMapping>& CurrentMapping)
			{
				return CurrentMapping.first == MappingName;
			});

		if (Iterator != m_AxisMappings.end())
			*Iterator = { MappingName, Mapping };
		else
			m_AxisMappings.push_back({ MappingName, Mapping });
	}

	void InputMapping::SetActionMapping(const std::string& MappingName, const ActionMapping& Mapping)
	{
		auto Iterator = std::find_if(m_ActionMappings.begin(), m_ActionMappings.end(),
			[MappingName](const std::pair<std::string, ActionMapping>& CurrentMapping)
			{
				return CurrentMapping.first == MappingName;
			});

		if (Iterator != m_ActionMappings.end())
			*Iterator = { MappingName, Mapping };
		else
			m_ActionMappings.push_back({ MappingName, Mapping });
	}

	AxisMapping* InputMapping::GetAxisMapping(const std::string& MappingName)
	{
		auto Iterator = std::find_if(m_AxisMappings.begin(), m_AxisMappings.end(),
			[MappingName](const std::pair<std::string, AxisMapping>& CurrentMapping)
			{
				return CurrentMapping.first == MappingName;
			});

		return Iterator != m_AxisMappings.end() ? &(Iterator->second) : nullptr;
	}

	ActionMapping* InputMapping::GetActionMapping(const std::string& MappingName)
	{
		auto Iterator = std::find_if(m_ActionMappings.begin(), m_ActionMappings.end(),
			[MappingName](const std::pair<std::string, ActionMapping>& CurrentMapping)
			{
				return CurrentMapping.first == MappingName;
			});

		return Iterator != m_ActionMappings.end() ? &(Iterator->second) : nullptr;
	}

	const std::vector<std::pair<std::string, AxisMapping>>& InputMapping::GetAllAxisMappings() const
	{
		return m_AxisMappings;
	}

	const std::vector<std::pair<std::string, ActionMapping>>& InputMapping::GetAllActionMappings() const
	{
		return m_ActionMappings;
	}

	void InputMappingState::SetInputMapping(const InputMapping& Mapping)
	{
		m_AxisMappingState.clear();
		m_ActionMappingState.clear();

		for (const std::pair<std::string, AxisMapping>& CurrentMapping : Mapping.GetAllAxisMappings())
			m_AxisMappingState.insert({ CurrentMapping.first, 0.0f });
		
		for (const std::pair<std::string, ActionMapping>& CurrentMapping : Mapping.GetAllActionMappings())
			m_ActionMappingState.insert({ CurrentMapping.first, ActionMappingState::RELEASED });
	}

	void InputMappingState::SetAxisValue(const std::string& MappingName, float Value)
	{
		auto Iterator = m_AxisMappingState.find(MappingName);

		if (Iterator != m_AxisMappingState.end())
			Iterator->second = Value;
	}

	float InputMappingState::GetAxisValue(const std::string& MappingName)
	{
		auto Iterator = m_AxisMappingState.find(MappingName);

		return Iterator != m_AxisMappingState.end() ? Iterator->second : 0.0f;
	}

	void InputMappingState::SetActionValue(const std::string& MappingName, ActionMappingState Value)
	{
		auto Iterator = m_ActionMappingState.find(MappingName);

		if (Iterator != m_ActionMappingState.end())
			Iterator->second = Value;
	}

	ActionMappingState InputMappingState::GetActionValue(const std::string& MappingName)
	{
		auto Iterator = m_ActionMappingState.find(MappingName);

		return Iterator != m_ActionMappingState.end() ? Iterator->second : ActionMappingState::RELEASED;
	}
}
