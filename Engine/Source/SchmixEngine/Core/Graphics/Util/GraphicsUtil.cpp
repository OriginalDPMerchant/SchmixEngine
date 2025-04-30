#include "GraphicsUtil.h"

//todo: error msgs

namespace SchmixEngine
{
	GraphicsPipelineCreateInfo_2 GraphicsUtil::LoadGraphicsPipelineCreateInfo(const std::filesystem::path& FilePath, bool* pbSuccess)
	{
		std::string Msg;
		bool bSuccess = true;

		GraphicsPipelineCreateInfo_2 PipelineCreateInfo{};

		std::unique_ptr<VFSFile> File = VirtualFileSystem::GetGlobalVFS().LoadFile(FilePath, VFSFileMode::Read);

		if (File)
		{
			std::vector<char> FileData(File->GetSize());
			File->Read(FileData.data(), 0, FileData.size());

			rapidjson::Document JSONDocument;
			JSONDocument.Parse(FileData.data(), FileData.size());

			if (!JSONDocument.HasParseError())
			{
				if (JSONDocument.HasMember("ShaderStages"))
					bSuccess = bSuccess ? ReadShaderStages(JSONDocument["ShaderStages"], "ShaderStages", PipelineCreateInfo) : false;

				if (JSONDocument.HasMember("VertexInputInfo"))
					bSuccess = bSuccess ? ReadVertexInputInfo(JSONDocument["VertexInputInfo"], "VertexInputInfo", PipelineCreateInfo) : false;

				if (JSONDocument.HasMember("PrimitiveAssemblyInfo"))
					bSuccess = bSuccess ? ReadPrimitiveAssemblyInfo(JSONDocument["PrimitiveAssemblyInfo"], "PrimitiveAssemblyInfo", PipelineCreateInfo) : false;

				if (JSONDocument.HasMember("ColourBlendingInfo"))
					bSuccess = bSuccess ? ReadColourBlendingInfo(JSONDocument["ColourBlendingInfo"], "ColourBlendingInfo", PipelineCreateInfo) : false;

				if (JSONDocument.HasMember("TextureSlots"))
					bSuccess = bSuccess ? ReadTextureSlots(JSONDocument["TextureSlots"], "TextureSlots", PipelineCreateInfo) : false;

				if (JSONDocument.HasMember("ShaderParameters"))
					bSuccess = bSuccess ? ReadShaderParameters(JSONDocument["ShaderParameters"], "ShaderParameters", PipelineCreateInfo) : false;
			}
			else
			{
				Msg = "Error when Parsing JSON in \"" + FilePath.string() + "\"";
				SMX_LOG_WSRC(Msg, "SchmixEngine::GraphicsUtil::LoadGraphicsPipelineCreateInfo", SchmixEngine::LogLevel::Error);

				Msg = rapidjson::GetParseError_En(JSONDocument.GetParseError());
				SMX_DBG_LOG_WSRC(Msg, "rapidjson", SchmixEngine::LogLevel::Error);
			}
		}
		else
		{
			bSuccess = false;

			Msg = "Could not Open File \"" + FilePath.string() + "\"";

			SMX_LOG_WSRC(Msg, "SchmixEngine::GraphicsUtil::LoadGraphicsPipelineCreateInfo", LogLevel::Error);
		}

		if (pbSuccess)
			*pbSuccess = bSuccess;

		return PipelineCreateInfo;
	}

