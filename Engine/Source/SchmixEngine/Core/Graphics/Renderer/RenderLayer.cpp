#include <SchmixEngine/Core/Graphics/Renderer/RenderLayer.h>

namespace SchmixEngine
{
	RenderLayer::RenderLayer(int64_t TargetFrameBuffer, ShaderData* pRenderLayerData)
		: m_TargetFrameBuffer(TargetFrameBuffer),
		m_pRenderLayerData(pRenderLayerData)
	{
		
	}

	ShaderData* RenderLayer::GetRenderLayerData()
	{
		return m_pRenderLayerData.get();
	}

	int64_t RenderLayer::GetTargetFrameBuffer()
	{
		return m_TargetFrameBuffer;
	}
}