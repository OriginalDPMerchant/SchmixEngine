#include <SchmixEngine/OpenGL/STD140ShaderData.h>
#include <SchmixEngine/Core/Graphics/Renderer/RenderSubSystem.h>
#include <iostream>
#include<array>
#include <SchmixEngine/ThirdParty/GLM/gtx/transform.hpp>

namespace SchmixEngine
{
	RenderSubSystem::RenderSubSystem(const std::string& SubSystemName)
		: SubSystem(SubSystemName)
	{

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
			TextureHandle = CreateTexture(TexInfo, TextureData);
			free(TextureData);
		}

		return TextureHandle;
	}
}