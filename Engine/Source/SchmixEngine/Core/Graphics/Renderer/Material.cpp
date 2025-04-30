#include "Material.h"

namespace SchmixEngine
{
	Material::Material(int64_t GraphicsPipelineHandle, ShaderData* pMaterialData, std::vector<TextureSlot>& Textures)
		: m_GraphicsPipelineHandle(GraphicsPipelineHandle),
		m_pShaderData(pMaterialData)
	{
		for (TextureSlot& Texture : Textures)
			m_Textures.push_back({ Texture, -1 });
	}

	void Material::SetTexture(const std::string& TextureName, int64_t Texture)
	{
		auto Iterator = std::find_if(m_Textures.begin(), m_Textures.end(),
			[TextureName](const MaterialTextureSlot& CurrentTexture)
			{
				return TextureName == CurrentTexture.Slot.TextureSlotName;
			});

		if (Iterator != m_Textures.end())
			Iterator->BoundTexture = Texture;
	}

	int64_t Material::GetTexture(const std::string& TextureName)
	{
		auto Iterator = std::find_if(m_Textures.begin(), m_Textures.end(),
			[TextureName](const MaterialTextureSlot& CurrentTexture)
			{
				return TextureName == CurrentTexture.Slot.TextureSlotName;
			});

		return Iterator != m_Textures.end() ? Iterator->BoundTexture : -1;
	}

	const std::vector<MaterialTextureSlot>& Material::GetTextures()
	{
		return m_Textures;
	}

	ShaderData* Material::GetShaderData()
	{
		return m_pShaderData.get();
	
	}
	int64_t Material::GetGraphicsPipelineHandle()
	{
		return m_GraphicsPipelineHandle;
	}
}
