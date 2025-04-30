#include <SchmixEngine/SDL2/Input/SDLGamepadWrapper.h>

namespace SchmixEngine
{
	SDL_GamepadWrapper::SDL_GamepadWrapper()
	{
		m_pJoystick = nullptr;
		m_pGameController = nullptr;
	}

	SDL_GamepadWrapper::~SDL_GamepadWrapper()
	{
		{
			Close();
		}
	}

	void SDL_GamepadWrapper::Close()
	{
		if (m_pJoystick)
			SDL_JoystickClose(m_pJoystick);

		m_pJoystick = nullptr;

		if (m_pGameController)
			SDL_GameControllerClose(m_pGameController);

		m_pGameController = nullptr;
	}
}