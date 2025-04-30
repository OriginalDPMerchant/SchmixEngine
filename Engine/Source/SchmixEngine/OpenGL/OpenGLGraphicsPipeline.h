#pragma once
#include <SchmixEngine/Core/Graphics/Core/GraphicsPipeline.h>
#include <SchmixEngine/OpenGL/OpenGLTexture.h>
#include <SchmixEngine/Core/Debugging/Debug.h>
#include <SchmixEngine/ThirdParty/GLAD/include/glad/glad.h>
#include <unordered_map>

namespace SchmixEngine
{
	class OpenGLGraphicsPipeline final
	{
	public:
		OpenGLGraphicsPipeline();

		OpenGLGraphicsPipeline(const OpenGLGraphicsPipeline&) = delete;
		OpenGLGraphicsPipeline& operator=(const OpenGLGraphicsPipeline&) = delete;

		OpenGLGraphicsPipeline(OpenGLGraphicsPipeline&& Other);
		OpenGLGraphicsPipeline& operator=(OpenGLGraphicsPipeline&& Other);

		~OpenGLGraphicsPipeline();

		void Init(const GraphicsPipelineCreateInfo& PipelineCreateInfo);
		void Destroy();

		void BindPipeline(GraphicsPipelineState& CurrentPipelineState);
		
		size_t GetBufferStride(uint32_t BindingPoint);

		bool IsInitialized();

		GLuint GetVertexArrayObject();
		GLuint GetShaderProgram();

		GraphicsPipelineState GetGraphicsPipelineState();

		std::vector<TextureSlot>& GetTextureSlots();

		std::vector<std::pair<std::string, ShaderDataType::Enum>>& GetShaderParameters();

	private:
		GLuint CreateShader(ShaderStageCreateInfo* pShaderStage);

		GLenum ShaderStageToGLenum(ShaderStage::Enum Stage);
		GLenum GetBaseTypeForShaderDataType(ShaderDataType::Enum Type);

		GLenum BlendFactorToGLenum(BlendFactor::Enum BlendFactor);

		GLenum BlendOperationToGLenum(BlendOperation::Enum BlendOp);

		GLuint m_VertexArrayObject;
		GLuint m_ShaderProgram;

		bool m_bIsInitialized;
		GraphicsPipelineState m_TargetPipelineState;

		std::unordered_map<uint32_t, size_t> m_BufferStrides;

		std::vector<TextureSlot> m_TextureSlots;

		std::vector<std::pair<std::string, ShaderDataType::Enum>> m_ShaderParameters;
	};
}