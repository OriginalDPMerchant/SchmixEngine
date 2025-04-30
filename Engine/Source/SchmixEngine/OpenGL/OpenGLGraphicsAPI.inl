#include <SchmixEngine/OpenGL/OpenGLGraphicsAPI.h>

namespace SchmixEngine
{
	template<typename GraphicsAPIFlavour>
	inline OpenGLGraphicsAPI<GraphicsAPIFlavour>::OpenGLGraphicsAPI()
		: m_UniformBufferAlignment(0),
		m_ElementBufferFormat(ElementBufferFormat::Unknown),
		m_CurrentGraphicsPipeline(-1)
	{
	}

	template<typename GraphicsAPIFlavour>
	inline bool OpenGLGraphicsAPI<GraphicsAPIFlavour>::InitGraphicsAPI(const GraphicsAPIInitInfo& Info)
	{
		bool Success = m_GraphicsAPIFlavour.InitOpenGL(Info);
		Success = Success ? m_GraphicsAPIFlavour.InitWindow(Info) : false;

		if (Success)
		{
			if (Info.bDebugEnabled)
			{
				glEnable(GL_DEBUG_OUTPUT);
				glDebugMessageCallback(DebugMessageCallback, nullptr);
			}

			GLint Alignment = 0;
			glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &Alignment);
			m_UniformBufferAlignment = Alignment;
		}