	void* GraphicsUtil::LoadTextureData(const std::filesystem::path& FilePath, TextureInfo& TexInfo)
	{
		std::string Msg;

		bool bSuccess = true;

		void* TextureData = nullptr;

		std::unique_ptr<VFSFile> File = VirtualFileSystem::GetGlobalVFS().LoadFile(FilePath, VFSFileMode::Read);

		if (File)
		{
			std::vector<char> FileData(File->GetSize());
			File->Read(FileData.data(), 0, FileData.size());

			rapidjson::Document JSONDocument;
			JSONDocument.Parse(FileData.data(), FileData.size());

			if (!JSONDocument.HasParseError())
			{
				TexInfo.Type = JSONUtil::ReadJSON_Enum<TextureType>("TextureType", JSONDocument, "rapidjson::Document", "TextureType", &bSuccess);

				if (!bSuccess)
					return nullptr;

				TexInfo.Format = JSONUtil::ReadJSON_Enum<TextureFormat>("TextureFormat", JSONDocument, "rapidjson::Document", "TextureFormat", &bSuccess);

				if (!bSuccess)
					return nullptr;

				TexInfo.FilterMode = JSONUtil::ReadJSON_Enum<TextureFilteringMode>("TextureFilteringMode", JSONDocument, "rapidjson::Document", "TextureFilterMode", &bSuccess);

				if (!bSuccess)
					return nullptr;

				TexInfo.WrappingMode = JSONUtil::ReadJSON_Enum<TextureWrappingMode>("TextureWrappingMode", JSONDocument, "rapidjson::Document", "TextureWrappingMode", &bSuccess);

				if (!bSuccess)
					return nullptr;

				TexInfo.MipMapLevels = JSONUtil::ReadJSON_Value<uint32_t>(JSONDocument, "rapidjson::Document", "TextureMipMapLevels", &bSuccess);

				if (!bSuccess)
					return nullptr;

				std::string TextureFilePath = JSONUtil::ReadJSON_Value<std::string>(JSONDocument, "rapidjson::Document", "TextureFile", &bSuccess);

				if (bSuccess)
					TextureData = LoadTextureFile(TextureFilePath, TexInfo);
			}
			else
			{
				Msg = "Error when Parsing JSON in \"" + FilePath.string() + "\"";
				SMX_LOG_WSRC(Msg, "SchmixEngine::GraphicsUtil::LoadTextureData", SchmixEngine::LogLevel::Error);

				Msg = rapidjson::GetParseError_En(JSONDocument.GetParseError());
				SMX_DBG_LOG_WSRC(Msg, "rapidjson", SchmixEngine::LogLevel::Error);
			}
		}
		else
		{
			Msg = "Could not Open File \"" + FilePath.string() + "\"";

			SMX_LOG_WSRC(Msg, "SchmixEngine::GraphicsUtil::LoadTextureData", LogLevel::Error);
		}

		return TextureData;
	}

	bool GraphicsUtil::ReadShaderStages(rapidjson::Value& JSONValue, const std::string& JSONValueName, GraphicsPipelineCreateInfo_2& CreateInfo)
	{
		bool bSuccess = true;

		if (JSONValue.IsArray())
		{
			CreateInfo.ShaderStages.resize(JSONValue.Size());

			for (size_t ShaderStageIndex = 0; ShaderStageIndex < JSONValue.Size(); ShaderStageIndex++)
			{
				if (JSONValue[ShaderStageIndex].IsObject())
				{
					ShaderStageCreateInfo_2& CurrentShaderStage = CreateInfo.ShaderStages[ShaderStageIndex];
					
					CurrentShaderStage.ShaderStageType = JSONUtil::ReadJSON_Enum<ShaderStage>("ShaderStage", JSONValue[ShaderStageIndex], JSONValueName, "ShaderStageType", &bSuccess);

					if (!bSuccess)
						break;

					std::string ShaderPath = JSONUtil::ReadJSON_Value<std::string>(JSONValue[ShaderStageIndex], JSONValueName, "ShaderCode", &bSuccess);

					if (!bSuccess)
						break;

					std::unique_ptr<VFSFile> ShaderFile = VirtualFileSystem::GetGlobalVFS().LoadFile(ShaderPath, VFSFileMode::Read);

					if (ShaderFile)
					{
						CurrentShaderStage.ShaderCode.resize(ShaderFile->GetSize());

						ShaderFile->Read(CurrentShaderStage.ShaderCode.data(), 0, CurrentShaderStage.ShaderCode.size());
					}
					else
					{
						std::string Msg = "The Shader File \"" + ShaderPath + "\" does not exist";
						SMX_LOG_WSRC(Msg, "SchmixEngine::GraphicsUtil::ReadShaderStages", LogLevel::Error);

						bSuccess = false;
					}
				}
			}
		}

		return bSuccess;
	}

