#pragma once
#include <SchmixEngine/Core/Input/BaseInputSubSystem.h>
#include <SchmixEngine/Core/Input/InputMappingResolver.h>
#include <memory>

namespace SchmixEngine
{
	template<typename MappingResolver>
	class InputSubSystem : public BaseInputSubSystem
	{
	public:
		static_assert(std::is_base_of_v<InputMappingResolver, MappingResolver>, "Template Argument \"MappingResolver\" must inherit from \"SchmixEngine::InputMappingResolver\"");

		InputSubSystem(const std::string& SubSystemName);

		InputSubSystem(const InputSubSystem&) = delete;
		InputSubSystem& operator=(const InputSubSystem&) = delete;

		virtual ~InputSubSystem() = default;

		virtual bool InitSubSystem() override;
		virtual void UpdateSubSystem(float DeltaTime) override;
		virtual bool ShutdownSubSystem() override;

		virtual void SetMaxPlayers(uint32_t MaxPlayers) override;
		virtual uint32_t GetMaxPlayers() override;

		virtual void SetDefaultInputMapping(const InputMapping& Mapping) override;
		virtual InputMapping* GetDefaultInputMapping() override;

		virtual void SetInputMappingOverride(uint32_t PlayerIndex, const InputMapping& Mapping) override;
		virtual InputMapping* GetInputMappingOverride(uint32_t PlayerIndex) override;
		virtual void ClearInputMappingOverride(uint32_t PlayerIndex) override;


		virtual float GetAxisMappingValue(uint32_t PlayerIndex, const std::string& AxisMappingName) override;
		virtual ActionMappingState GetActionMappingValue(uint32_t PlayerIndex, const std::string& ActionMappingName) override;

	protected:
		MappingResolver m_InputMappingResolver;

		uint32_t m_MaxPlayers;

		InputMapping m_DefaultInputMapping;

		std::vector<std::unique_ptr<InputMapping>> m_InputMappingOverrides;

		std::vector<InputMappingState> m_InputMappingStates;
	};
}

#include <SchmixEngine/Core/Input/InputSubSystem.inl>