/*#pragma once 
#include <SchmixEngine/Core/Application/SubSystem.h>
#include <SchmixEngine/Core/Application/Application.h>
#include <SchmixEngine/ThirdParty/GLM/glm.hpp>
#include <SchmixEngine/ThirdParty/stb_image.h>
#include <SchmixEngine/OpenGL/OpenGLGraphicsAPI.h>
#include <SchmixEngine/OpenGL/OpenGLTexture.h>
#include <SchmixEngine/SDL2/OpenGL/SDL2OpenGLGraphicsAPIFlavour.h>
#include <SchmixEngine/Core/Graphics/Material.h>
#include <SchmixEngine/Core/Util/Util.h>
#include <SchmixEngine/Core/Graphics/GraphicsUtil.h>

namespace SchmixEngine
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Colour;
		glm::vec2 TextureCoordinate;
	};

	struct MeshDataPtr
	{
		size_t VertesDataOffset;
		size_t VertexDataSize;
		size_t ElementDataOffset;
		size_t ElementDataSize;
	};

	class RenderSubSystem : public SubSystem
	{
	public:
		RenderSubSystem(const std::string& SubSystemName);

		RenderSubSystem(const RenderSubSystem&) = delete;
		RenderSubSystem& operator=(const RenderSubSystem&) = delete;

		virtual ~RenderSubSystem();
		
		int64_t UploadMesh(void* pVertexData, size_t VertexDataSize, uint32_t* pElementData, size_t ElementDataSize, size_t VertexStride);
		void DrawMesh(int64_t MeshID, int64_t MaterialID, glm::mat4 ModelMatrix);

		int64_t CreateGraphicsPipeline(const GraphicsPipelineCreateInfo& CreateInfo);
		int64_t CreateGraphicsPipeline(const GraphicsPipelineCreateInfo_2& CreateInfo);

		int64_t CreateMaterial(int64_t GraphicsPipelineHandle);
		Material* GetMaterial(int64_t MaterialID);

		int64_t CreateTexture(TextureInfo& Info, void* Data);
		
		int64_t LoadGraphicsPipeline(const std::filesystem::path& FilePath);

		int64_t LoadTexture(const std::filesystem::path& FilePath, TextureInfo& TexInfo);

		ShaderData* GetPerFrameShaderData();

		bool IsMeshValid(int64_t MeshID);
		bool IsMaterialValid(int64_t MaterialID);

	protected:
		virtual bool InitSubSystem() override;
		virtual bool ShutdownSubSystem() override;

		virtual void UpdateSubSystem(float DeltaTime) override;

		void UploadMaterialData();

	private:
		SDL2OpenGLGraphicsAPI OpenGL;

		std::vector<MeshDataPtr> m_Meshes;

		size_t m_CurrentVertexBufferByte;
		size_t m_CurrentElementBufferByte;

		std::vector<std::pair<int64_t,int64_t>> m_MeshesToDraw;
		uint32_t m_NumMeshesToDraw;

		std::vector<std::pair<Material,size_t>> m_Materials;
		
		std::unique_ptr<ShaderData> m_pPerFrameShaderData;

		int64_t m_VertexBufferHandle;
		int64_t m_ModelMatrixBufferHandle;
		int64_t m_ElementBufferHandle;
		int64_t m_PerFrameUniformBufferHandle;
		int64_t m_PerMaterialUniformBufferHandle;
	};
}*/