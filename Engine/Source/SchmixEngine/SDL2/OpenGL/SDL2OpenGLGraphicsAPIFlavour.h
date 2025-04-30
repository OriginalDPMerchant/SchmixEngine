#pragma once
#include <SchmixEngine/OpenGL/OpenGLGraphicsAPI.h>
#include <SchmixEngine/ThirdParty/SDL2/include/SDL.h>

namespace SchmixEngine
{
	class SDL2OpenGLGraphicsAPIFlavour : public OpenGLGraphicsAPIFlavour
	{
	public:
		SDL2OpenGLGraphicsAPIFlavour();
		virtual ~SDL2OpenGLGraphicsAPIFlavour();

		bool InitOpenGL(const GraphicsAPIInitInfo& Info);
		bool InitWindow(const GraphicsAPIInitInfo& Info);

		bool ShutdownOpenGL();
		bool ShutdownWindow();

		void PresentFrame();

	private:
		SDL_Window* m_pWindow;
		SDL_GLContext m_pGLContext;
	};

	typedef OpenGLGraphicsAPI<SDL2OpenGLGraphicsAPIFlavour> SDL2OpenGLGraphicsAPI;
}