#pragma once
#include <SchmixEngine/Core/Graphics/Core/ShaderData.h>
#include <SchmixEngine/Core/Graphics/Core/GraphicsPipeline.h>
#include <memory>
#include <unordered_map>
#include <string>
#include <algorithm>

namespace SchmixEngine
{
	struct MaterialTextureSlot
	{
		TextureSlot Slot;
		int64_t BoundTexture;
	};

	class Material
	{
	public:
		Material(int64_t GraphicsPipelineHandle, ShaderData* pMaterialData, std::vector<TextureSlot>& Textures);

		void SetTexture(const std::string& TextureName, int64_t Texture);
		int64_t GetTexture(const std::string& TextureName);

		const std::vector<MaterialTextureSlot>& GetTextures();

		ShaderData* GetShaderData();

		int64_t GetGraphicsPipelineHandle();

	private:
		std::vector<MaterialTextureSlot> m_Textures;
		std::unique_ptr<ShaderData> m_pShaderData;

		int64_t m_GraphicsPipelineHandle;
	};
}