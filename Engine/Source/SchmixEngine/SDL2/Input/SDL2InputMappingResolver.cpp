#include "SDL2InputMappingResolver.h"

namespace SchmixEngine
{
	bool SDL2InputMappingResolver::Init()
	{
		return SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) == 0;
	}

	bool SDL2InputMappingResolver::Shutdown()
	{
		SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
		return true;
	}

	void SDL2InputMappingResolver::UpdateInputDevices()
	{
		ResizeJoystickArray();

		//When we find that a Gamepad has been disconnected...
		if (SDL_NumJoysticks() < m_ConnectedGamePadCount)
		{
			//Iterate through all SDL_GamepadWrapper objects in m_Gamepads to find the disconnected Gamepads
			for (int i = 0; i < m_Gamepads.size(); i++)
			{
				if (m_Gamepads[i].m_pJoystick)
				{
					//When we find a disconnected Gamepad, we "close" it (Free the memory SDL2 has allocated for tracking this gamepad)
					if (SDL_JoystickGetAttached(m_Gamepads[i].m_pJoystick) == SDL_FALSE)
						m_Gamepads[i].Close();
				}
			}

			m_ConnectedGamePadCount = SDL_NumJoysticks();
		}
		else if (SDL_NumJoysticks() > m_ConnectedGamePadCount)
		{
			//When we find that a new Gamepad has been connected...

			//If adding this Gamepad would not cause us to exceed the Maximum Gamepad Count...
			if (SDL_NumJoysticks() <= m_MaxGamepadCount)
			{
				std::vector<int> FreeDeviceIndexes;

				//Add all indexes of all the SDL_Joysticks to the FreeDeviceIndexes list
				for (int i = 0; i < SDL_NumJoysticks(); i++)
					FreeDeviceIndexes.push_back(i);

				std::vector<int> FreeJoystickIndexes;

				for (int i = 0; i < m_Gamepads.size(); i++)
				{
					//If the SDL_Joystick stored in the SDL_GamepadWrapper is valid...
					if (m_Gamepads[i].m_pJoystick)
					{
						auto Iterator = FreeDeviceIndexes.begin();

						//Iterate through the elements in FreeDeviceIndexes and remove all the elements which store the index of
						//SDL_Joysticks which have an ID that matches the ID of m_Gamepads[i].m_pJoystick.
						while (Iterator != FreeDeviceIndexes.end())
						{
							if (SDL_JoystickInstanceID(m_Gamepads[i].m_pJoystick) == SDL_JoystickGetDeviceInstanceID((*Iterator)))
							{
								Iterator = FreeDeviceIndexes.erase(Iterator);
								break;
							}
							else
							{
								Iterator++;
							}
						}
					}
					else
					{
						//If the SDL_Joystick stored in the SDL_GamepadWrapper is valid, add this index to FreeJoystickIndexes
						FreeJoystickIndexes.push_back(i);
					}
				}

				//For every new gamepad that was connected...
				for (int i = 0; i < SDL_NumJoysticks() - m_ConnectedGamePadCount; i++)
				{
					//If there is an available SDL_Joystick and an available SDL_GamepadWrapper 
					if (i < FreeDeviceIndexes.size() && i < FreeJoystickIndexes.size())
					{
						//Assign the available SDL_Joystick to the available SDL_GamepadWrapper 
						m_Gamepads[FreeJoystickIndexes[i]].m_pJoystick = SDL_JoystickOpen(FreeDeviceIndexes[i]);

						//Assign the available SDL_GameController to the available SDL_GamepadWrapper
						m_Gamepads[FreeJoystickIndexes[i]].m_pGameController = SDL_GameControllerOpen(FreeDeviceIndexes[i]);
					}
				}

				m_ConnectedGamePadCount = SDL_NumJoysticks();
			}
		}

		//Tell SDL2 to gather input events from Input Devices
		SDL_PumpEvents();

		//Tell SDL2 to update the internal Joystick structures
		SDL_JoystickUpdate();
	}

	float SDL2InputMappingResolver::ResolveAxisMapping(const AxisMapping& Mappings, uint32_t PlayerIndex)
	{
		float AxisMappingValue = 0.0f;

		//Get the value for each Axis in Mappings.m_Axis and assign the one with the largest magnitude to AxisMappingValue
		for (int i = 0; i < Mappings.m_Axis.size(); i++)
		{
			float AxisValue = GetAxisValue(Mappings.m_Axis[i].first, PlayerIndex) * Mappings.m_Axis[i].second;
			AxisMappingValue = abs(AxisValue) > abs(AxisMappingValue) ? AxisValue : AxisMappingValue;
		}

		//For each Button in Mappings.Button...
		for (int i = 0; i < Mappings.m_Buttons.size(); i++)
		{
			//If the Button is pressed set the AxisValue to Mappings.m_Buttons[i].second
			float AxisValue = GetButtonValue(Mappings.m_Buttons[i].first, PlayerIndex) ? Mappings.m_Buttons[i].second : 0.0f;

			//If |AxisValue| > |AxisMappingValue| then AxisMappingValue = AxisValue, otherwise AxisMappingValue stays the same
			AxisMappingValue = abs(AxisValue) > abs(AxisMappingValue) ? AxisValue : AxisMappingValue;
		}

		return AxisMappingValue;
	}

	bool SDL2InputMappingResolver::ResolveActionMapping(const ActionMapping& Mappings, uint32_t PlayerIndex)
	{
		bool ActionMappingValue = false;

		//Set ActionMappingValue to true if one of the Axis in Mappings.m_Axis has a value greater than the threshold
		for (int i = 0; i < Mappings.m_Axis.size(); i++)
		{
			float AxisValue = GetAxisValue(std::get<0>(Mappings.m_Axis[i]), PlayerIndex);

			if (std::get<2>(Mappings.m_Axis[i]))
				ActionMappingValue = AxisValue > std::get<1>(Mappings.m_Axis[i]);
			else
				ActionMappingValue = AxisValue < std::get<1>(Mappings.m_Axis[i]);

			if (ActionMappingValue)
				break;
		}

		if (!ActionMappingValue)
		{
			//Set ActionMappingValue to true if one of the Buttons in Mappings.m_Buttons is being pressed
			for (int i = 0; i < Mappings.m_Buttons.size(); i++)
			{
				ActionMappingValue = GetButtonValue(Mappings.m_Buttons[i], PlayerIndex);

				if (ActionMappingValue)
					break;
			}
		}

		return ActionMappingValue;
	}

	//TODO: IMPLEMENT MOUSE AXIS
	float SDL2InputMappingResolver::GetAxisValue(Axis TargetAxis, uint32_t PlayerIndex)
	{
		float AxisValue = 0.0f;

		if (PlayerIndex < m_Gamepads.size())
		{
			const uint32_t FirstGenericJoystickAxis = static_cast<uint32_t>(Axis::JOYSTICK_GENERIC_AXIS_0);
			const uint32_t LastGenericJoystickAxis = static_cast<uint32_t>(Axis::JOYSTICK_GENERIC_AXIS_7);
			const uint32_t CurrentAxisInt = static_cast<uint32_t>(TargetAxis);

			if (CurrentAxisInt < FirstGenericJoystickAxis)
			{
				switch (TargetAxis)
				{
				case Axis::MOUSE_X:
					break;
				case Axis::MOUSE_Y:
					break;
				case Axis::GAMEPAD_LEFT_STICK_X:
					if (m_Gamepads[PlayerIndex].m_pGameController)
					{
						int Axis = SDL_GameControllerGetBindForAxis(m_Gamepads[PlayerIndex].m_pGameController, SDL_CONTROLLER_AXIS_LEFTX).value.axis;
						AxisValue = GetJoystickAxis(m_Gamepads[PlayerIndex].m_pJoystick, Axis);
					}
					break;
				case Axis::GAMEPAD_LEFT_STICK_Y:
					if (m_Gamepads[PlayerIndex].m_pGameController)
					{
						int Axis = SDL_GameControllerGetBindForAxis(m_Gamepads[PlayerIndex].m_pGameController, SDL_CONTROLLER_AXIS_LEFTY).value.axis;
						AxisValue = GetJoystickAxis(m_Gamepads[PlayerIndex].m_pJoystick, Axis);
					}
					break;
				case Axis::GAMEPAD_RIGHT_STICK_X:
					if (m_Gamepads[PlayerIndex].m_pGameController)
					{
						int Axis = SDL_GameControllerGetBindForAxis(m_Gamepads[PlayerIndex].m_pGameController, SDL_CONTROLLER_AXIS_RIGHTX).value.axis;
						AxisValue = GetJoystickAxis(m_Gamepads[PlayerIndex].m_pJoystick, Axis);
					}
					break;
				case Axis::GAMEPAD_RIGHT_STICK_Y:
					if (m_Gamepads[PlayerIndex].m_pGameController)
					{
						int Axis = SDL_GameControllerGetBindForAxis(m_Gamepads[PlayerIndex].m_pGameController, SDL_CONTROLLER_AXIS_RIGHTY).value.axis;
						AxisValue = GetJoystickAxis(m_Gamepads[PlayerIndex].m_pJoystick, Axis);
					}
					break;
				case Axis::GAMEPAD_LEFT_TRIGGER:
					if (m_Gamepads[PlayerIndex].m_pGameController)
					{
						int Axis = SDL_GameControllerGetBindForAxis(m_Gamepads[PlayerIndex].m_pGameController, SDL_CONTROLLER_AXIS_TRIGGERLEFT).value.axis;
						AxisValue = GetJoystickAxis(m_Gamepads[PlayerIndex].m_pJoystick, Axis);
					}
					break;
				case Axis::GAMEPAD_RIGHT_TRIGGER:
					if (m_Gamepads[PlayerIndex].m_pGameController)
					{
						int Axis = SDL_GameControllerGetBindForAxis(m_Gamepads[PlayerIndex].m_pGameController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT).value.axis;
						AxisValue = GetJoystickAxis(m_Gamepads[PlayerIndex].m_pJoystick, Axis);
					}
					break;
				}
			}
			else if (CurrentAxisInt <= LastGenericJoystickAxis)
			{
				AxisValue = GetJoystickAxis(m_Gamepads[PlayerIndex].m_pJoystick, CurrentAxisInt - FirstGenericJoystickAxis);
			}
		}

		return AxisValue;
	}

	bool SDL2InputMappingResolver::GetButtonValue(Buttons TargetButton, uint32_t PlayerIndex)
	{
		bool ButtonState = false;

		if (PlayerIndex < m_Gamepads.size())
		{
			const uint32_t FirstKeyboardKey = static_cast<uint32_t>(Buttons::KEYBOARD_KEY_SPACE);
			const uint32_t LastKeyboardKey = static_cast<uint32_t>(Buttons::KEYBOARD_KEY_MENU);

			const uint32_t FirstGenericJoystickButton = static_cast<uint32_t>(Buttons::JOYSTICK_GENERIC_BUTTON_0);
			const uint32_t LastGenericJoystickButton = static_cast<uint32_t>(Buttons::JOYSTICK_GENERIC_BUTTON_127);

			const uint32_t CurrentButtonInt = static_cast<uint32_t>(TargetButton);

			if (CurrentButtonInt >= FirstKeyboardKey && CurrentButtonInt <= LastKeyboardKey)
			{
				if (PlayerIndex == 0)
					ButtonState = SDL_GetKeyboardState(nullptr)[KeyboardKeyToSDLScancode(TargetButton)];
			}
			else if (CurrentButtonInt >= FirstGenericJoystickButton && CurrentButtonInt <= LastGenericJoystickButton)
			{
				ButtonState = GetJoystickButton(m_Gamepads[PlayerIndex].m_pJoystick, CurrentButtonInt - FirstGenericJoystickButton);
			}
			else
			{
				switch (TargetButton)
				{
				case Buttons::MOUSE_CLICK_LEFT:
					if (PlayerIndex == 0) {
						Uint32 MouseState = SDL_GetMouseState(nullptr, nullptr);
						ButtonState = SDL_BUTTON(SDL_BUTTON_LEFT) & MouseState;
					}
					break;
				case Buttons::MOUSE_CLICK_MIDDLE:
					if (PlayerIndex == 0) {
						Uint32 MouseState = SDL_GetMouseState(nullptr, nullptr);
						ButtonState = SDL_BUTTON(SDL_BUTTON_MIDDLE) & MouseState;
					}
					break;
				case Buttons::MOUSE_CLICK_RIGHT:
					if (PlayerIndex == 0) {
						Uint32 MouseState = SDL_GetMouseState(nullptr, nullptr);
						ButtonState = SDL_BUTTON(SDL_BUTTON_RIGHT) & MouseState;
					}
					break;
				case Buttons::GAMEPAD_BUTTON_A:
					if (m_Gamepads[PlayerIndex].m_pGameController)
					{
						int Button = SDL_GameControllerGetBindForButton(m_Gamepads[PlayerIndex].m_pGameController, SDL_CONTROLLER_BUTTON_A).value.button;
						ButtonState = GetJoystickButton(m_Gamepads[PlayerIndex].m_pJoystick, Button);
					}
					break;
				case Buttons::GAMEPAD_BUTTON_B:
					if (m_Gamepads[PlayerIndex].m_pGameController)
					{
						int Button = SDL_GameControllerGetBindForButton(m_Gamepads[PlayerIndex].m_pGameController, SDL_CONTROLLER_BUTTON_B).value.button;
						ButtonState = GetJoystickButton(m_Gamepads[PlayerIndex].m_pJoystick, Button);
					}
					break;
				case Buttons::GAMEPAD_BUTTON_X:
					if (m_Gamepads[PlayerIndex].m_pGameController)
					{
						int Button = SDL_GameControllerGetBindForButton(m_Gamepads[PlayerIndex].m_pGameController, SDL_CONTROLLER_BUTTON_X).value.button;
						ButtonState = GetJoystickButton(m_Gamepads[PlayerIndex].m_pJoystick, Button);
					}
					break;
				case Buttons::GAMEPAD_BUTTON_Y:
					if (m_Gamepads[PlayerIndex].m_pGameController)
					{
						int Button = SDL_GameControllerGetBindForButton(m_Gamepads[PlayerIndex].m_pGameController, SDL_CONTROLLER_BUTTON_Y).value.button;
						ButtonState = GetJoystickButton(m_Gamepads[PlayerIndex].m_pJoystick, Button);
					}
					break;
				case Buttons::GAMEPAD_BUTTON_LEFT_BUMPER:
					if (m_Gamepads[PlayerIndex].m_pGameController)
					{
						int Button = SDL_GameControllerGetBindForButton(m_Gamepads[PlayerIndex].m_pGameController, SDL_CONTROLLER_BUTTON_LEFTSHOULDER).value.button;
						ButtonState = GetJoystickButton(m_Gamepads[PlayerIndex].m_pJoystick, Button);
					}
					break;
				case Buttons::GAMEPAD_BUTTON_RIGHT_BUMPER:
					if (m_Gamepads[PlayerIndex].m_pGameController)
					{
						int Button = SDL_GameControllerGetBindForButton(m_Gamepads[PlayerIndex].m_pGameController, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER).value.button;
						ButtonState = GetJoystickButton(m_Gamepads[PlayerIndex].m_pJoystick, Button);
					}
					break;
				case Buttons::GAMEPAD_BUTTON_START:
					if (m_Gamepads[PlayerIndex].m_pGameController)
					{
						int Button = SDL_GameControllerGetBindForButton(m_Gamepads[PlayerIndex].m_pGameController, SDL_CONTROLLER_BUTTON_START).value.button;
						ButtonState = GetJoystickButton(m_Gamepads[PlayerIndex].m_pJoystick, Button);
					}
					break;
				case Buttons::GAMEPAD_BUTTON_SELECT:
					if (m_Gamepads[PlayerIndex].m_pGameController)
					{
						int Button = SDL_GameControllerGetBindForButton(m_Gamepads[PlayerIndex].m_pGameController, SDL_CONTROLLER_BUTTON_BACK).value.button;
						ButtonState = GetJoystickButton(m_Gamepads[PlayerIndex].m_pJoystick, Button);
					}
					break;
				case Buttons::GAMEPAD_BUTTON_LEFT_THUMB:
					if (m_Gamepads[PlayerIndex].m_pGameController)
					{
						int Button = SDL_GameControllerGetBindForButton(m_Gamepads[PlayerIndex].m_pGameController, SDL_CONTROLLER_BUTTON_LEFTSTICK).value.button;
						ButtonState = GetJoystickButton(m_Gamepads[PlayerIndex].m_pJoystick, Button);
					}
					break;
				case Buttons::GAMEPAD_BUTTON_RIGHT_THUMB:
					if (m_Gamepads[PlayerIndex].m_pGameController)
					{
						int Button = SDL_GameControllerGetBindForButton(m_Gamepads[PlayerIndex].m_pGameController, SDL_CONTROLLER_BUTTON_RIGHTSTICK).value.button;
						ButtonState = GetJoystickButton(m_Gamepads[PlayerIndex].m_pJoystick, Button);
					}
					break;
				case Buttons::GAMEPAD_BUTTON_DPAD_UP:
					if (m_Gamepads[PlayerIndex].m_pGameController)
					{
						int Button = SDL_GameControllerGetBindForButton(m_Gamepads[PlayerIndex].m_pGameController, SDL_CONTROLLER_BUTTON_DPAD_UP).value.button;
						ButtonState = GetJoystickButton(m_Gamepads[PlayerIndex].m_pJoystick, Button);
					}
					break;
				case Buttons::GAMEPAD_BUTTON_DPAD_RIGHT:
					if (m_Gamepads[PlayerIndex].m_pGameController)
					{
						int Button = SDL_GameControllerGetBindForButton(m_Gamepads[PlayerIndex].m_pGameController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT).value.button;
						ButtonState = GetJoystickButton(m_Gamepads[PlayerIndex].m_pJoystick, Button);
					}
					break;
				case Buttons::GAMEPAD_BUTTON_DPAD_DOWN:
					if (m_Gamepads[PlayerIndex].m_pGameController)
					{
						int Button = SDL_GameControllerGetBindForButton(m_Gamepads[PlayerIndex].m_pGameController, SDL_CONTROLLER_BUTTON_DPAD_DOWN).value.button;
						ButtonState = GetJoystickButton(m_Gamepads[PlayerIndex].m_pJoystick, Button);
					}
					break;
				case Buttons::GAMEPAD_BUTTON_DPAD_LEFT:
					if (m_Gamepads[PlayerIndex].m_pGameController)
					{
						int Button = SDL_GameControllerGetBindForButton(m_Gamepads[PlayerIndex].m_pGameController, SDL_CONTROLLER_BUTTON_DPAD_LEFT).value.button;
						ButtonState = GetJoystickButton(m_Gamepads[PlayerIndex].m_pJoystick, Button);
					}
					break;
				}
			}
		}

		return ButtonState;
	}

	float SDL2InputMappingResolver::GetJoystickAxis(SDL_Joystick* Joystick, int JoystickAxis)
	{
		float AxisValue = 0.0f;

		if (Joystick)
		{
			AxisValue = static_cast<float>(SDL_JoystickGetAxis(Joystick, JoystickAxis));
			AxisValue /= 32767.0f;
			AxisValue = fminf(fmaxf(AxisValue, -1.0f), 1.0f);

			//Note: AxisValue is divided by 32767.0 as the output of SDL_JoystickGetAxis(Joystick, JoystickAxis)
			//is between -32768.0 and 32767.0, so to convert this to a range between -1.0f and 1.0f we can divide
			//by 32767.0 and then clamp between -1 and 1
		}

		return AxisValue;
	}

	bool SDL2InputMappingResolver::GetJoystickButton(SDL_Joystick* Joystick, int JoystickButton)
	{
		bool ButtonState = false;

		if (Joystick)
			ButtonState = SDL_JoystickGetButton(Joystick, JoystickButton);

		return ButtonState;
	}

	void SDL2InputMappingResolver::ResizeJoystickArray()
	{
		if (m_MaxGamepadCount != m_Gamepads.size())
			m_Gamepads.resize(m_MaxGamepadCount, SDL_GamepadWrapper());
	}

	SDL_Scancode SDL2InputMappingResolver::KeyboardKeyToSDLScancode(Buttons Button)
	{
		SDL_Scancode Scancode = SDL_Scancode();
		const uint32_t FirstKeyboardKey = static_cast<uint32_t>(Buttons::KEYBOARD_KEY_SPACE);
		const uint32_t LastKeyboardKey = static_cast<uint32_t>(Buttons::KEYBOARD_KEY_MENU);
		const uint32_t CurrentButtonInt = static_cast<uint32_t>(Button);

		const uint32_t Zero = static_cast<uint32_t>(Buttons::KEYBOARD_KEY_0);
		const uint32_t One = static_cast<uint32_t>(Buttons::KEYBOARD_KEY_1);
		const uint32_t Nine = static_cast<uint32_t>(Buttons::KEYBOARD_KEY_9);

		const uint32_t A = static_cast<uint32_t>(Buttons::KEYBOARD_KEY_A);
		const uint32_t Z = static_cast<uint32_t>(Buttons::KEYBOARD_KEY_Z);

		const uint32_t F1 = static_cast<uint32_t>(Buttons::KEYBOARD_KEY_F1);
		const uint32_t F12 = static_cast<uint32_t>(Buttons::KEYBOARD_KEY_F12);

		const uint32_t KeypadZero = static_cast<uint32_t>(Buttons::KEYBOARD_KEY_KP_0);
		const uint32_t KeypadOne = static_cast<uint32_t>(Buttons::KEYBOARD_KEY_KP_1);
		const uint32_t KeypadNine = static_cast<uint32_t>(Buttons::KEYBOARD_KEY_KP_9);

		if (CurrentButtonInt >= FirstKeyboardKey && CurrentButtonInt <= LastKeyboardKey)
		{
			if (CurrentButtonInt == Zero)
			{
				Scancode = SDL_SCANCODE_0;
			}
			else if (CurrentButtonInt >= One && CurrentButtonInt <= Nine)
			{
				Scancode = static_cast<SDL_Scancode>((CurrentButtonInt - One) + static_cast<uint32_t>(SDL_SCANCODE_1));
			}
			else if (CurrentButtonInt >= A && CurrentButtonInt <= Z)
			{
				Scancode = static_cast<SDL_Scancode>((CurrentButtonInt - A) + static_cast<uint32_t>(SDL_SCANCODE_A));
			}
			else if (CurrentButtonInt >= F1 && CurrentButtonInt <= F12)
			{
				Scancode = static_cast<SDL_Scancode>((CurrentButtonInt - F1) + static_cast<uint32_t>(SDL_SCANCODE_F1));
			}
			else if (CurrentButtonInt == KeypadZero)
			{
				Scancode = SDL_SCANCODE_KP_0;
			}
			else if (CurrentButtonInt >= KeypadOne && CurrentButtonInt <= KeypadNine)
			{
				Scancode = static_cast<SDL_Scancode>((CurrentButtonInt - KeypadOne) + static_cast<uint32_t>(SDL_SCANCODE_KP_1));
			}
			else
			{
				switch (Button)
				{
				case Buttons::KEYBOARD_KEY_SPACE:
					Scancode = SDL_SCANCODE_SPACE;
					break;
				case Buttons::KEYBOARD_KEY_APOSTROPHE:
					Scancode = SDL_SCANCODE_APOSTROPHE;
					break;
				case Buttons::KEYBOARD_KEY_COMMA:
					Scancode = SDL_SCANCODE_COMMA;
					break;
				case Buttons::KEYBOARD_KEY_MINUS:
					Scancode = SDL_SCANCODE_MINUS;
					break;
				case Buttons::KEYBOARD_KEY_PERIOD:
					Scancode = SDL_SCANCODE_PERIOD;
					break;
				case Buttons::KEYBOARD_KEY_SLASH:
					Scancode = SDL_SCANCODE_SLASH;
					break;
				case Buttons::KEYBOARD_KEY_SEMICOLON:
					Scancode = SDL_SCANCODE_SEMICOLON;
					break;
				case Buttons::KEYBOARD_KEY_EQUAL:
					Scancode = SDL_SCANCODE_EQUALS;
					break;
				case Buttons::KEYBOARD_KEY_LEFT_BRACKET:
					Scancode = SDL_SCANCODE_LEFTBRACKET;
					break;
				case Buttons::KEYBOARD_KEY_BACKSLASH:
					Scancode = SDL_SCANCODE_BACKSLASH;
					break;
				case Buttons::KEYBOARD_KEY_RIGHT_BRACKET:
					Scancode = SDL_SCANCODE_RIGHTBRACKET;
					break;
				case Buttons::KEYBOARD_KEY_GRAVE_ACCENT:
					Scancode = SDL_SCANCODE_GRAVE;
					break;
				case Buttons::KEYBOARD_KEY_WORLD_1:
					Scancode = SDL_SCANCODE_INTERNATIONAL1;
					break;
				case Buttons::KEYBOARD_KEY_WORLD_2:
					Scancode = SDL_SCANCODE_INTERNATIONAL2;
					break;
				case Buttons::KEYBOARD_KEY_ESCAPE:
					Scancode = SDL_SCANCODE_ESCAPE;
					break;
				case Buttons::KEYBOARD_KEY_ENTER:
					Scancode = SDL_SCANCODE_RETURN;
					break;
				case Buttons::KEYBOARD_KEY_TAB:
					Scancode = SDL_SCANCODE_TAB;
					break;
				case Buttons::KEYBOARD_KEY_BACKSPACE:
					Scancode = SDL_SCANCODE_BACKSPACE;
					break;
				case Buttons::KEYBOARD_KEY_INSERT:
					Scancode = SDL_SCANCODE_INSERT;
					break;
				case Buttons::KEYBOARD_KEY_DELETE:
					Scancode = SDL_SCANCODE_DELETE;
					break;
				case Buttons::KEYBOARD_KEY_RIGHT:
					Scancode = SDL_SCANCODE_RIGHT;
					break;
				case Buttons::KEYBOARD_KEY_LEFT:
					Scancode = SDL_SCANCODE_LEFT;
					break;
				case Buttons::KEYBOARD_KEY_DOWN:
					Scancode = SDL_SCANCODE_DOWN;
					break;
				case Buttons::KEYBOARD_KEY_UP:
					Scancode = SDL_SCANCODE_UP;
					break;
				case Buttons::KEYBOARD_KEY_PAGE_UP:
					Scancode = SDL_SCANCODE_PAGEUP;
					break;
				case Buttons::KEYBOARD_KEY_PAGE_DOWN:
					Scancode = SDL_SCANCODE_PAGEDOWN;
					break;
				case Buttons::KEYBOARD_KEY_HOME:
					Scancode = SDL_SCANCODE_HOME;
					break;
				case Buttons::KEYBOARD_KEY_END:
					Scancode = SDL_SCANCODE_END;
					break;
				case Buttons::KEYBOARD_KEY_CAPS_LOCK:
					Scancode = SDL_SCANCODE_CAPSLOCK;
					break;
				case Buttons::KEYBOARD_KEY_SCROLL_LOCK:
					Scancode = SDL_SCANCODE_SCROLLLOCK;
					break;
				case Buttons::KEYBOARD_KEY_NUM_LOCK:
					Scancode = SDL_SCANCODE_NUMLOCKCLEAR;
					break;
				case Buttons::KEYBOARD_KEY_PRINT_SCREEN:
					Scancode = SDL_SCANCODE_PRINTSCREEN;
					break;
				case Buttons::KEYBOARD_KEY_PAUSE:
					Scancode = SDL_SCANCODE_PAUSE;
					break;
				case Buttons::KEYBOARD_KEY_KP_DECIMAL:
					Scancode = SDL_SCANCODE_KP_DECIMAL;
					break;
				case Buttons::KEYBOARD_KEY_KP_DIVIDE:
					Scancode = SDL_SCANCODE_KP_DIVIDE;
					break;
				case Buttons::KEYBOARD_KEY_KP_MULTIPLY:
					Scancode = SDL_SCANCODE_KP_MULTIPLY;
					break;
				case Buttons::KEYBOARD_KEY_KP_SUBTRACT:
					Scancode = SDL_SCANCODE_KP_MINUS;
					break;
				case Buttons::KEYBOARD_KEY_KP_ADD:
					Scancode = SDL_SCANCODE_KP_PLUS;
					break;
				case Buttons::KEYBOARD_KEY_KP_ENTER:
					Scancode = SDL_SCANCODE_KP_ENTER;
					break;
				case Buttons::KEYBOARD_KEY_KP_EQUAL:
					Scancode = SDL_SCANCODE_KP_EQUALS;
					break;
				case Buttons::KEYBOARD_KEY_LEFT_SHIFT:
					Scancode = SDL_SCANCODE_LSHIFT;
					break;
				case Buttons::KEYBOARD_KEY_LEFT_CONTROL:
					Scancode = SDL_SCANCODE_LCTRL;
					break;
				case Buttons::KEYBOARD_KEY_LEFT_ALT:
					Scancode = SDL_SCANCODE_LALT;
					break;
				case Buttons::KEYBOARD_KEY_RIGHT_SHIFT:
					Scancode = SDL_SCANCODE_RSHIFT;
					break;
				case Buttons::KEYBOARD_KEY_RIGHT_CONTROL:
					Scancode = SDL_SCANCODE_RCTRL;
					break;
				case Buttons::KEYBOARD_KEY_RIGHT_ALT:
					Scancode = SDL_SCANCODE_RALT;
					break;
				case Buttons::KEYBOARD_KEY_MENU:
					Scancode = SDL_SCANCODE_MENU;
					break;
				}
			}
		}

		return Scancode;
	}
}