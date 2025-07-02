#include <SchmixEngine/ThirdParty/GLM/glm.hpp>
#include <SchmixEngine/Core/Reflection/Macros.h>

namespace SchmixEngine
{
	SMX_CLASS()
	struct Vertex2D
	{
		SMX_PROPERTY()
		glm::vec2 Position;

		SMX_PROPERTY()
		glm::vec4 Colour;

		SMX_PROPERTY()
		glm::vec2 TextureCoordinate;
	};
}