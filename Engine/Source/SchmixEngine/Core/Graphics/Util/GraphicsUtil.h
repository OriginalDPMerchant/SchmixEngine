#pragma once
#include <SchmixEngine/Core/Graphics/Core/GraphicsPipeline.h>
#include <SchmixEngine/Core/Graphics/Core/Texture.h>
#include <SchmixEngine/Core/FileSystem/VirtualFileSystem.h>
#include <SchmixEngine/Core/Util/JSONUtil.h>
#include <SchmixEngine/ThirdParty/stb_image.h>
#include <SchmixEngine/ThirdParty/rapidjson/rapidjson.h>
#include <SchmixEngine/ThirdParty/rapidjson/error/error.h>
#include <SchmixEngine/ThirdParty/rapidjson/error/en.h>

namespace SchmixEngine
{
	class GraphicsUtil
	{
	public:
		GraphicsUtil() = delete;
		GraphicsUtil(const GraphicsUtil&) = delete;
		GraphicsUtil& operator=(const GraphicsUtil&) = delete;

		static GraphicsPipelineCreateInfo_2 LoadGraphicsPipelineCreateInfo(const std::filesystem::path& FilePath, bool* pbSuccess);

		static void* LoadTextureData(const std::filesystem::path& FilePath, TextureInfo& TexInfo);

	private:
		static bool ReadShaderStages(rapidjson::Value& JSONValue, const std::string& JSONValueName, GraphicsPipelineCreateInfo_2& CreateInfo);

		static bool ReadVertexInputInfo(rapidjson::Value& JSONValue, const std::string& JSONValueName, GraphicsPipelineCreateInfo_2& CreateInfo);

		static bool ReadPrimitiveAssemblyInfo(rapidjson::Value& JSONValue, const std::string& JSONValueName, GraphicsPipelineCreateInfo_2& CreateInfo);

		static bool ReadColourBlendingInfo(rapidjson::Value& JSONValue, const std::string& JSONValueName, GraphicsPipelineCreateInfo_2& CreateInfo);

		static bool ReadTextureSlots(rapidjson::Value& JSONValue, const std::string& JSONValueName, GraphicsPipelineCreateInfo_2& CreateInfo);

		static bool ReadShaderParameters(rapidjson::Value& JSONValue, const std::string& JSONValueName, GraphicsPipelineCreateInfo_2& CreateInfo);

		static void* LoadTextureFile(const std::filesystem::path& FilePath, TextureInfo& TexInfo);
	};
}