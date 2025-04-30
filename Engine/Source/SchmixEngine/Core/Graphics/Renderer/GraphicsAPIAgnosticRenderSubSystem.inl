#include <SchmixEngine/Core/Graphics/Renderer/GraphicsAPIAgnosticRenderSubSystem.h>

namespace SchmixEngine
{
	template<typename T>
	inline GraphicsAPIAgnosticRenderSubSystem<T>::GraphicsAPIAgnosticRenderSubSystem(const std::string& SubSystemName)
		: RenderSubSystem(SubSystemName)
	{
	}

	template<typename T>
	inline void GraphicsAPIAgnosticRenderSubSystem<T>::SetInitInfo(const GraphicsAPIInitInfo& Info)
	{
		m_GraphicsAPIInitiInfo = Info;
	}

	template<typename T>
	inline int64_t GraphicsAPIAgnosticRenderSubSystem<T>::CreateGraphicsPipeline(const GraphicsPipelineCreateInfo& CreateInfo)
	{
		return m_GraphicsAPI.CreateGraphicsPipeline(CreateInfo);
	}

	template<typename T>
	inline void GraphicsAPIAgnosticRenderSubSystem<T>::DestroyGraphicsPipeline(int64_t PipelineID)
	{
		m_GraphicsAPI.QueueDestroyGraphicsPipeline(PipelineID);
	}

	template<typename T>
	inline bool GraphicsAPIAgnosticRenderSubSystem<T>::IsGraphicsPipelineValid(int64_t PipelineID)
	{
		return m_GraphicsAPI.IsGraphicsPipelineHandleValid(PipelineID);
	}

	template<typename T>
	inline int64_t GraphicsAPIAgnosticRenderSubSystem<T>::CreateTexture(TextureInfo& Info, void* Data)
	{
		return m_GraphicsAPI.CreateTexture(Info, Data);
	}

	template<typename T>
	inline void GraphicsAPIAgnosticRenderSubSystem<T>::DestroyTexture(int64_t TextureID)
	{
		m_GraphicsAPI.QueueDestroyTexture(TextureID);
	}

	template<typename T>
	inline bool GraphicsAPIAgnosticRenderSubSystem<T>::IsTextureValid(int64_t TextureID)
	{
		return m_GraphicsAPI.IsTextureHandleValid(TextureID);
	}

	template<typename T>
	inline bool GraphicsAPIAgnosticRenderSubSystem<T>::InitSubSystem()
	{
		return m_GraphicsAPI.InitGraphicsAPI(m_GraphicsAPIInitiInfo);
	}

	template<typename T>
	inline bool GraphicsAPIAgnosticRenderSubSystem<T>::ShutdownSubSystem()
	{
		m_GraphicsAPI.ShutdownGraphicsAPI();

		return true;
	}

	template<typename T>
	inline void GraphicsAPIAgnosticRenderSubSystem<T>::UpdateSubSystem(float DeltaTime)
	{
		m_GraphicsAPI.DestroyPendingResources();
	}
}