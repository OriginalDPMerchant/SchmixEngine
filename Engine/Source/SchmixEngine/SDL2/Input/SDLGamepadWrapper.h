#pragma once
#include <SchmixEngine/ThirdParty/SDL2/include/SDL.h>

namespace SchmixEngine
{
	//A wrappper for SDL2 Gamepad and Joystick objects
	struct SDL_GamepadWrapper
	{
		SDL_GamepadWrapper();
		~SDL_GamepadWrapper();

		void Close();

		SDL_Joystick* m_pJoystick;
		SDL_GameController* m_pGameController;
	};
}