#include <SchmixEngine/Core/Graphics/Renderer/SchmixEngineRenderSubSystemBase.h>

namespace SchmixEngine
{
	template<typename T>
	inline SchmixEngineRenderSubSystemBase<T>::SchmixEngineRenderSubSystemBase(const std::string& SubSystemName)
		: GraphicsAPIAgnosticRenderSubSystem<T>(SubSystemName),
		m_NumDrawCalls(0),
		m_VertexBufferHandle(-1),
		m_ModelMatrixBufferHandle(-1),
		m_ElementBufferHandle(-1),
		m_PerLayerUniformBufferHandle(-1),
		m_PerMaterialUniformBufferHandle(-1),
		m_DesiredVertexBufferSize(1),
		m_DesiredModelMatrixBufferSize(1),
		m_DesiredElementBufferSize(1),
		m_DesiredPerLayerUniformBufferSize(1),
		m_DesiredPerMaterialUniformBufferSize(1)
	{
	}

	template<typename T>
	inline int64_t SchmixEngineRenderSubSystemBase<T>::UploadMesh(MeshUploadInfo& UploadInfo)
	{
		int64_t MeshDataID = -1;

		if (UploadInfo.pVertexData && UploadInfo.pElementData)
		{
			uint8_t* VertexBufferMemory = static_cast<uint8_t*>(m_GraphicsAPI.MapBufferMemory(m_VertexBufferHandle, false));
			uint8_t* ElementufferMemory = static_cast<uint8_t*>(m_GraphicsAPI.MapBufferMemory(m_ElementBufferHandle, false));

			if (VertexBufferMemory && ElementufferMemory)
			{
				BufferMemoryBlockPtr VertexDataPtr = m_VertexBufferMemoryManager.Malloc(UploadInfo.VertexDataSize, UploadInfo.VertexStride);
				BufferMemoryBlockPtr ElementDataPtr = m_ElementBufferMemoryManager.Malloc(UploadInfo.ElementDataSize, sizeof(uint32_t));

				if (VertexDataPtr.bIsValid && ElementDataPtr.bIsValid)
				{
					for (size_t i = 0; i < m_FreeMeshSlots.size(); i++)
					{
						int64_t FreeSlot = m_FreeMeshSlots[i];

						if (!m_Meshes[FreeSlot].bMeshDataValid)
						{
							MeshDataID = FreeSlot;
							m_FreeMeshSlots.erase(m_FreeMeshSlots.begin() + i);
							break;
						}
					}

					if (MeshDataID == -1)
					{
						MeshDataID = m_Meshes.size();
						m_Meshes.push_back(MeshDataPtr{});
					}

					m_Meshes[MeshDataID].VertexDataOffset = VertexDataPtr.Offset;
					m_Meshes[MeshDataID].VertexDataSize = UploadInfo.VertexDataSize;

					m_Meshes[MeshDataID].ElementDataOffset = ElementDataPtr.Offset;
					m_Meshes[MeshDataID].ElementDataSize = UploadInfo.ElementDataSize;

					m_Meshes[MeshDataID].bMeshDataValid = true;

					memcpy(VertexBufferMemory + VertexDataPtr.Offset, UploadInfo.pVertexData, UploadInfo.VertexDataSize);
					memcpy(ElementufferMemory + ElementDataPtr.Offset, UploadInfo.pElementData, UploadInfo.ElementDataSize);
				}
			}

			m_GraphicsAPI.UnMapBufferMemory(m_VertexBufferHandle);
			m_GraphicsAPI.UnMapBufferMemory(m_ElementBufferHandle);
		}

		return MeshDataID;
	}

	template<typename T>
	inline void SchmixEngineRenderSubSystemBase<T>::DestroyMesh(int64_t MeshID)
	{
		if (IsMeshValid(MeshID))
		{
			m_VertexBufferMemoryManager.Free(m_Meshes[MeshID].VertexDataOffset);
			m_ElementBufferMemoryManager.Free(m_Meshes[MeshID].ElementDataOffset);

			m_VertexBufferMemoryManager.MergeAdjacentFreeMemory();
			m_ElementBufferMemoryManager.MergeAdjacentFreeMemory();

			m_Meshes[MeshID].bMeshDataValid = false;

			m_FreeMeshSlots.push_back(MeshID);
		}
	}