		return Success;
	}

	template<typename GraphicsAPIFlavour>
	inline bool OpenGLGraphicsAPI<GraphicsAPIFlavour>::ShutdownGraphicsAPI()
	{
		m_ElementBufferFormat = ElementBufferFormat::Unknown;

		m_GraphicsPipelines.clear();
		m_FreeGraphicsPipelineHandles.clear();
		m_CurrentGraphicsPipeline = -1;

		m_Textures.clear();
		m_FreeTextureHandles.clear();

		m_Buffers.clear();
		m_FreeBufferHandles.clear();

		m_DeletionQueue.clear();

		bool Success = m_GraphicsAPIFlavour.ShutdownWindow();
		Success &= m_GraphicsAPIFlavour.ShutdownOpenGL();

		return Success;
	}

	template<typename GraphicsAPIFlavour>
	inline size_t OpenGLGraphicsAPI<GraphicsAPIFlavour>::GetUniformBufferAlignment()
	{
		return m_UniformBufferAlignment;
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::PresentFrame()
	{
		m_GraphicsAPIFlavour.PresentFrame();
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::SetClearColour(glm::vec4 ClearColour)
	{
		glClearColor(ClearColour.r, ClearColour.g, ClearColour.b, ClearColour.a);
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::Clear(uint32_t ClearBits)
	{
		GLbitfield Flags = 0;

		if (ClearBits & static_cast<uint32_t>(ClearBufferBit::ColourBuffer))
			Flags |= GL_COLOR_BUFFER_BIT;

		if (ClearBits & static_cast<uint32_t>(ClearBufferBit::DepthBuffer))
			Flags |= GL_DEPTH_BUFFER_BIT;

		glClear(Flags);
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::DrawArrays(uint32_t VertexCount, uint32_t InstanceCount, uint32_t FirstVertex, uint32_t FirstInstance)
	{
		if (IsGraphicsPipelineHandleValid(m_CurrentGraphicsPipeline))
		{
			GraphicsPipelineState CurrentGraphicsPipleineState = m_GraphicsPipelines[m_CurrentGraphicsPipeline].GetGraphicsPipelineState();

			GLenum PrimitiveType = PrimitiveAssemblyTypeToGLenum(CurrentGraphicsPipleineState.PrimitiveAssemblyInfo.PrimitiveType);
			glDrawArraysInstancedBaseInstance(PrimitiveType, FirstVertex, VertexCount, InstanceCount, FirstInstance);
		}
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::DrawElements(uint32_t IndexCount, uint32_t InstanceCount, uint32_t FirstIndex, uint32_t FirstVertex, uint32_t FirstInstance)
	{
		if (IsGraphicsPipelineHandleValid(m_CurrentGraphicsPipeline))
		{
			GraphicsPipelineState CurrentGraphicsPipleineState = m_GraphicsPipelines[m_CurrentGraphicsPipeline].GetGraphicsPipelineState();

			GLenum ElementBufferFormat = ElementBufferFormatToGLenum(m_ElementBufferFormat);
			GLenum PrimitiveType = PrimitiveAssemblyTypeToGLenum(CurrentGraphicsPipleineState.PrimitiveAssemblyInfo.PrimitiveType);
			glDrawElementsInstancedBaseVertexBaseInstance(PrimitiveType, IndexCount, ElementBufferFormat, (void*)FirstIndex, InstanceCount, FirstVertex, FirstInstance);
		}
	}

	template<typename GraphicsAPIFlavour>
	inline bool OpenGLGraphicsAPI<GraphicsAPIFlavour>::IsBufferHandleValid(int64_t BufferHandle)
	{
		return BufferHandle > -1 && BufferHandle < m_Buffers.size() ? m_Buffers[BufferHandle].IsInitialized() : false;
	}

	template<typename GraphicsAPIFlavour>
	inline int64_t OpenGLGraphicsAPI<GraphicsAPIFlavour>::CreateBuffer(const void* Data, size_t Size, BufferType Type, bool bIsPersistentlyMapped)
	{
		int64_t BufferHandle = -1;

		if (m_FreeBufferHandles.size() > 0)
		{
			BufferHandle = m_FreeBufferHandles[m_FreeBufferHandles.size() - 1];
			m_FreeBufferHandles.pop_back();
		}
		else
		{
			BufferHandle = m_Buffers.size();
			m_Buffers.push_back(std::move(OpenGLBuffer()));
		}

		m_Buffers[BufferHandle].Init(Data, Size, Type, bIsPersistentlyMapped);

		return BufferHandle;
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::QueueDestroyBuffer(int64_t BufferHandle)
	{
		m_DeletionQueue.push_back({ DeletionQueueElement::DeletionType::Buffer, BufferHandle });
	}

	template<typename GraphicsAPIFlavour>
	inline void* OpenGLGraphicsAPI<GraphicsAPIFlavour>::MapBufferMemory(int64_t BufferHandle, bool bInvalidateBuffer)
	{
		if (IsBufferHandleValid(BufferHandle))
			return m_Buffers[BufferHandle].MapMemory(bInvalidateBuffer);
		else
			return nullptr;
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::UnMapBufferMemory(int64_t BufferHandle)
	{
		if (IsBufferHandleValid(BufferHandle))
			m_Buffers[BufferHandle].UnMapMemory();
	}

	template<typename GraphicsAPIFlavour>
	inline size_t OpenGLGraphicsAPI<GraphicsAPIFlavour>::GetBufferSize(int64_t BufferHandle)
	{
		if (IsBufferHandleValid(BufferHandle))
			return m_Buffers[BufferHandle].GetBufferSize();
		else
			return 0;
	}

	template<typename GraphicsAPIFlavour>
	inline BufferType OpenGLGraphicsAPI<GraphicsAPIFlavour>::GetBufferType(int64_t BufferHandle)
	{
		if (IsBufferHandleValid(BufferHandle))
			return m_Buffers[BufferHandle].GetBufferType();
		else
			return BufferType::Unknown;
	}

	template<typename GraphicsAPIFlavour>
	inline bool OpenGLGraphicsAPI<GraphicsAPIFlavour>::IsBufferPersistentlyMapped(int64_t BufferHandle)
	{
		if (IsBufferHandleValid(BufferHandle))
			return m_Buffers[BufferHandle].IsBufferPersistentlyMapped();
		else
			return false;
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::BindVertexBuffers(uint32_t FirstBinding, uint32_t BindingCount, const int64_t* pBufferHandles, const size_t* pOffsets)
	{
		if (IsGraphicsPipelineHandleValid(m_CurrentGraphicsPipeline))
		{
			GLuint VertexArrayObject = m_GraphicsPipelines[m_CurrentGraphicsPipeline].GetVertexArrayObject();

			if (pBufferHandles)
			{
				std::vector<GLuint> BufferIDs(BindingCount);
				std::vector<GLsizei> BufferStrides(BindingCount);
				std::vector<GLintptr> BufferOffsets(BindingCount);

				for (uint32_t i = 0; i < BindingCount; i++)
				{
					BufferIDs[i] = IsBufferHandleValid(pBufferHandles[i]) ? m_Buffers[pBufferHandles[i]].GetBufferID() : 0;
					BufferStrides[i] = m_GraphicsPipelines[m_CurrentGraphicsPipeline].GetBufferStride(FirstBinding + i);
					BufferOffsets[i] = pOffsets ? pOffsets[i] : 0;
				}

				glVertexArrayVertexBuffers(VertexArrayObject, FirstBinding, BindingCount, BufferIDs.data(), BufferOffsets.data(), BufferStrides.data());
			}
			else
			{
				glVertexArrayVertexBuffers(VertexArrayObject, FirstBinding, BindingCount, nullptr, nullptr, nullptr);
			}
		}
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::BindElementBuffer(int64_t BufferHandle, ElementBufferFormat Format)
	{
		if (IsGraphicsPipelineHandleValid(m_CurrentGraphicsPipeline))
		{
			m_ElementBufferFormat = Format;

			GLuint VertexArrayObject = m_GraphicsPipelines[m_CurrentGraphicsPipeline].GetVertexArrayObject();
			GLuint BufferID = IsBufferHandleValid(BufferHandle) ? m_Buffers[BufferHandle].GetBufferID() : 0;

			glVertexArrayElementBuffer(VertexArrayObject, BufferID);
		}
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::BindBufferRange(int64_t BufferHandle, uint32_t Index, size_t Offset, size_t Size)
	{
		if (IsBufferHandleValid(BufferHandle))
		{
			GLenum GLBufferType = 0;

			switch (m_Buffers[BufferHandle].GetBufferType())
			{
			case BufferType::UniformBuffer:
				GLBufferType = GL_UNIFORM_BUFFER;
				break;
			case BufferType::ShaderStorageBuffer:
				GLBufferType = GL_SHADER_STORAGE_BUFFER;
				break;
			}

			glBindBufferRange(GLBufferType, Index, m_Buffers[BufferHandle].GetBufferID(), Offset, Size);
		}
	}

	template<typename GraphicsAPIFlavour>
	inline bool OpenGLGraphicsAPI<GraphicsAPIFlavour>::IsTextureHandleValid(int64_t TextureHandle)
	{
		return TextureHandle > -1 && TextureHandle < m_Textures.size() ? m_Textures[TextureHandle].IsInitialized() : false;
	}

	template<typename GraphicsAPIFlavour>
	inline int64_t OpenGLGraphicsAPI<GraphicsAPIFlavour>::CreateTexture(TextureInfo& CreationInfo, void* Data)
	{
		int64_t TextureHandle;

		if (m_FreeTextureHandles.size() > 0)
		{
			TextureHandle = m_FreeTextureHandles[m_FreeTextureHandles.size() - 1];
			m_FreeTextureHandles.pop_back();
		}
		else
		{
			TextureHandle = m_Textures.size();
			m_Textures.push_back(std::move(OpenGLTexture()));
		}

		m_Textures[TextureHandle].Init(CreationInfo, Data);

		return TextureHandle;
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::QueueDestroyTexture(int64_t TextureHandle)
	{
		m_DeletionQueue.push_back({ DeletionQueueElement::DeletionType::Texture, TextureHandle });
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::UploadTextureData(int64_t TextureHandle, void* Data, TextureFormat Format, uint32_t MipMapLevels, uint32_t Width, uint32_t Height, uint32_t Depth)
	{
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::SetTextureFilterMode(int64_t TextureHandle, TextureFilteringMode Mode)
	{
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::SetTextureWrappingMode(int64_t TextureHandle, TextureWrappingMode Mode)
	{
	}

	template<typename GraphicsAPIFlavour>
	inline TextureInfo OpenGLGraphicsAPI<GraphicsAPIFlavour>::GetTextureInfo(int64_t TextureHandle)
	{
		return TextureInfo();
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::BindTexture(uint32_t TextureSlot, int64_t TextureHandle)
	{
		if (IsTextureHandleValid(TextureHandle))
			glBindTextureUnit(TextureSlot, m_Textures[TextureHandle].GetTextureID());
	}

	template<typename GraphicsAPIFlavour>
	inline bool OpenGLGraphicsAPI<GraphicsAPIFlavour>::IsGraphicsPipelineHandleValid(int64_t GraphicsPipelineHandle)
	{
		return GraphicsPipelineHandle > -1 && GraphicsPipelineHandle < m_GraphicsPipelines.size() ? m_GraphicsPipelines[GraphicsPipelineHandle].IsInitialized() : false;
	}

	template<typename GraphicsAPIFlavour>
	inline int64_t OpenGLGraphicsAPI<GraphicsAPIFlavour>::CreateGraphicsPipeline(const GraphicsPipelineCreateInfo& CreationInfo)
	{
		int64_t GraphicsPipelineHandle;

		if (m_FreeGraphicsPipelineHandles.size() > 0)
		{
			GraphicsPipelineHandle = m_FreeGraphicsPipelineHandles[m_FreeGraphicsPipelineHandles.size() - 1];
			m_FreeGraphicsPipelineHandles.pop_back();
		}
		else
		{
			GraphicsPipelineHandle = m_GraphicsPipelines.size();
			m_GraphicsPipelines.push_back(std::move(OpenGLGraphicsPipeline()));
		}

		m_GraphicsPipelines[GraphicsPipelineHandle].Init(CreationInfo);

		return GraphicsPipelineHandle;
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::QueueDestroyGraphicsPipeline(int64_t GraphicsPipelineHandle)
	{
		m_DeletionQueue.push_back({ DeletionQueueElement::DeletionType::GraphicsPipeline, GraphicsPipelineHandle });
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::BindGraphicsPipeline(int64_t GraphicsPipelineHandle)
	{
		if (IsGraphicsPipelineHandleValid(GraphicsPipelineHandle))
		{
			GraphicsPipelineState OldPipelineState{};

			if (IsGraphicsPipelineHandleValid(m_CurrentGraphicsPipeline))
				m_GraphicsPipelines[m_CurrentGraphicsPipeline].GetGraphicsPipelineState();

			m_GraphicsPipelines[GraphicsPipelineHandle].BindPipeline(OldPipelineState);

			m_CurrentGraphicsPipeline = GraphicsPipelineHandle;
		}
	}

	template<typename GraphicsAPIFlavour>
	inline std::vector<std::pair<std::string, ShaderDataType::Enum>> OpenGLGraphicsAPI<GraphicsAPIFlavour>::GetGraphicsPipelineShaderPatameters(int64_t GraphicsPipelineHandle)
	{
		if (IsGraphicsPipelineHandleValid(GraphicsPipelineHandle))
			return m_GraphicsPipelines[GraphicsPipelineHandle].GetShaderParameters();
		else
			return std::vector<std::pair<std::string, ShaderDataType::Enum>>();
	}

	template<typename GraphicsAPIFlavour>
	inline std::vector<TextureSlot> OpenGLGraphicsAPI<GraphicsAPIFlavour>::GetGraphicsPipelineTextureSlots(int64_t GraphicsPipelineHandle)
	{
		if(IsGraphicsPipelineHandleValid(GraphicsPipelineHandle))
			return m_GraphicsPipelines[GraphicsPipelineHandle].GetTextureSlots();
		else
		return std::vector<TextureSlot>();
	}

	template<typename GraphicsAPIFlavour>
	inline size_t OpenGLGraphicsAPI<GraphicsAPIFlavour>::GetGraphicsPipelineVertexBufferStrides(int64_t GraphicsPipelineHandle, uint32_t BindingPoint)
	{
		if (IsGraphicsPipelineHandleValid(GraphicsPipelineHandle))
			return m_GraphicsPipelines[GraphicsPipelineHandle].GetBufferStride(BindingPoint);
		else
			return 0;
	}

	template<typename GraphicsAPIFlavour>
	inline GraphicsPipelineState OpenGLGraphicsAPI<GraphicsAPIFlavour>::GetGraphicsPipelineState(int64_t GraphicsPipelineHandle)
	{
		if (IsGraphicsPipelineHandleValid(GraphicsPipelineHandle))
			return m_GraphicsPipelines[GraphicsPipelineHandle].GetGraphicsPipelineState();
		else
			return GraphicsPipelineState{};
	}

	template<typename GraphicsAPIFlavour>
	inline ShaderData* OpenGLGraphicsAPI<GraphicsAPIFlavour>::CreateShaderData(std::vector<std::pair<std::string, ShaderDataType::Enum>> ShaderParameters)
	{
		return new STD140ShaderData(ShaderParameters);
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::DestroyPendingResources()
	{
		while (!m_DeletionQueue.empty())
		{
			switch (m_DeletionQueue.back().Type)
			{
			case DeletionQueueElement::Buffer:
				DestroyBuffer(m_DeletionQueue.back().Handle);
				break;
			case DeletionQueueElement::Texture:
				DestroyTexture(m_DeletionQueue.back().Handle);
				break;
			case DeletionQueueElement::GraphicsPipeline:
				DestroyGraphicsPipeline(m_DeletionQueue.back().Handle);
				break;
			}

			m_DeletionQueue.pop_back();
		}
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::DestroyBuffer(int64_t BufferHandle)
	{
		if (IsBufferHandleValid(BufferHandle))
		{
			m_FreeBufferHandles.push_back(BufferHandle);
			m_Buffers[BufferHandle].Destroy();
		}
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::DestroyTexture(int64_t TextureHandle)
	{
		if (IsTextureHandleValid(TextureHandle))
		{
			m_Textures[TextureHandle].Destroy();
			m_FreeTextureHandles.push_back(TextureHandle);
		}
	}

	template<typename GraphicsAPIFlavour>
	inline void OpenGLGraphicsAPI<GraphicsAPIFlavour>::DestroyGraphicsPipeline(int64_t GraphicsPipelineHandle)
	{
		if (IsGraphicsPipelineHandleValid(GraphicsPipelineHandle))
		{
			m_GraphicsPipelines[GraphicsPipelineHandle].Destroy();
			m_FreeGraphicsPipelineHandles.push_back(GraphicsPipelineHandle);

			if (GraphicsPipelineHandle == m_CurrentGraphicsPipeline)
				m_CurrentGraphicsPipeline = -1;
		}
	}

	template<typename GraphicsAPIFlavour>
	inline GLenum OpenGLGraphicsAPI<GraphicsAPIFlavour>::ElementBufferFormatToGLenum(ElementBufferFormat Format)
	{
		switch (Format)
		{
		case ElementBufferFormat::UInt16:
			return GL_UNSIGNED_SHORT;
		case ElementBufferFormat::UInt32:
			return GL_UNSIGNED_INT;
		default:
			return 0;
		}
	}

	template<typename GraphicsAPIFlavour>
	inline GLenum OpenGLGraphicsAPI<GraphicsAPIFlavour>::PrimitiveAssemblyTypeToGLenum(PrimitiveAssemblyType::Enum Type)
	{
		switch (Type)
		{
		case PrimitiveAssemblyType::Triangles:
			return GL_TRIANGLES;
		case PrimitiveAssemblyType::TriangleFan:
			return GL_TRIANGLE_FAN;
		case PrimitiveAssemblyType::TriangleStrip:
			return GL_TRIANGLE_STRIP;
		case PrimitiveAssemblyType::Line:
			return GL_LINE;
		case PrimitiveAssemblyType::LineStrip:
			return GL_LINE_STRIP;
		case PrimitiveAssemblyType::LineLoop:
			return GL_LINE_LOOP;
		default:
			return 0;
		}
	}

	template<typename GraphicsAPIFlavour>
	inline void GLAPIENTRY OpenGLGraphicsAPI<GraphicsAPIFlavour>::DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		std::string Msg = "OpenGL Debug Message (" + std::to_string(id) + "): " + message + "\n";
		LogLevel Level = LogLevel::Debug;

		switch (source)
		{
		case GL_DEBUG_SOURCE_API:
			Msg += "Source: API";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			Msg += "Source: Window System";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			Msg += "Source: Shader Compiler";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			Msg += "Source: Third Party";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			Msg += "Source: Application";
			break;
		case GL_DEBUG_SOURCE_OTHER:
			Msg += "Source: Other";
			break;
		}

		Msg += "\n";

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:
			Msg += "Type: Error";
			Level = LogLevel::Error;
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			Msg += "Type: Deprecated Behaviour";
			Level = LogLevel::Warning;
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			Msg += "Type: Undefined Behaviour";
			Level = LogLevel::Warning;
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			Msg += "Type: Portability";
			Level = LogLevel::Warning;
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			Msg += "Type: Performance";
			Level = LogLevel::Warning;
			break;
		case GL_DEBUG_TYPE_MARKER:
			Msg += "Type: Marker";
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			Msg += "Type: Push Group";
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			Msg += "Type: Pop Group";
			break;
		case GL_DEBUG_TYPE_OTHER:
			Msg += "Type: Other";
			break;
		}

		Msg += "\n";

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			Msg += "Severity: High";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			Msg += "Severity: Medium";
			break;
		case GL_DEBUG_SEVERITY_LOW:
			Msg += "Severity: Low";
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			Msg += "Severity: Notification";
			break;
		}

		SMX_DBG_LOG_WSRC(Msg, "OpenGL Debug", Level);
	}
}