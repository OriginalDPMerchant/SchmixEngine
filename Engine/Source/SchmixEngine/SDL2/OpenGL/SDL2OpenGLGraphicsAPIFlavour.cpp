#include "SDL2OpenGLGraphicsAPIFlavour.h"

namespace SchmixEngine
{
	SDL2OpenGLGraphicsAPIFlavour::SDL2OpenGLGraphicsAPIFlavour()
		: m_pWindow(nullptr),
		m_pGLContext(nullptr)
	{
	}

	SDL2OpenGLGraphicsAPIFlavour::~SDL2OpenGLGraphicsAPIFlavour()
	{
	}

	bool SDL2OpenGLGraphicsAPIFlavour::InitOpenGL(const GraphicsAPIInitInfo& Info)
	{
		//Initialize SDL2 Video SubSystem
		bool Success = SDL_InitSubSystem(SDL_INIT_VIDEO) == 0;

		//Load OpenGL
		Success = Success ? SDL_GL_LoadLibrary(NULL) == 0: false;

		// Request an OpenGL 4.6 Core context
		Success = Success ? SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1) == 0 : false;
		Success = Success ? SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) == 0 : false;
		Success = Success ? SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6) == 0 : false;
		Success = Success ? SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) == 0 : false;
		
		//Enable Debug Context
		if (Info.bDebugEnabled)
			Success = Success ? SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG) == 0 : false;

		// Also request a depth buffer
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		return Success;
	}

	bool SDL2OpenGLGraphicsAPIFlavour::InitWindow(const GraphicsAPIInitInfo& Info)
	{
		if (!m_pWindow)
		{ 
			Uint32 WindowFlags = SDL_WINDOW_OPENGL;
			WindowFlags |= Info.DisplayInfo.bFullscreen ? SDL_WINDOW_FULLSCREEN : 0;
			WindowFlags |= Info.DisplayInfo.bResizeable ? SDL_WINDOW_RESIZABLE : 0;

			int Width = Info.DisplayInfo.Width;
			int Height = Info.DisplayInfo.Height;
			const char* Title = Info.ApplicationName.c_str();

			m_pWindow = SDL_CreateWindow(Title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, WindowFlags);
			bool Success = m_pWindow;

			if (Success)
				m_pGLContext = SDL_GL_CreateContext(m_pWindow);

			Success = m_pGLContext;

			Success = Success ? gladLoadGLLoader(SDL_GL_GetProcAddress) : false;

			if (Success)
				glViewport(0, 0, Width, Height);

			return Success;
		}

		return false;
	}

	bool SDL2OpenGLGraphicsAPIFlavour::ShutdownOpenGL()
	{
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		return true;
	}

	bool SDL2OpenGLGraphicsAPIFlavour::ShutdownWindow()
	{
		if (m_pGLContext)
		{
			SDL_GL_DeleteContext(m_pGLContext);
			m_pGLContext = nullptr;
		}

		if (m_pWindow)
		{
			SDL_DestroyWindow(m_pWindow);
			m_pWindow = nullptr;
		}

		return true;
	}

	void SDL2OpenGLGraphicsAPIFlavour::PresentFrame()
	{
		if (m_pWindow)
			SDL_GL_SwapWindow(m_pWindow);
	}
}
