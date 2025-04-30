#pragma once
#include <SchmixEngine/Core/Application/SubSystem.h>
#include <SchmixEngine/Core/Input/Mappings.h>
#include <unordered_map>

namespace SchmixEngine
{
	class BaseInputSubSystem : public SubSystem
	{
	public:
		BaseInputSubSystem(const std::string& SubSystemName);

		BaseInputSubSystem(const BaseInputSubSystem&) = delete;
		BaseInputSubSystem& operator=(const BaseInputSubSystem&) = delete;

		virtual ~BaseInputSubSystem() = default;

		virtual void SetMaxPlayers(uint32_t MaxPlayers) = 0;
		virtual uint32_t GetMaxPlayers() = 0;

		virtual void SetDefaultInputMapping(const InputMapping& Mapping) = 0;
		virtual InputMapping* GetDefaultInputMapping() = 0;

		virtual void SetInputMappingOverride(uint32_t PlayerIndex, const InputMapping& Mapping) = 0;
		virtual InputMapping* GetInputMappingOverride(uint32_t PlayerIndex) = 0;
		virtual void ClearInputMappingOverride(uint32_t PlayerIndex) = 0;

		virtual float GetAxisMappingValue(uint32_t PlayerIndex, const std::string& AxisMappingName) = 0;
		virtual ActionMappingState GetActionMappingValue(uint32_t PlayerIndex, const std::string& ActionMappingName) = 0;
	};
}