#pragma once 
#include <SchmixEngine/Core/Application/SubSystem.h>
#include <SchmixEngine/Core/Application/Application.h>
#include <SchmixEngine/Core/Graphics/Core/GraphicsAPI.h>
#include <SchmixEngine/Core/Graphics/Renderer/Material.h>
#include <SchmixEngine/Core/Graphics/Renderer/RenderLayer.h>
#include <SchmixEngine/Core/Graphics/Util/GraphicsUtil.h>
#include <SchmixEngine/Core/Util/Util.h>
#include <SchmixEngine/ThirdParty/GLM/glm.hpp>
#include <SchmixEngine/ThirdParty/stb_image.h>

namespace SchmixEngine
{
	struct MeshUploadInfo
	{
		void* pVertexData;
		size_t VertexDataSize;
		size_t VertexStride; 
		uint32_t* pElementData;
		size_t ElementDataSize;
	};

	//todo add num instances
	struct MeshRenderInfo
	{
		int64_t MeshID;
		int64_t MaterialID;
		uint8_t LayerID;
		float Depth;
		void* pPerInstanceData;
		size_t PerInstanceDataSize;
		size_t PerInstanceDataStride;
	};

	class RenderSubSystem : public SubSystem
	{
	public:
		RenderSubSystem(const std::string& SubSystemName);

		RenderSubSystem(const RenderSubSystem&) = delete;
		RenderSubSystem& operator=(const RenderSubSystem&) = delete;

		virtual ~RenderSubSystem() = default;

		virtual void Render() = 0;

		virtual void SetInitInfo(const GraphicsAPIInitInfo& Info) = 0;

		virtual int64_t CreateGraphicsPipeline(const GraphicsPipelineCreateInfo& CreateInfo) = 0;

		virtual void DestroyGraphicsPipeline(int64_t PipelineID) = 0;

		virtual bool IsGraphicsPipelineValid(int64_t PipelineID) = 0;

		virtual int64_t CreateTexture(TextureInfo& Info, void* Data) = 0;

		virtual void DestroyTexture(int64_t TextureID) = 0;

		virtual bool IsTextureValid(int64_t TextureID) = 0;
		
		virtual int64_t UploadMesh(MeshUploadInfo& UploadInfo) = 0;
		
		virtual void DestroyMesh(int64_t MeshID) = 0;

		virtual void SubmitMeshToRenderQueue(MeshRenderInfo& MeshInfo) = 0;

		virtual bool IsMeshValid(int64_t MeshID) = 0;

		virtual int64_t CreateMaterial(int64_t GraphicsPipelineHandle) = 0;
		
		virtual void DestroyMaterial(int64_t MaterialID) = 0;

		virtual Material* GetMaterial(int64_t MaterialID) = 0;

		virtual bool IsMaterialValid(int64_t MaterialID) = 0;

		virtual int8_t CreateRenderLayer(RenderLayerCreateInfo& RenderLayerInfo) = 0;

		virtual void DestroyRenderLayer(int8_t RenderLayerID) = 0;

		virtual RenderLayer* GetRenderLayer(int8_t RenderLayerID) = 0;

		virtual bool IsRenderLayerValid(int8_t RenderLayerID) = 0;

		int64_t CreateGraphicsPipeline(const GraphicsPipelineCreateInfo_2& CreateInfo);
		
		int64_t LoadGraphicsPipeline(const std::filesystem::path& FilePath);

		int64_t LoadTexture(const std::filesystem::path& FilePath, TextureInfo& TexInfo);
	};
}