	bool GraphicsUtil::ReadVertexInputInfo(rapidjson::Value& JSONValue, const std::string& JSONValueName, GraphicsPipelineCreateInfo_2& CreateInfo)
	{
		bool bSuccess = true;

		if (JSONValue.IsArray())
		{
			CreateInfo.VertexInputInfos.resize(JSONValue.Size());

			for (size_t VertexInputIndex = 0; VertexInputIndex < JSONValue.Size(); VertexInputIndex++)
			{
				if (JSONValue[VertexInputIndex].IsObject())
				{
					VertexInputInfo_2& CurrentVertexInputInfo = CreateInfo.VertexInputInfos[VertexInputIndex];

					CurrentVertexInputInfo.VertexBufferBindingIndex = JSONUtil::ReadJSON_Value<uint32_t>(JSONValue[VertexInputIndex], JSONValueName, "VertexBufferBindingIndex", &bSuccess);

					if (!bSuccess)
						break;

					CurrentVertexInputInfo.VertexInputRate = JSONUtil::ReadJSON_Enum<VertexInputRate>("VertexInputRate", JSONValue[VertexInputIndex], JSONValueName, "VertexInputRate", &bSuccess);

					if (!bSuccess)
						break;

					if (JSONValue[VertexInputIndex].HasMember("VertexBufferLayout"))
					{
						rapidjson::Value& VertexBufferLayout = JSONValue[VertexInputIndex]["VertexBufferLayout"];

						if (VertexBufferLayout.IsArray())
						{
							std::vector<ShaderDataType::Enum> Layout(VertexBufferLayout.Size(), ShaderDataType::Unknown);

							for (size_t ShaderDataTypeIndex = 0; ShaderDataTypeIndex < VertexBufferLayout.Size(); ShaderDataTypeIndex++)
							{
								if (VertexBufferLayout[ShaderDataTypeIndex].IsString())
									Layout[ShaderDataTypeIndex] = ShaderDataType::FromString(VertexBufferLayout[ShaderDataTypeIndex].GetString());

								if (Layout[ShaderDataTypeIndex] == ShaderDataType::Unknown)
								{
									std::string Msg = "Found Invalid ShaderDataType";
									SMX_LOG_WSRC(Msg, "SchmixEngine::GraphicsUtil::ReadVertexInputInfo", LogLevel::Error);

									bSuccess = false;
									break;
								}
							}

							CurrentVertexInputInfo.VertexBufferLayout.SetBufferLayout(Layout);
						}
					}
				}
			}
		}

		return bSuccess;
	}

	bool GraphicsUtil::ReadPrimitiveAssemblyInfo(rapidjson::Value& JSONValue, const std::string& JSONValueName, GraphicsPipelineCreateInfo_2& CreateInfo)
	{
		bool bSuccess = true;

		if (JSONValue.IsObject())
		{
			CreateInfo.PrimitiveAssemblyInfo.PrimitiveType = JSONUtil::ReadJSON_Enum<PrimitiveAssemblyType>("PrimitiveAssemblyType", JSONValue, JSONValueName, "PrimitiveType", &bSuccess);

			if (!bSuccess)
				return bSuccess;

			CreateInfo.PrimitiveAssemblyInfo.bPrimativeRestartEnabled = JSONUtil::ReadJSON_Value<bool>(JSONValue, JSONValueName, "PrimitiveRestartEnabled", &bSuccess);
		}

		return bSuccess;
	}

	bool GraphicsUtil::ReadColourBlendingInfo(rapidjson::Value& JSONValue, const std::string& JSONValueName, GraphicsPipelineCreateInfo_2& CreateInfo)
	{
		bool bSuccess = true;

		if (JSONValue.IsObject())
		{
			CreateInfo.ColourBlendingInfo.bBlendingEnabled = JSONUtil::ReadJSON_Value<bool>(JSONValue, JSONValueName, "BlendingEnabled", &bSuccess);

			if (!bSuccess)
				return bSuccess;

			CreateInfo.ColourBlendingInfo.SrcColourBlendFactor = JSONUtil::ReadJSON_Enum<BlendFactor>("BlendFactor", JSONValue, JSONValueName, "SrcColourBlendFactor", &bSuccess);

			if (!bSuccess)
				return bSuccess;

			CreateInfo.ColourBlendingInfo.DstColourBlendFactor = JSONUtil::ReadJSON_Enum<BlendFactor>("BlendFactor", JSONValue, JSONValueName, "DstColourBlendFactor", &bSuccess);

			if (!bSuccess)
				return bSuccess;

			CreateInfo.ColourBlendingInfo.ColourBlendOperation = JSONUtil::ReadJSON_Enum<BlendOperation>("BlendOperation", JSONValue, JSONValueName, "ColourBlendOperation", &bSuccess);

			if (!bSuccess)
				return bSuccess;

			CreateInfo.ColourBlendingInfo.SrcAlphaBlendFactor = JSONUtil::ReadJSON_Enum<BlendFactor>("BlendFactor", JSONValue, JSONValueName, "SrcAlphaBlendFactor", &bSuccess);

			if (!bSuccess)
				return bSuccess;

			CreateInfo.ColourBlendingInfo.DstAlphaBlendFactor = JSONUtil::ReadJSON_Enum<BlendFactor>("BlendFactor", JSONValue, JSONValueName, "DstAlphaBlendFactor", &bSuccess);

			if (!bSuccess)
				return bSuccess;

			CreateInfo.ColourBlendingInfo.AlphaBlendOperation = JSONUtil::ReadJSON_Enum<BlendOperation>("BlendOperation", JSONValue, JSONValueName, "AlphaBlendOperation", &bSuccess);

			if (!bSuccess)
				return bSuccess;

			CreateInfo.ColourBlendingInfo.ConstantColour = JSONUtil::ReadJSON_Vector<float, 4>(JSONValue, JSONValueName, "ConstantColour", &bSuccess);
		}

		return bSuccess;
	}

