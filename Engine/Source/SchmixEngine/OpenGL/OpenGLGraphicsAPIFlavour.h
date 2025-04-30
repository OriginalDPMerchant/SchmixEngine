#pragma once
#include <SchmixEngine/Core/Graphics/Core/GraphicsAPI.h>
#include <SchmixEngine/Core/Debugging/Debug.h>

namespace SchmixEngine
{
	class OpenGLGraphicsAPIFlavour
	{
	public:
		OpenGLGraphicsAPIFlavour() = default;
		virtual ~OpenGLGraphicsAPIFlavour() = default;

		bool InitOpenGL(const GraphicsAPIInitInfo& Info);
		bool InitWindow(const GraphicsAPIInitInfo& Info);

		bool ShutdownOpenGL();
		bool ShutdownWindow();

		void PresentFrame();
	};
}