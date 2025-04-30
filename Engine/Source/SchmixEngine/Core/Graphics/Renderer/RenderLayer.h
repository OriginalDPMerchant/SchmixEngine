#pragma once
#include <SchmixEngine/Core/Graphics/Core/ShaderData.h>
#include <memory>

namespace SchmixEngine
{
	struct RenderLayerCreateInfo
	{
		uint32_t NumRenderLayerParameters;
		ShaderParameter* pRenderLayerParameters;
		int64_t RenderLayerTargetFrameBuffer;
	};

	class RenderLayer
	{
	public:
		RenderLayer(int64_t TargetFrameBuffer, ShaderData* pRenderLayerData);

		ShaderData* GetRenderLayerData();

		int64_t GetTargetFrameBuffer();

	private:
		std::unique_ptr<ShaderData> m_pRenderLayerData;

		int64_t m_TargetFrameBuffer;
	};
}