	//TODO: USE MEMORY MANAGER SO THAT OFFSET IS ALIGNED TO STRIDE
	template<typename T>
	inline void SchmixEngineRenderSubSystemBase<T>::SubmitMeshToRenderQueue(MeshRenderInfo& RenderInfo)
	{
		if (IsMeshValid(RenderInfo.MeshID))
		{
			uint64_t DrawCallCode = MakeDrawCallCode(RenderInfo);

			if (IsDrawCallCodeValid(DrawCallCode))
			{
				uint8_t* ModelMatrixBufferMemory = static_cast<uint8_t*>(m_GraphicsAPI.MapBufferMemory(m_ModelMatrixBufferHandle, false));

				if (ModelMatrixBufferMemory)
				{
					BufferMemoryBlockPtr PerInstanceDataPtr = m_ModelMatrixMemoryManager.Malloc(RenderInfo.PerInstanceDataSize, RenderInfo.PerInstanceDataStride);

					if (PerInstanceDataPtr.bIsValid)
					{
						if (m_NumDrawCalls >= m_DrawCalls.size())
						{
							m_NumDrawCalls = m_DrawCalls.size();
							m_DrawCalls.push_back(DrawCall{});
						}

						m_DrawCalls[m_NumDrawCalls] = { RenderInfo.MeshID, PerInstanceDataPtr.Offset, RenderInfo.PerInstanceDataStride, DrawCallCode };

						m_NumDrawCalls++;

						memcpy(ModelMatrixBufferMemory + PerInstanceDataPtr.Offset, RenderInfo.pPerInstanceData, RenderInfo.PerInstanceDataSize);
					}
				}

				m_GraphicsAPI.UnMapBufferMemory(m_ModelMatrixBufferHandle);
			}
		}
	}

	template<typename T>
	inline bool SchmixEngineRenderSubSystemBase<T>::IsMeshValid(int64_t MeshID)
	{
		return MeshID > -1 && MeshID < m_Meshes.size() ? m_Meshes[MeshID].bMeshDataValid : false;
	}

	template<typename T>
	inline int64_t SchmixEngineRenderSubSystemBase<T>::CreateMaterial(int64_t GraphicsPipelineHandle)
	{
		int64_t MaterialID = -1;

		if (m_GraphicsAPI.IsGraphicsPipelineHandleValid(GraphicsPipelineHandle))
		{
			std::vector<ShaderParameter> ShaderParameters = m_GraphicsAPI.GetGraphicsPipelineShaderPatameters(GraphicsPipelineHandle);
			std::vector<TextureSlot> ShaderTextures = m_GraphicsAPI.GetGraphicsPipelineTextureSlots(GraphicsPipelineHandle);

			ShaderData* pShaderData = m_GraphicsAPI.CreateShaderData(ShaderParameters);

			for (size_t i = 0; i < m_FreeMaterialSlots.size(); i++)
			{
				int64_t FreeSlot = m_FreeMaterialSlots[i];

				if (!m_Materials[FreeSlot].pMaterial)
				{
					MaterialID = FreeSlot;
					m_FreeMaterialSlots.erase(m_FreeMaterialSlots.begin() + i);

					break;
				}
			}

			if (MaterialID == -1)
			{
				//Ensure that MaterialID can always be represented by 16 bits 
				if (m_Materials.size() < std::numeric_limits<uint16_t>::max())
				{
					MaterialID = m_Materials.size();
					m_Materials.push_back(MaterialPtr{});
				}
			}

			if(MaterialID != -1)
				m_Materials[MaterialID] = { std::make_unique<Material>(GraphicsPipelineHandle, pShaderData, ShaderTextures), 0 };
		}

		return MaterialID;
	}

	template<typename T>
	inline void SchmixEngineRenderSubSystemBase<T>::DestroyMaterial(int64_t MaterialID)
	{
		if (IsMaterialValid(MaterialID))
		{
			m_Materials[MaterialID].pMaterial.reset();

			m_FreeMaterialSlots.push_back(MaterialID);
		}
	}

	template<typename T>
	inline Material* SchmixEngineRenderSubSystemBase<T>::GetMaterial(int64_t MaterialID)
	{
		return IsMaterialValid(MaterialID) ? m_Materials[MaterialID].pMaterial.get() : nullptr;
	}

