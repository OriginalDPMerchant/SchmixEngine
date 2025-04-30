#include <SchmixEngine/ThirdParty/GLM/glm.hpp>

namespace SchmixEngine
{
	struct Vertex3D
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec4 Colour;
		glm::vec2 TextureCoordinate;
	};
} 