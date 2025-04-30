#pragma once
#include <math.h>
#include <SchmixEngine/SDL2/Input/SDLGamepadWrapper.h>
#include <SchmixEngine/Core/Input/InputSubSystem.h>
#include <SchmixEngine/Core/Debugging/Debug.h>
#include <SchmixEngine/ThirdParty/SDL2/include/SDL.h>

namespace SchmixEngine
{
	class SDL2InputMappingResolver : public InputMappingResolver
	{
	public:
		SDL2InputMappingResolver() = default;
		virtual ~SDL2InputMappingResolver() = default;

		//Overriden Functions
		bool Init();
		bool Shutdown();

		void UpdateInputDevices();
		float ResolveAxisMapping(const AxisMapping& Mappings, uint32_t PlayerIndex);
		bool ResolveActionMapping(const ActionMapping& Mappings, uint32_t PlayerIndex);
		//End of overriden funtions

	private:
		//Returns the value of an axis for a specific player 
		float GetAxisValue(Axis TargetAxis, uint32_t PlayerIndex);

		//Returns if a button is pressed for a specific player
		bool GetButtonValue(Buttons TargetButton, uint32_t PlayerIndex);

		//Returns the current value of a Joystick Axis for an SDL_Joystick
		float GetJoystickAxis(SDL_Joystick* Joystick, int JoystickAxis);

		//Returns whether a button is pressed for an SDL_Joystick
		bool GetJoystickButton(SDL_Joystick* Joystick, int JoystickButton);

		void ResizeJoystickArray();

		//Converts a Button (Enum) to an SDL_Scancode for reading Keyboard Inputs
		SDL_Scancode KeyboardKeyToSDLScancode(Buttons Button);

		std::vector<SDL_GamepadWrapper> m_Gamepads;
	};

	typedef InputSubSystem<SDL2InputMappingResolver> SDL2InputSubSystem;
}