	template<typename T>
	inline bool SchmixEngineRenderSubSystemBase<T>::IsMaterialValid(int64_t MaterialID)
	{
		return MaterialID > -1 && MaterialID < m_Materials.size() ? m_Materials[MaterialID].pMaterial.get() : false;
	}

	template<typename T>
	inline int8_t SchmixEngineRenderSubSystemBase<T>::CreateRenderLayer(RenderLayerCreateInfo& RenderLayerInfo)
	{
		int8_t RenderLayerID = -1;

		std::vector<ShaderParameter> ShaderParameters(RenderLayerInfo.NumRenderLayerParameters);

		if (RenderLayerInfo.pRenderLayerParameters)
			memcpy(ShaderParameters.data(), RenderLayerInfo.pRenderLayerParameters, ShaderParameters.size());

		ShaderData* pRenderLayerData = m_GraphicsAPI.CreateShaderData(ShaderParameters);

		for (size_t i = 0; i < m_RenderLayers.size(); i++)
		{
			if (!m_RenderLayers[i].pRenderLayer)
			{
				RenderLayerID = i;
				m_RenderLayers[i] = { std::make_unique<RenderLayer>(RenderLayerInfo.RenderLayerTargetFrameBuffer, pRenderLayerData), 0 };
				break;
			}
			}

		return RenderLayerID;
	}

	template<typename T>
	inline void SchmixEngineRenderSubSystemBase<T>::DestroyRenderLayer(int8_t RenderLayerID)
	{
		if (IsRenderLayerValid(RenderLayerID))
			m_RenderLayers[RenderLayerID].pRenderLayer.reset();
	}

	template<typename T>
	inline RenderLayer* SchmixEngineRenderSubSystemBase<T>::GetRenderLayer(int8_t RenderLayerID)
	{
		return IsRenderLayerValid(RenderLayerID) ? m_RenderLayers[RenderLayerID].pRenderLayer.get() : nullptr;
	}

	template<typename T>
	inline bool SchmixEngineRenderSubSystemBase<T>::IsRenderLayerValid(int8_t RenderLayerID)
	{
		return RenderLayerID > -1 && RenderLayerID < m_RenderLayers.size() ? m_RenderLayers[RenderLayerID].pRenderLayer.get() : false;
	}

	template<typename T>
	inline bool SchmixEngineRenderSubSystemBase<T>::UploadMaterialData()
	{
		bool bSuccess = true;

		size_t MaterialUniformBufferSize = m_GraphicsAPI.GetBufferSize(m_PerMaterialUniformBufferHandle);

		size_t Alignment = m_GraphicsAPI.GetUniformBufferAlignment();

		size_t Offset = 0;

		uint8_t* MaterialDataBufferMemory = static_cast<uint8_t*>(m_GraphicsAPI.MapBufferMemory(m_PerMaterialUniformBufferHandle, false));

		if (MaterialDataBufferMemory)
		{
			for (MaterialPtr& Ptr : m_Materials)
			{
				if (Ptr.pMaterial)
				{
					ShaderData* Data = Ptr.pMaterial->GetShaderData();

					if (Data)
					{
						if (MaterialUniformBufferSize >= Offset + Data->GetShaderDataSize())
						{
							Ptr.UniformBufferOffset = Offset;
							memcpy(MaterialDataBufferMemory + Offset, Data->GetShaderDataPtr(), Data->GetShaderDataSize());

							Offset = Util::AlignOffset(Offset + Data->GetShaderDataSize(), Alignment);
						}
						else
						{
							bSuccess = false;
							break;
						}
					}
				}
			}
		}
		else
		{
			bSuccess = false;
		}

		m_GraphicsAPI.UnMapBufferMemory(m_PerMaterialUniformBufferHandle);

		return bSuccess;
	}

