#pragma once
#include <SchmixEngine/Core/Graphics/Core/GraphicsAPI.h>
#include <SchmixEngine/Core/Graphics/Renderer/RenderSubSystem.h>

namespace SchmixEngine
{
	template<typename T>
	class GraphicsAPIAgnosticRenderSubSystem : public RenderSubSystem
	{
	public:
		static_assert(std::is_base_of_v<GraphicsAPI, T>, "Template Parameter T for SchmixEngine::GraphicsAPIAgnosticRenderSubSystem<T> must be a class deriving from \"SchmixEngine::GraphicsAPI\"");

		GraphicsAPIAgnosticRenderSubSystem(const std::string& SubSystemName);

		GraphicsAPIAgnosticRenderSubSystem(const GraphicsAPIAgnosticRenderSubSystem&) = delete;
		GraphicsAPIAgnosticRenderSubSystem& operator=(const GraphicsAPIAgnosticRenderSubSystem&) = delete;

		virtual ~GraphicsAPIAgnosticRenderSubSystem() = default;

		virtual void SetInitInfo(const GraphicsAPIInitInfo& Info) override;

		virtual int64_t CreateGraphicsPipeline(const GraphicsPipelineCreateInfo& CreateInfo) override;

		virtual void DestroyGraphicsPipeline(int64_t PipelineID) override;

		virtual bool IsGraphicsPipelineValid(int64_t PipelineID) override;

		virtual int64_t CreateTexture(TextureInfo& Info, void* Data) override;

		virtual void DestroyTexture(int64_t TextureID) override;

		virtual bool IsTextureValid(int64_t TextureID) override;

	protected:
		GraphicsAPIInitInfo m_GraphicsAPIInitiInfo;
		T m_GraphicsAPI;

		virtual bool InitSubSystem() override;
		virtual bool ShutdownSubSystem() override;

		virtual void UpdateSubSystem(float DeltaTime) override;
	};
}

#include <SchmixEngine/Core/Graphics/Renderer/GraphicsAPIAgnosticRenderSubSystem.inl>