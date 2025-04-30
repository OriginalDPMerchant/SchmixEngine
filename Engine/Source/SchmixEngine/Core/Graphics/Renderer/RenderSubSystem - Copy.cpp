/*/#include <SchmixEngine/OpenGL/STD140ShaderData.h>
#include <SchmixEngine/Core/Graphics/Renderer/RenderSubSystem.h>
#include <iostream>
#include<array>
#include <SchmixEngine/ThirdParty/GLM/gtx/transform.hpp>

namespace SchmixEngine
{
	RenderSubSystem::RenderSubSystem(const std::string& SubSystemName)
		: SubSystem(SubSystemName),
		m_CurrentVertexBufferByte(0),
		m_CurrentElementBufferByte(0),
		m_NumMeshesToDraw(0),
		m_VertexBufferHandle(-1),
		m_ModelMatrixBufferHandle(-1),
		m_ElementBufferHandle(-1),
		m_PerMaterialUniformBufferHandle(-1)
	{
	}

	RenderSubSystem::~RenderSubSystem()
	{
	}

	int64_t RenderSubSystem::UploadMesh(void* pVertexData, size_t VertexDataSize, uint32_t* pElementData, size_t ElementDataSize, size_t VertexStride)
	{
		size_t VertexBufferSize = OpenGL.GetBufferSize(m_VertexBufferHandle);
		size_t ElementBufferSize = OpenGL.GetBufferSize(m_ElementBufferHandle);

		size_t AlignedVertexBufferByte = Util::AlignOffset(m_CurrentVertexBufferByte, VertexStride);

		if (VertexBufferSize >= AlignedVertexBufferByte + VertexDataSize && ElementBufferSize >= m_CurrentElementBufferByte + ElementDataSize)
		{
			MeshDataPtr MeshData;
			MeshData.VertesDataOffset = AlignedVertexBufferByte;
			MeshData.VertexDataSize = VertexDataSize;
			MeshData.ElementDataOffset = m_CurrentElementBufferByte;
			MeshData.ElementDataSize = ElementDataSize;

			m_Meshes.push_back(MeshData);

			uint8_t* VertexBufferMemory = static_cast<uint8_t*>(OpenGL.MapBufferMemory(m_VertexBufferHandle, false));
			uint8_t* ElementufferMemory = static_cast<uint8_t*>(OpenGL.MapBufferMemory(m_ElementBufferHandle, false));


			if (VertexBufferMemory && ElementufferMemory)
			{
				memcpy(VertexBufferMemory + AlignedVertexBufferByte, pVertexData, VertexDataSize);
				memcpy(ElementufferMemory + m_CurrentElementBufferByte, pElementData, ElementDataSize);

				m_CurrentVertexBufferByte = AlignedVertexBufferByte + VertexDataSize;
				m_CurrentElementBufferByte += ElementDataSize;
			}

			OpenGL.UnMapBufferMemory(m_VertexBufferHandle);
			OpenGL.UnMapBufferMemory(m_ElementBufferHandle);

			return m_Meshes.size() - 1;
		}
			
		return -1;
	}

	void RenderSubSystem::DrawMesh(int64_t MeshID, int64_t MaterialID, glm::mat4 ModelMatrix)
	{
		if (OpenGL.GetBufferSize(m_ModelMatrixBufferHandle) >= (m_NumMeshesToDraw + 1) * sizeof(glm::mat4))
		{
			if (m_NumMeshesToDraw < m_MeshesToDraw.size())
				m_MeshesToDraw[m_NumMeshesToDraw] = { MeshID, MaterialID };
			else
				m_MeshesToDraw.push_back({ MeshID, MaterialID });

			uint8_t* ModelMatrixBufferMemory = static_cast<uint8_t*>(OpenGL.MapBufferMemory(m_ModelMatrixBufferHandle, false));

			if (ModelMatrixBufferMemory)
			{
				size_t Offset = m_NumMeshesToDraw * sizeof(glm::mat4);
				memcpy(ModelMatrixBufferMemory + Offset, &ModelMatrix, sizeof(glm::mat4));
			}

			OpenGL.UnMapBufferMemory(m_ModelMatrixBufferHandle);

			m_NumMeshesToDraw += 1;
		}
	}

	int64_t RenderSubSystem::CreateGraphicsPipeline(const GraphicsPipelineCreateInfo& CreateInfo)
	{
		return OpenGL.CreateGraphicsPipeline(CreateInfo);
	}

	int64_t RenderSubSystem::CreateGraphicsPipeline(const GraphicsPipelineCreateInfo_2& CreateInfo)
	{
		GraphicsPipelineCreateInfo PipelineCreateInfo{};

		std::vector<ShaderStageCreateInfo> ShaderStages(CreateInfo.ShaderStages.size());

		for (size_t i = 0; i < ShaderStages.size(); i++)
		{
			ShaderStages[i].ShaderStageType = CreateInfo.ShaderStages[i].ShaderStageType;
			ShaderStages[i].pShaderCode = CreateInfo.ShaderStages[i].ShaderCode.c_str();
		}

		std::vector<VertexInputInfo> VertexInputInfos(CreateInfo.VertexInputInfos.size());

		for (size_t i = 0; i < VertexInputInfos.size(); i++)
		{
			VertexInputInfos[i].VertexBufferBindingIndex = CreateInfo.VertexInputInfos[i].VertexBufferBindingIndex;
			VertexInputInfos[i].VertexInputRate = CreateInfo.VertexInputInfos[i].VertexInputRate;
			VertexInputInfos[i].pVertexBufferLayout = const_cast<PackedBufferLayout*>(&(CreateInfo.VertexInputInfos[i].VertexBufferLayout));
		}

		PipelineCreateInfo.NumShaderStages = ShaderStages.size();
		PipelineCreateInfo.pShaderStages = ShaderStages.data();
		PipelineCreateInfo.NumVertexInputInfos = VertexInputInfos.size();
		PipelineCreateInfo.pVertexInputInfos = VertexInputInfos.data();
		PipelineCreateInfo.PrimitiveAssemblyInfo = CreateInfo.PrimitiveAssemblyInfo;
		PipelineCreateInfo.ColourBlendingInfo = CreateInfo.ColourBlendingInfo;
		PipelineCreateInfo.NumTextureSlots = CreateInfo.TextureSlots.size();
		PipelineCreateInfo.pTextureSlots = const_cast<TextureSlot*>(CreateInfo.TextureSlots.data());
		PipelineCreateInfo.NumShaderParameters = CreateInfo.ShaderParameters.size();
		PipelineCreateInfo.pShaderParameters = const_cast<std::pair<std::string, ShaderDataType::Enum>*>(CreateInfo.ShaderParameters.data());

		return CreateGraphicsPipeline(PipelineCreateInfo);
	}

	int64_t RenderSubSystem::CreateMaterial(int64_t GraphicsPipelineHandle)
	{
		std::vector<std::pair<std::string, ShaderDataType::Enum>> ShaderParameters = OpenGL.GetGraphicsPipelineShaderPatameters(GraphicsPipelineHandle);
		std::vector<TextureSlot> ShaderTextures = OpenGL.GetGraphicsPipelineTextureSlots(GraphicsPipelineHandle);

		ShaderData* pShaderData = OpenGL.CreateShaderData(ShaderParameters);
		
		m_Materials.emplace_back(std::pair<Material, size_t>(Material(GraphicsPipelineHandle, pShaderData, ShaderTextures), 0));

		return m_Materials.size() - 1;
	}

	Material* RenderSubSystem::GetMaterial(int64_t MaterialID)
	{
		return MaterialID >= 0 && MaterialID < m_Materials.size() ? &m_Materials[MaterialID].first : nullptr;
	}

	int64_t RenderSubSystem::CreateTexture(TextureInfo& Info, void* Data)
	{
		return OpenGL.CreateTexture(Info, Data);
	}

	int64_t RenderSubSystem::LoadGraphicsPipeline(const std::filesystem::path& FilePath)
	{
		bool bSuccess = true;

		GraphicsPipelineCreateInfo_2 PipelineCreateInfo = GraphicsUtil::LoadGraphicsPipelineCreateInfo(FilePath, &bSuccess);

		return bSuccess ? CreateGraphicsPipeline(PipelineCreateInfo) : -1;
	}

	int64_t RenderSubSystem::LoadTexture(const std::filesystem::path& FilePath, TextureInfo& TexInfo)
	{
		int64_t TextureHandle = -1;

		void* TextureData = GraphicsUtil::LoadTextureData(FilePath, TexInfo);

		if (TextureData)
		{
			TextureHandle = OpenGL.CreateTexture(TexInfo, TextureData);
			free(TextureData);
		}

		return TextureHandle;
	}

	ShaderData* RenderSubSystem::GetPerFrameShaderData()
	{
		return m_pPerFrameShaderData.get();
	}

	bool RenderSubSystem::InitSubSystem()
	{
		GraphicsAPIInitInfo InitInfo;
		InitInfo.ApplicationName = "Renderer V1 OpenGL";
		InitInfo.bDebugEnabled = true;
		InitInfo.DisplayInfo.bFullscreen = false;
		InitInfo.DisplayInfo.bResizeable = false;
		InitInfo.DisplayInfo.bVSyncEnabled = false;
		InitInfo.DisplayInfo.Width = 800;
		InitInfo.DisplayInfo.Height = 450;

		OpenGL.InitGraphicsAPI(InitInfo);

		m_VertexBufferHandle = OpenGL.CreateBuffer(nullptr, 1024, BufferType::VertexBuffer, true);
		m_ModelMatrixBufferHandle = OpenGL.CreateBuffer(nullptr, 1024, BufferType::VertexBuffer, true);
		m_ElementBufferHandle = OpenGL.CreateBuffer(nullptr, 1024, BufferType::ElementBuffer, true);

		m_PerFrameUniformBufferHandle = OpenGL.CreateBuffer(nullptr, 1024, BufferType::UniformBuffer, true);
		m_PerMaterialUniformBufferHandle = OpenGL.CreateBuffer(nullptr, 1024, BufferType::UniformBuffer, true);

		m_pPerFrameShaderData = std::make_unique<STD140ShaderData>();

		return true;
	}

	bool RenderSubSystem::ShutdownSubSystem()
	{
		OpenGL.ShutdownGraphicsAPI();
		return true;
	}

	void RenderSubSystem::UpdateSubSystem(float DeltaTime)
	{
		SDL_Event Event;

		while (SDL_PollEvent(&Event))
		{
			if (Event.type == SDL_QUIT)
				Application::GetCurrentApplication()->Stop();
		}

		size_t PerFrameUniformBufferSize = OpenGL.GetBufferSize(m_PerFrameUniformBufferHandle);
		size_t PerFrameDataSize = m_pPerFrameShaderData->GetShaderDataSize();

		if (PerFrameUniformBufferSize < PerFrameDataSize)
		{
			OpenGL.QueueDestroyBuffer(m_PerFrameUniformBufferHandle);

			m_PerFrameUniformBufferHandle = m_PerFrameUniformBufferHandle = OpenGL.CreateBuffer(nullptr, PerFrameDataSize, BufferType::UniformBuffer, true);
		}

		uint8_t* PerFrameDataBufferMemory = static_cast<uint8_t*>(OpenGL.MapBufferMemory(m_PerFrameUniformBufferHandle, true));

		memcpy(PerFrameDataBufferMemory, m_pPerFrameShaderData->GetShaderDataPtr(), PerFrameDataSize);

		OpenGL.UnMapBufferMemory(m_PerFrameUniformBufferHandle);
		
		if (PerFrameDataSize > 0)
			OpenGL.BindBufferRange(m_PerFrameUniformBufferHandle, 0, 0, PerFrameDataSize);

		UploadMaterialData();

		OpenGL.SetClearColour(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		OpenGL.Clear(static_cast<uint32_t>(ClearBufferBit::ColourBuffer));

		for (uint32_t i = 0; i < m_NumMeshesToDraw; i++)
		{
			int64_t MeshID = m_MeshesToDraw[i].first;
			int64_t MaterialID = m_MeshesToDraw[i].second;

			MeshDataPtr CurrentMesh = m_Meshes[MeshID];
			Material& CurrentMaterial = m_Materials[MaterialID].first;

			int64_t CurrentGraphicsPipeline = CurrentMaterial.GetGraphicsPipelineHandle();
			OpenGL.BindGraphicsPipeline(CurrentGraphicsPipeline);

			size_t Offset = 0;
			OpenGL.BindVertexBuffers(0, 1, &m_VertexBufferHandle, &Offset);
			OpenGL.BindVertexBuffers(1, 1, &m_ModelMatrixBufferHandle, &Offset);
			OpenGL.BindElementBuffer(m_ElementBufferHandle, ElementBufferFormat::UInt32);

			OpenGL.BindBufferRange(m_PerMaterialUniformBufferHandle, 1, m_Materials[MaterialID].second, CurrentMaterial.GetShaderData()->GetShaderDataSize());

			for (const MaterialTextureSlot& Texture : CurrentMaterial.GetTextures())
				OpenGL.BindTexture(Texture.Slot.TextureSlotLocation, Texture.BoundTexture);

			uint32_t FirstElement = CurrentMesh.ElementDataSize / sizeof(uint32_t);
			uint32_t FirstVertex = CurrentMesh.VertesDataOffset / OpenGL.GetGraphicsPipelineVertexBufferStrides(CurrentGraphicsPipeline, 0);

			OpenGL.DrawElements(FirstElement, 1, CurrentMesh.ElementDataOffset, FirstVertex, i);
		}

		m_NumMeshesToDraw = 0;

		OpenGL.PresentFrame();
		OpenGL.DestroyPendingResources();
	}

	void RenderSubSystem::UploadMaterialData()
	{
		size_t Alignment = OpenGL.GetUniformBufferAlignment();

		size_t Offset = 0;

		uint8_t* MaterialDataBufferMemory = static_cast<uint8_t*>(OpenGL.MapBufferMemory(m_PerMaterialUniformBufferHandle, false));
		
		for (std::pair<Material,size_t>& Mat : m_Materials)
		{
			ShaderData* Data = Mat.first.GetShaderData();
			
			if (Data)
			{
				Mat.second = Offset;
				memcpy(MaterialDataBufferMemory + Offset, Data->GetShaderDataPtr(), Data->GetShaderDataSize());

				Offset = Util::AlignOffset(Offset + Data->GetShaderDataSize(), Alignment);
			}
		}

		OpenGL.UnMapBufferMemory(m_PerMaterialUniformBufferHandle);
	}
}*/