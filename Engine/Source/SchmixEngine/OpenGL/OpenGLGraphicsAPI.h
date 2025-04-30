#pragma once
#include <SchmixEngine/Core/Graphics/Core/GraphicsAPI.h>
#include <SchmixEngine/OpenGL/OpenGLGraphicsAPIFlavour.h>
#include <SchmixEngine/OpenGL/OpenGLBuffer.h>
#include <SchmixEngine/OpenGL/OpenGLGraphicsPipeline.h>
#include <SchmixEngine/OpenGL/STD140ShaderData.h>
#include <SchmixEngine/ThirdParty/GLM/glm.hpp> 
#include <unordered_set>
#include <queue>

//todo: make graphics resources reference counted
namespace SchmixEngine
{
	template<typename GraphicsAPIFlavour>
	class OpenGLGraphicsAPI : public GraphicsAPI
	{
	public:
		static_assert(std::is_base_of_v<OpenGLGraphicsAPIFlavour, GraphicsAPIFlavour>, "Template Argument \"GraphicsAPIFlavour\" must inherit from \"SchmixEngine::OpenGLGraphicsAPIFlavour\"");
		
		OpenGLGraphicsAPI();

		OpenGLGraphicsAPI(const OpenGLGraphicsAPI&) = delete;
		OpenGLGraphicsAPI& operator=(const OpenGLGraphicsAPI&) = delete;

		virtual ~OpenGLGraphicsAPI() = default;

		bool InitGraphicsAPI(const GraphicsAPIInitInfo& Info);
		bool ShutdownGraphicsAPI();

		size_t GetUniformBufferAlignment();

		void PresentFrame();

		void SetClearColour(glm::vec4 ClearColour);

		void Clear(uint32_t ClearBits);

		void DrawArrays(uint32_t VertexCount, uint32_t InstanceCount, uint32_t FirstVertex, uint32_t FirstInstance);
		void DrawElements(uint32_t IndexCount, uint32_t InstanceCount, uint32_t FirstIndex, uint32_t FirstVertex, uint32_t FirstInstance);

		//Functions for Buffers
		bool IsBufferHandleValid(int64_t BufferHandle);

		int64_t CreateBuffer(const void* Data, size_t Size, BufferType Type, bool bIsPersistentlyMapped);
		void QueueDestroyBuffer(int64_t BufferHandle);

		void* MapBufferMemory(int64_t BufferHandle, bool bInvalidateBuffer);
		void UnMapBufferMemory(int64_t BufferHandle);

		size_t GetBufferSize(int64_t BufferHandle);
		BufferType GetBufferType(int64_t BufferHandle);
		bool IsBufferPersistentlyMapped(int64_t BufferHandle);

		void BindVertexBuffers(uint32_t FirstBinding, uint32_t BindingCount, const int64_t* pBufferHandles, const size_t* pOffsets);
		void BindElementBuffer(int64_t BufferHandle, ElementBufferFormat Format);
		void BindBufferRange(int64_t BufferHandle, uint32_t Index, size_t Offset, size_t Size);

		//Functions for Textures
		bool IsTextureHandleValid(int64_t TextureHandle);

		int64_t CreateTexture(TextureInfo& CreationInfo, void* Data);
		void QueueDestroyTexture(int64_t TextureHandle);

		void UploadTextureData(int64_t TextureHandle, void* Data, TextureFormat Format, uint32_t MipMapLevels, uint32_t Width, uint32_t Height, uint32_t Depth);
		void SetTextureFilterMode(int64_t TextureHandle, TextureFilteringMode Mode);
		void SetTextureWrappingMode(int64_t TextureHandle, TextureWrappingMode Mode);
		TextureInfo GetTextureInfo(int64_t TextureHandle);

		void BindTexture(uint32_t TextureSlot, int64_t TextureHandle);

		//Functions for GraphicsPipelines
		bool IsGraphicsPipelineHandleValid(int64_t GraphicsPipelineHandle);

		int64_t CreateGraphicsPipeline(const GraphicsPipelineCreateInfo & CreationInfo);
		void QueueDestroyGraphicsPipeline(int64_t GraphicsPipelineHandle);

		void BindGraphicsPipeline(int64_t GraphicsPipelineHandle);

		std::vector<std::pair<std::string, ShaderDataType::Enum>> GetGraphicsPipelineShaderPatameters(int64_t GraphicsPipelineHandle);

		std::vector<TextureSlot> GetGraphicsPipelineTextureSlots(int64_t GraphicsPipelineHandle);

		size_t GetGraphicsPipelineVertexBufferStrides(int64_t GraphicsPipelineHandle, uint32_t BindingPoint);

		GraphicsPipelineState GetGraphicsPipelineState(int64_t GraphicsPipelineHandle);

		ShaderData* CreateShaderData(std::vector<std::pair<std::string, ShaderDataType::Enum>> ShaderParameters);

		void DestroyPendingResources();

	private:

		void DestroyBuffer(int64_t BufferHandle);
		void DestroyTexture(int64_t TextureHandle);
		void DestroyGraphicsPipeline(int64_t GraphicsPipelineHandle);

		GLenum ElementBufferFormatToGLenum(ElementBufferFormat Format);

		GLenum PrimitiveAssemblyTypeToGLenum(PrimitiveAssemblyType::Enum Type);

		static void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

		GraphicsAPIFlavour m_GraphicsAPIFlavour;

		size_t m_UniformBufferAlignment;

		std::vector<OpenGLBuffer> m_Buffers;
		std::vector<int64_t> m_FreeBufferHandles;

		std::vector<OpenGLTexture> m_Textures;
		std::vector<int64_t> m_FreeTextureHandles;

		std::vector<OpenGLGraphicsPipeline> m_GraphicsPipelines;
		std::vector<int64_t> m_FreeGraphicsPipelineHandles;

		std::vector<DeletionQueueElement> m_DeletionQueue;

		ElementBufferFormat m_ElementBufferFormat;
		int64_t m_CurrentGraphicsPipeline;
	};
}

#include <SchmixEngine/OpenGL/OpenGLGraphicsAPI.inl>