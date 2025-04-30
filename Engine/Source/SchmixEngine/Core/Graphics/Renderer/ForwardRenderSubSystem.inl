#include <SchmixEngine/Core/Graphics/Renderer/ForwardRenderSubSystem.h>

namespace SchmixEngine
{
	template<typename T>
	inline ForwardRenderSubSystem<T>::ForwardRenderSubSystem(const std::string& SubSystemName)
		: SchmixEngineRenderSubSystemBase<T>(SubSystemName)
	{
	}

	template<typename T>
	inline void ForwardRenderSubSystem<T>::Render()
	{
		std::sort(m_DrawCalls.begin(), m_DrawCalls.begin() + m_NumDrawCalls, ForwardRenderSubSystem<T>::CompareDrawCalls);

		UploadRenderLayerData();
		UploadMaterialData();

		m_GraphicsAPI.SetClearColour(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		m_GraphicsAPI.Clear(static_cast<uint32_t>(ClearBufferBit::ColourBuffer));

		int8_t RenderLayerID = -1;
		int64_t MaterialID = -1;

		uint32_t VertexBufferStride = 0;

		for (size_t i = 0; i < m_NumDrawCalls; i++)
		{
			DrawCall& Draw = m_DrawCalls[i];

			int8_t NewRenderLayerID = static_cast<uint8_t>((Draw.DrawCallCode & static_cast<uint64_t>(DrawCallCodeMasks::RenderLayerID)) >> 60);
			int64_t NewMaterialID = (Draw.DrawCallCode & static_cast<uint64_t>(DrawCallCodeMasks::MaterialID)) >> 19;

			if (RenderLayerID != NewRenderLayerID)
			{
				RenderLayerID = NewRenderLayerID;
				RenderLayerPtr& Ptr = m_RenderLayers[RenderLayerID];
				size_t RenderLayerDataSize = Ptr.pRenderLayer->GetRenderLayerData()->GetShaderDataSize();

				m_GraphicsAPI.BindBufferRange(m_PerLayerUniformBufferHandle, 0, Ptr.UniformBufferOffset, RenderLayerDataSize);
			}

			if (MaterialID != NewMaterialID)
			{
				MaterialID = NewMaterialID;
				MaterialPtr& Ptr = m_Materials[MaterialID];
				size_t MaterialDataSize = Ptr.pMaterial->GetShaderData()->GetShaderDataSize();

				int64_t GraphicsPipelineHandle = Ptr.pMaterial->GetGraphicsPipelineHandle();

				VertexBufferStride = m_GraphicsAPI.GetGraphicsPipelineVertexBufferStrides(GraphicsPipelineHandle, 0);

				m_GraphicsAPI.BindGraphicsPipeline(GraphicsPipelineHandle);

				size_t Offset = 0;
				m_GraphicsAPI.BindVertexBuffers(0, 1, &m_VertexBufferHandle, &Offset);
				m_GraphicsAPI.BindVertexBuffers(1, 1, &m_ModelMatrixBufferHandle, &Offset);
				m_GraphicsAPI.BindElementBuffer(m_ElementBufferHandle, ElementBufferFormat::UInt32);

				m_GraphicsAPI.BindBufferRange(m_PerMaterialUniformBufferHandle, 1, Ptr.UniformBufferOffset, MaterialDataSize);

				for (const MaterialTextureSlot& Texture : Ptr.pMaterial->GetTextures())
					m_GraphicsAPI.BindTexture(Texture.Slot.TextureSlotLocation, Texture.BoundTexture);
			}

			MeshDataPtr& Mesh = m_Meshes[Draw.MeshID];

			uint32_t NumElements = Mesh.ElementDataSize / sizeof(uint32_t);
			uint32_t FirstVertex = Mesh.VertexDataOffset / VertexBufferStride;
			uint32_t FirstInstance = Draw.PerInstanceDataOffset / Draw.PerInstanceDataStride;

			m_GraphicsAPI.DrawElements(NumElements, 1, Mesh.ElementDataOffset, FirstVertex, FirstInstance);
		}

		m_GraphicsAPI.PresentFrame();

		m_NumDrawCalls = 0;
		m_ModelMatrixMemoryManager.Init(m_DesiredModelMatrixBufferSize);
	}

	template<typename T>
	inline bool ForwardRenderSubSystem<T>::InitSubSystem()
	{
		m_DesiredVertexBufferSize = 1024;
		m_DesiredModelMatrixBufferSize = 1024;
		m_DesiredElementBufferSize = 1024;
		m_DesiredPerLayerUniformBufferSize = 1024;
		m_DesiredPerMaterialUniformBufferSize = 1024;

		bool bSuccess = SchmixEngineRenderSubSystemBase<T>::InitSubSystem();

		return bSuccess;
	}

	template<typename T>
	inline void ForwardRenderSubSystem<T>::UpdateSubSystem(float DeltaTime)
	{
		SchmixEngine::SchmixEngineRenderSubSystemBase<T>::UpdateSubSystem(DeltaTime);
		Render();
	}
}