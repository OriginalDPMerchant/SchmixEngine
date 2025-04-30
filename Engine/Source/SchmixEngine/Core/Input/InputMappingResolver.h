#pragma once
#include <SchmixEngine/Core/Input/Mappings.h>
#include <SchmixEngine/Core/Debugging/Debug.h>
#include <SchmixEngine/Core/Input/Mappings.h>

namespace SchmixEngine
{
	class InputMappingResolver
	{
	public:
		InputMappingResolver();
		virtual ~InputMappingResolver() = default;

		void SetMaxGamepadCount(uint32_t MaxGamepadCount);
		uint32_t GetMaxGamepadCount();

		uint32_t GetConnectedGamePadCount();

		void SetSkipAssigningPlayerOneGamepad(bool SkipAssigningPlayerOneGamepad);
		bool GetSkipAssigningPlayerOneGamepad();

		bool Init();
		bool Shutdown();

		void UpdateInputDevices();
		float ResolveAxisMapping(const AxisMapping& Mappings, uint32_t PlayerIndex);
		bool ResolveActionMapping(const ActionMapping& Mappings, uint32_t PlayerIndex);

	protected:
		uint32_t m_MaxGamepadCount;
		uint32_t m_ConnectedGamePadCount;

		bool m_bSkipAssigningPlayerOneGamepad;
	};
}