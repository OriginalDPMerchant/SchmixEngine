#pragma once
#include <SchmixEngine/Core/Graphics/Renderer/GraphicsAPIAgnosticRenderSubSystem.h>
#include <SchmixEngine/Core/Graphics/Util/MemoryManager.h>
#include <array>

namespace SchmixEngine
{
	struct MeshDataPtr
	{
		size_t VertexDataOffset;
		size_t VertexDataSize;

		size_t ElementDataOffset;
		size_t ElementDataSize;

		bool bMeshDataValid;
	};

	struct MaterialPtr
	{
		std::unique_ptr<Material> pMaterial;
		size_t UniformBufferOffset;
	};

	struct RenderLayerPtr
	{
		std::unique_ptr<RenderLayer> pRenderLayer;
		size_t UniformBufferOffset;
	};

	struct DrawCall
	{
		int64_t MeshID;
		size_t PerInstanceDataOffset;
		size_t PerInstanceDataStride;
		uint64_t DrawCallCode;
	};

	enum class DrawCallCodeMasks : uint64_t
	{
		RenderLayerID   = static_cast<uint64_t>(0x0000000F) << 60,
		BlendingEnabled = static_cast<uint64_t>(0x00000001) << 59,
		Depth			= static_cast<uint64_t>(0x00FFFFFF) << 35,
		MaterialID      = static_cast<uint64_t>(0x0000FFFF) << 19,
		Validation      = static_cast<uint64_t>(0x00000002) << 17
		
	};

	//todo: maybe follow a deffered deletion aproach for Meshes, mATERIALS AND rENDERlAYERS
	template<typename T>
	class SchmixEngineRenderSubSystemBase : public GraphicsAPIAgnosticRenderSubSystem<T>
	{
	public:
		SchmixEngineRenderSubSystemBase(const std::string& SubSystemName);

		SchmixEngineRenderSubSystemBase(const SchmixEngineRenderSubSystemBase&) = delete;
		SchmixEngineRenderSubSystemBase& operator=(const SchmixEngineRenderSubSystemBase&) = delete;

		virtual ~SchmixEngineRenderSubSystemBase() = default;

		virtual int64_t UploadMesh(MeshUploadInfo& UploadInfo) override;

		virtual void DestroyMesh(int64_t MeshID) override;

		virtual void SubmitMeshToRenderQueue(MeshRenderInfo& RenderInfo) override;

		virtual bool IsMeshValid(int64_t MeshID) override;

		virtual int64_t CreateMaterial(int64_t GraphicsPipelineHandle) override;

		virtual void DestroyMaterial(int64_t MaterialID) override;

		virtual Material* GetMaterial(int64_t MaterialID) override;

		virtual bool IsMaterialValid(int64_t MaterialID) override;

		virtual int8_t CreateRenderLayer(RenderLayerCreateInfo& RenderLayerInfo) override;

		virtual void DestroyRenderLayer(int8_t RenderLayerID) override;

		virtual RenderLayer* GetRenderLayer(int8_t RenderLayerID) override;

		virtual bool IsRenderLayerValid(int8_t RenderLayerID) override;

	protected:
		bool UploadMaterialData();

		bool UploadRenderLayerData();

		uint64_t MakeDrawCallCode(MeshRenderInfo& Info);

		static bool IsDrawCallCodeValid(uint64_t DrawCallCode);

		static bool CompareDrawCalls(const DrawCall& LHS, const DrawCall& RHS);

		virtual bool InitSubSystem() override;

		//virtual bool ShutdownSubSystem() override;

		//virtual void UpdateSubSystem(float DeltaTime) override;

		size_t m_NumDrawCalls;

		std::vector<DrawCall> m_DrawCalls;

		std::vector<MeshDataPtr> m_Meshes;
		std::vector<int64_t> m_FreeMeshSlots;

		std::vector<MaterialPtr> m_Materials;
		std::vector<int64_t> m_FreeMaterialSlots;

		std::array<RenderLayerPtr, 16> m_RenderLayers;

		BufferMemoryManager m_VertexBufferMemoryManager;
		BufferMemoryManager m_ElementBufferMemoryManager;
		BufferMemoryManager m_ModelMatrixMemoryManager;

		int64_t m_VertexBufferHandle;
		int64_t m_ModelMatrixBufferHandle;
		int64_t m_ElementBufferHandle;
		int64_t m_PerLayerUniformBufferHandle;
		int64_t m_PerMaterialUniformBufferHandle;

		size_t m_DesiredVertexBufferSize;
		size_t m_DesiredModelMatrixBufferSize;
		size_t m_DesiredElementBufferSize;
		size_t m_DesiredPerLayerUniformBufferSize;
		size_t m_DesiredPerMaterialUniformBufferSize;
	};
}

#include <SchmixEngine/Core/Graphics/Renderer/SchmixEngineRenderSubSystemBase.inl>