	bool GraphicsUtil::ReadTextureSlots(rapidjson::Value& JSONValue, const std::string& JSONValueName, GraphicsPipelineCreateInfo_2& CreateInfo)
	{
		bool bSuccess = true;

		if (JSONValue.IsArray())
		{
			CreateInfo.TextureSlots.resize(JSONValue.Size());

			for (size_t i = 0; i < JSONValue.Size(); i++)
			{
				if (JSONValue[i].IsObject())
				{
					CreateInfo.TextureSlots[i].TextureSlotName = JSONUtil::ReadJSON_Value<std::string>(JSONValue[i], JSONValueName, "TextureSlotName", &bSuccess);

					if (!bSuccess)
						return bSuccess;

					CreateInfo.TextureSlots[i].TextureSlotLocation = JSONUtil::ReadJSON_Value<int64_t>(JSONValue[i], JSONValueName, "TextureSlotLocation", &bSuccess);
				}
			}
		}

		return bSuccess;
	}

	bool GraphicsUtil::ReadShaderParameters(rapidjson::Value& JSONValue, const std::string& JSONValueName, GraphicsPipelineCreateInfo_2& CreateInfo)
	{
		bool bSuccess = true;

		if (JSONValue.IsArray())
		{
			CreateInfo.ShaderParameters.resize(JSONValue.Size());

			for (size_t i = 0; i < JSONValue.Size(); i++)
			{
				if (JSONValue[i].IsObject())
				{
					CreateInfo.ShaderParameters[i].first = JSONUtil::ReadJSON_Value<std::string>(JSONValue[i], JSONValueName, "ShaderParameterName", &bSuccess);

					if (!bSuccess)
						return bSuccess;

					CreateInfo.ShaderParameters[i].second = JSONUtil::ReadJSON_Enum<ShaderDataType>("ShaderParameterType", JSONValue[i], JSONValueName, "ShaderParameterType", &bSuccess);
				}
			}
		}

		return bSuccess;
	}

	void* GraphicsUtil::LoadTextureFile(const std::filesystem::path& FilePath, TextureInfo& TexInfo)
	{
		void* TextureData = nullptr;

		std::unique_ptr<VFSFile> File = VirtualFileSystem::GetGlobalVFS().LoadFile(FilePath, VFSFileMode::Read);

		if (File)
		{
			std::vector<char> FileData(File->GetSize());
			File->Read(FileData.data(), 0, FileData.size());

			if (TexInfo.Type == TextureType::Texture1D || TexInfo.Type == TextureType::Texture2D)
			{
				int Width = 0;
				int Height = 0;
				int NumChannels = 0; 
				int DesiredChannels = 0;

				DesiredChannels = static_cast<int>(TextureFormat::GetNumChannels(TexInfo.Format));

				switch (TextureFormat::GetBitsPerChannel(TexInfo.Format))
				{
				case 8:
					TextureData = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(FileData.data()), FileData.size(), &Width, &Height, &NumChannels, DesiredChannels);
					break;
				case 16:
					TextureData = stbi_load_16_from_memory(reinterpret_cast<const stbi_uc*>(FileData.data()), FileData.size(), &Width, &Height, &NumChannels, DesiredChannels);
					break;
				case 32:
					TextureData = stbi_loadf_from_memory(reinterpret_cast<const stbi_uc*>(FileData.data()), FileData.size(), &Width, &Height, &NumChannels, DesiredChannels);
					break;
				}
				
				TexInfo.Width = static_cast<uint32_t>(Width);
				TexInfo.Height = static_cast<uint32_t>(Height);
				TexInfo.Depth = 0;
			}
			else
			{
				TexInfo.Width = 0;
				TexInfo.Height = 0;
				TexInfo.Depth = 0;
				//Todo: other texture Types are currently unsupported
			}
		}
		else
		{
			std::string Msg = "Could not Open File \"" + FilePath.string() + "\"";

			SMX_LOG_WSRC(Msg, "SchmixEngine::GraphicsUtil::LoadTextureFile", LogLevel::Error);
		}

		return TextureData;
	}
}
