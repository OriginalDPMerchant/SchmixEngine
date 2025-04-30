#include <SchmixEngine/OpenGL/OpenGLGraphicsAPIFlavour.h>

namespace SchmixEngine
{
	bool OpenGLGraphicsAPIFlavour::InitOpenGL(const GraphicsAPIInitInfo& Info)
	{
		SMX_DBG_LOG("The Function \"InitOpenGL()\" has not been implemented in the class inheriting from \"SchmixEngine::OpenGLGraphicsAPIFlavour\"", LogLevel::Warning);
		return false;
	}

	bool OpenGLGraphicsAPIFlavour::InitWindow(const GraphicsAPIInitInfo& Info)
	{
		SMX_DBG_LOG("The Function \"InitWindow()\" has not been implemented in the class inheriting from \"SchmixEngine::OpenGLGraphicsAPIFlavour\"", LogLevel::Warning);
		return false;
	}

	bool OpenGLGraphicsAPIFlavour::ShutdownOpenGL()
	{
		SMX_DBG_LOG("The Function \"ShutdownOpenGL()\" has not been implemented in the class inheriting from \"SchmixEngine::OpenGLGraphicsAPIFlavour\"", LogLevel::Warning);
		return false;
	}

	bool OpenGLGraphicsAPIFlavour::ShutdownWindow()
	{
		SMX_DBG_LOG("The Function \"ShutdownWindow()\" has not been implemented in the class inheriting from \"SchmixEngine::OpenGLGraphicsAPIFlavour\"", LogLevel::Warning);
		return false;
	}

	void OpenGLGraphicsAPIFlavour::PresentFrame()
	{
		SMX_DBG_LOG("The Function \"PresentFrame()\" has not been implemented in the class inheriting from \"SchmixEngine::OpenGLGraphicsAPIFlavour\"", LogLevel::Warning);
	}
}