	template<typename T>
	inline bool SchmixEngineRenderSubSystemBase<T>::UploadRenderLayerData()
	{
		bool bSuccess = true;

		size_t LayerUniformBufferSize = m_GraphicsAPI.GetBufferSize(m_PerLayerUniformBufferHandle);

		size_t Alignment = m_GraphicsAPI.GetUniformBufferAlignment();

		size_t Offset = 0;

		uint8_t* LayerDataBufferMemory = static_cast<uint8_t*>(m_GraphicsAPI.MapBufferMemory(m_PerLayerUniformBufferHandle, false));

		if (LayerDataBufferMemory)
		{
			for (RenderLayerPtr& Ptr : m_RenderLayers)
			{
				if (Ptr.pRenderLayer)
				{
					ShaderData* Data = Ptr.pRenderLayer->GetRenderLayerData();

					if (Data)
					{
						if (LayerUniformBufferSize >= Offset + Data->GetShaderDataSize())
						{
							Ptr.UniformBufferOffset = Offset;
							memcpy(LayerDataBufferMemory + Offset, Data->GetShaderDataPtr(), Data->GetShaderDataSize());

							Offset = Util::AlignOffset(Offset + Data->GetShaderDataSize(), Alignment);
						}
						else
						{
							bSuccess = false;
							break;
						}
					}
				}
			}
		}
		else
		{
			bSuccess = false;
		}

		m_GraphicsAPI.UnMapBufferMemory(m_PerLayerUniformBufferHandle);

		return bSuccess;
	}

	//Todo: does depth need to be reversed? 1- Depth
	template<typename T>
	inline uint64_t SchmixEngineRenderSubSystemBase<T>::MakeDrawCallCode(MeshRenderInfo& Info)
	{
		//| 63 - 60  | 59    | 58 - 35 | 34 - 19  | 18 - 17    | 16 - 0 |
		//| Layer ID | Blend | Depth   | Material | Validation | N/A    |

		uint64_t DrawCallCode = IsRenderLayerValid(Info.LayerID) ? static_cast<uint64_t>(Info.LayerID) << 60 : 1 << 18;

		Material* pMaterial = GetMaterial(Info.MaterialID);

		if (pMaterial)
		{
			int64_t GraphicsPipeline = pMaterial->GetGraphicsPipelineHandle();
			bool bBlendingEnabled = m_GraphicsAPI.GetGraphicsPipelineState(GraphicsPipeline).ColourBlendingInfo.bBlendingEnabled;

			DrawCallCode += bBlendingEnabled ? static_cast<uint64_t>(1) << 59 : 0;

			DrawCallCode += static_cast<uint64_t>(Info.MaterialID) << 19;
		}
		else
		{
			DrawCallCode += 1 << 17;
		}

		uint64_t Depth = 2;

		for (size_t i = 0; i < 24; i++)
			Depth *= Depth;

		Depth *= static_cast<uint64_t>(std::clamp(Info.Depth, 0.0f, 1.0f));

		DrawCallCode += Depth << 35;

		return DrawCallCode;
	}

	template<typename T>
	inline bool SchmixEngineRenderSubSystemBase<T>::IsDrawCallCodeValid(uint64_t DrawCallCode)
	{
		return (DrawCallCode & static_cast<uint64_t>(DrawCallCodeMasks::Validation)) == 0;
	}

	template<typename T>
	inline bool SchmixEngineRenderSubSystemBase<T>::CompareDrawCalls(const DrawCall& LHS, const DrawCall& RHS)
	{
		return LHS.DrawCallCode < RHS.DrawCallCode;
	}

	template<typename T>
	inline bool SchmixEngineRenderSubSystemBase<T>::InitSubSystem()
	{
		bool bSuccess = GraphicsAPIAgnosticRenderSubSystem<T>::InitSubSystem();

		if (bSuccess)
		{
			m_VertexBufferMemoryManager.Init(m_DesiredVertexBufferSize);
			m_ElementBufferMemoryManager.Init(m_DesiredElementBufferSize);
			m_ModelMatrixMemoryManager.Init(m_DesiredModelMatrixBufferSize);

			m_VertexBufferHandle = m_GraphicsAPI.CreateBuffer(nullptr, m_DesiredVertexBufferSize, BufferType::VertexBuffer, true);
			m_ModelMatrixBufferHandle = m_GraphicsAPI.CreateBuffer(nullptr, m_DesiredModelMatrixBufferSize, BufferType::VertexBuffer, true);
			m_ElementBufferHandle = m_GraphicsAPI.CreateBuffer(nullptr, m_DesiredElementBufferSize, BufferType::ElementBuffer, true);

			m_PerLayerUniformBufferHandle = m_GraphicsAPI.CreateBuffer(nullptr, m_DesiredPerLayerUniformBufferSize, BufferType::UniformBuffer, true);
			m_PerMaterialUniformBufferHandle = m_GraphicsAPI.CreateBuffer(nullptr, m_DesiredPerMaterialUniformBufferSize, BufferType::UniformBuffer, true);
		}

		return bSuccess;
	}
}