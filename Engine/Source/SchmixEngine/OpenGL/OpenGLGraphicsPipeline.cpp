#include "OpenGLGraphicsPipeline.h"

//Todo : use move semantics on vectors
namespace SchmixEngine
{
	OpenGLGraphicsPipeline::OpenGLGraphicsPipeline()
		: m_VertexArrayObject(0),
		m_ShaderProgram(0),
		m_bIsInitialized(false),
		m_TargetPipelineState(GraphicsPipelineState{})
	{
	}

	OpenGLGraphicsPipeline::OpenGLGraphicsPipeline(OpenGLGraphicsPipeline&& Other)
		: m_VertexArrayObject(Other.m_VertexArrayObject),
		m_ShaderProgram(Other.m_ShaderProgram),
		m_bIsInitialized(Other.m_bIsInitialized),
		m_TargetPipelineState(Other.m_TargetPipelineState),
		m_BufferStrides(Other.m_BufferStrides),
		m_TextureSlots(Other.m_TextureSlots),
		m_ShaderParameters(Other.m_ShaderParameters)
	{
		Other.m_VertexArrayObject = 0;
		Other.m_ShaderProgram = 0;
		Other.m_TargetPipelineState = GraphicsPipelineState{};
		Other.m_BufferStrides.clear();
		Other.m_TextureSlots.clear();
		Other.m_ShaderParameters.clear();
		Other.m_bIsInitialized = false;
	}

	OpenGLGraphicsPipeline& OpenGLGraphicsPipeline::operator=(OpenGLGraphicsPipeline&& Other)
	{
		if (this != &Other)
		{
			Destroy();

			m_VertexArrayObject = Other.m_VertexArrayObject;
			m_ShaderProgram = Other.m_ShaderProgram;
			m_TargetPipelineState = Other.m_TargetPipelineState;
			m_BufferStrides = Other.m_BufferStrides;
			m_TextureSlots = Other.m_TextureSlots;
			m_ShaderParameters = Other.m_ShaderParameters;
			m_bIsInitialized = Other.m_bIsInitialized;

			Other.m_VertexArrayObject = 0;
			Other.m_ShaderProgram = 0;
			Other.m_TargetPipelineState = GraphicsPipelineState{};
			Other.m_BufferStrides.clear();
			Other.m_TextureSlots.clear();
			Other.m_ShaderParameters.clear();
			Other.m_bIsInitialized = false;
		}

		return *this;
	}

	OpenGLGraphicsPipeline::~OpenGLGraphicsPipeline()
	{
		Destroy();
	}

	void OpenGLGraphicsPipeline::Init(const GraphicsPipelineCreateInfo& PipelineCreateInfo)
	{
		if (!m_bIsInitialized)
		{
			if (PipelineCreateInfo.pShaderStages)
			{
				m_ShaderProgram = glCreateProgram();

				std::vector<GLuint> Shaders(PipelineCreateInfo.NumShaderStages);

				for (size_t i = 0; i < Shaders.size(); i++)
				{
					Shaders[i] = CreateShader(PipelineCreateInfo.pShaderStages + i);
					glAttachShader(m_ShaderProgram, Shaders[i]);
				}

				glLinkProgram(m_ShaderProgram);

				for (size_t i = 0; i < Shaders.size(); i++)
					glDeleteShader(Shaders[i]);
			}

			if (PipelineCreateInfo.pVertexInputInfos)
			{
				glCreateVertexArrays(1, &m_VertexArrayObject);

				uint32_t AttribIndex = 0;

				for (size_t VertexInputInfoIndex = 0; VertexInputInfoIndex < PipelineCreateInfo.NumVertexInputInfos; VertexInputInfoIndex++)
				{
					VertexInputInfo& CurrentInfo = PipelineCreateInfo.pVertexInputInfos[VertexInputInfoIndex];
					uint32_t BindingIndex = CurrentInfo.VertexBufferBindingIndex;

					if (CurrentInfo.pVertexBufferLayout)
					{
						m_BufferStrides[BindingIndex] = CurrentInfo.pVertexBufferLayout->GetStride();

						for (const BufferLayoutElement& LayoutElement : CurrentInfo.pVertexBufferLayout->GetBufferLayoutElements())
						{
							const uint32_t Count = LayoutElement.Count;
							const size_t Offset = LayoutElement.Offset;
							GLenum Type = GetBaseTypeForShaderDataType(LayoutElement.Type);
							GLuint Divisor = CurrentInfo.VertexInputRate == VertexInputRate::PerInstance ? 1 : 0;

							uint32_t NumSubAttribIndices = 1;
							uint32_t OffsetPerSubAttribIndex = 0;

							switch (LayoutElement.Type)
							{
							case ShaderDataType::Mat2x2:
								NumSubAttribIndices = 2;
								OffsetPerSubAttribIndex = 4 * 2;
								break;
							case ShaderDataType::Mat3x3:
								NumSubAttribIndices = 3;
								OffsetPerSubAttribIndex = 4 * 3;
								break;
							case ShaderDataType::Mat4x4:
								NumSubAttribIndices = 4;
								OffsetPerSubAttribIndex = 4 * 4;
								break;
							}

							for (uint32_t SubAttribIndex = 0; SubAttribIndex < NumSubAttribIndices; SubAttribIndex++)
							{
								glEnableVertexArrayAttrib(m_VertexArrayObject, AttribIndex);
								glVertexArrayAttribFormat(m_VertexArrayObject, AttribIndex, Count, Type, GL_FALSE, Offset + (OffsetPerSubAttribIndex * SubAttribIndex));
								glVertexArrayAttribBinding(m_VertexArrayObject, AttribIndex, BindingIndex);
								glVertexArrayBindingDivisor(m_VertexArrayObject, BindingIndex, Divisor);
								AttribIndex++;
							}
						}
					}
				}
			}

			if (PipelineCreateInfo.pShaderParameters)
			{
				std::pair<std::string, ShaderDataType::Enum>* Begin = PipelineCreateInfo.pShaderParameters;
				std::pair<std::string, ShaderDataType::Enum>* End = PipelineCreateInfo.pShaderParameters + PipelineCreateInfo.NumShaderParameters;
				m_ShaderParameters = std::vector<std::pair<std::string, ShaderDataType::Enum>>(Begin, End);
			}

			if (PipelineCreateInfo.pTextureSlots)
			{
				const TextureSlot* pTextureSlots = PipelineCreateInfo.pTextureSlots;
				const uint32_t NumTextureSlots = PipelineCreateInfo.NumTextureSlots;

				m_TextureSlots.resize(NumTextureSlots);

				for (uint32_t i = 0; i < NumTextureSlots; i++)
				{
					const std::string& TextureSlotName = pTextureSlots[i].TextureSlotName;
					int64_t TextureSlotLocation = pTextureSlots[i].TextureSlotLocation;

					m_TextureSlots[i] = { TextureSlotName, TextureSlotLocation};
				}
			}

			m_TargetPipelineState.PrimitiveAssemblyInfo = PipelineCreateInfo.PrimitiveAssemblyInfo;
			m_TargetPipelineState.ColourBlendingInfo = PipelineCreateInfo.ColourBlendingInfo;

			m_bIsInitialized = m_VertexArrayObject != 0 && m_ShaderProgram != 0;

			if (!m_bIsInitialized)
				Destroy();
		}
	}

	void OpenGLGraphicsPipeline::Destroy()
	{
		if (m_VertexArrayObject != 0)
		{
			glDeleteVertexArrays(1, &m_VertexArrayObject);
			m_VertexArrayObject = 0;
		}

		if (m_ShaderProgram != 0)
		{
			glDeleteProgram(m_ShaderProgram);
			m_ShaderProgram = 0;
		}

		m_TargetPipelineState = GraphicsPipelineState{};
		m_BufferStrides.clear();
		m_TextureSlots.clear();
		m_ShaderParameters.clear();

		m_bIsInitialized = false;
	}

	void OpenGLGraphicsPipeline::BindPipeline(GraphicsPipelineState& CurrentPipelineState)
	{
		if (m_bIsInitialized)
		{
			if (m_TargetPipelineState.PrimitiveAssemblyInfo.bPrimativeRestartEnabled)
				glEnable(GL_PRIMITIVE_RESTART);
			else
				glDisable(GL_PRIMITIVE_RESTART);

			if (m_TargetPipelineState.ColourBlendingInfo.bBlendingEnabled)
			{
				GLenum SrcColourBlendFactor = BlendFactorToGLenum(m_TargetPipelineState.ColourBlendingInfo.SrcColourBlendFactor);
				GLenum DstColourBlendFactor = BlendFactorToGLenum(m_TargetPipelineState.ColourBlendingInfo.DstColourBlendFactor);
				GLenum ColourBlendOperation = BlendOperationToGLenum(m_TargetPipelineState.ColourBlendingInfo.ColourBlendOperation);
				GLenum SrcAlphaBlendFactor = BlendFactorToGLenum(m_TargetPipelineState.ColourBlendingInfo.SrcAlphaBlendFactor);
				GLenum DstAlphaBlendFactor = BlendFactorToGLenum(m_TargetPipelineState.ColourBlendingInfo.DstAlphaBlendFactor);
				GLenum AlphaBlendOperation = BlendOperationToGLenum(m_TargetPipelineState.ColourBlendingInfo.AlphaBlendOperation);
				glm::vec4 ConstantColour = m_TargetPipelineState.ColourBlendingInfo.ConstantColour;

				glEnable(GL_BLEND);

				glBlendColor(ConstantColour.r, ConstantColour.g, ConstantColour.b, ConstantColour.a);
				glBlendFuncSeparate(SrcColourBlendFactor, DstColourBlendFactor, SrcAlphaBlendFactor, DstAlphaBlendFactor);
				glBlendEquationSeparate(ColourBlendOperation, AlphaBlendOperation);
			}
			else
			{
				glDisable(GL_BLEND);
			}

			glBindVertexArray(m_VertexArrayObject);
			glUseProgram(m_ShaderProgram);
		}
	}

	size_t OpenGLGraphicsPipeline::GetBufferStride(uint32_t BindingPoint)
	{
		auto Iterator = m_BufferStrides.find(BindingPoint);

		return Iterator != m_BufferStrides.end() ? Iterator->second : 0;
	}

	bool OpenGLGraphicsPipeline::IsInitialized()
	{
		return m_bIsInitialized;
	}

	GLuint OpenGLGraphicsPipeline::GetVertexArrayObject()
	{
		return m_VertexArrayObject;
	}

	GLuint OpenGLGraphicsPipeline::GetShaderProgram()
	{
		return m_ShaderProgram;
	}

	GraphicsPipelineState OpenGLGraphicsPipeline::GetGraphicsPipelineState()
	{
		return m_TargetPipelineState;
	}

	std::vector<TextureSlot>& OpenGLGraphicsPipeline::GetTextureSlots()
	{
		return m_TextureSlots;
	}

	std::vector<std::pair<std::string, ShaderDataType::Enum>>& OpenGLGraphicsPipeline::GetShaderParameters()
	{
		return m_ShaderParameters;
	}

	GLuint OpenGLGraphicsPipeline::CreateShader(ShaderStageCreateInfo* pShaderStage)
	{
		GLuint Shader = glCreateShader(ShaderStageToGLenum(pShaderStage->ShaderStageType));
		glShaderSource(Shader, 1, &(pShaderStage->pShaderCode), nullptr);
		glCompileShader(Shader);

		int  Success;
		char InfoLog[512];
		glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);

		if (!Success)
		{
			glGetShaderInfoLog(Shader, 512, NULL, InfoLog);
			
			std::string Msg = "Failed to Compile " + ShaderStage::ToString(pShaderStage->ShaderStageType);

			SMX_DBG_LOG_WSRC(Msg, "Graphics Pipeline", LogLevel::Error);

			Msg = "Shader Compile Error:\n" + std::string(InfoLog);
			SMX_DBG_LOG_WSRC(Msg, "Graphics Pipeline", LogLevel::Warning);
		}

		return Shader;
	}

	GLenum OpenGLGraphicsPipeline::ShaderStageToGLenum(ShaderStage::Enum Stage)
	{
		switch (Stage)
		{
		case ShaderStage::VertexShaderStage:
			return GL_VERTEX_SHADER;
		case ShaderStage::FragmentShaderStage:
			return GL_FRAGMENT_SHADER;
		default:
			return 0;
		}
	}
	GLenum OpenGLGraphicsPipeline::GetBaseTypeForShaderDataType(ShaderDataType::Enum Type)
	{
		switch (Type)
		{
		case ShaderDataType::Bool:
		case ShaderDataType::Bool2:
		case ShaderDataType::Bool3:
		case ShaderDataType::Bool4:
			return GL_BOOL;
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
			return GL_INT;
		case ShaderDataType::UInt:
		case ShaderDataType::UInt2:
		case ShaderDataType::UInt3:
		case ShaderDataType::UInt4:
			return GL_UNSIGNED_INT;
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
		case ShaderDataType::Mat2x2:
		case ShaderDataType::Mat3x3:
		case ShaderDataType::Mat4x4:
			return GL_FLOAT;
		default:
			return 0;
		}
	}

	GLenum OpenGLGraphicsPipeline::BlendFactorToGLenum(BlendFactor::Enum BlendFactor)
	{
		switch (BlendFactor)
		{
		case BlendFactor::Zero:
			return GL_ZERO;
		case BlendFactor::One:
			return GL_ONE;
		case BlendFactor::SrcColour:
			return GL_SRC_COLOR;
		case BlendFactor::OneMinusSrcColour:
			return GL_ONE_MINUS_SRC_COLOR;
		case BlendFactor::DstColour:
			return GL_DST_COLOR;
		case BlendFactor::OneMinusDstColour:
			return GL_ONE_MINUS_DST_COLOR;
		case BlendFactor::SrcAlpha:
			return GL_SRC_ALPHA;
		case BlendFactor::OneMinusSrcAlpha:
			return GL_ONE_MINUS_SRC_ALPHA;
		case BlendFactor::DstAlpha:
			return GL_DST_ALPHA;
		case BlendFactor::OneMinusDstAlpha:
			return GL_ONE_MINUS_DST_COLOR;
		case BlendFactor::ConstantColour:
			return GL_CONSTANT_COLOR;
		case BlendFactor::OneMinusConstantColour:
			return GL_ONE_MINUS_CONSTANT_COLOR;
		case BlendFactor::ConstantAlpha:
			return GL_CONSTANT_ALPHA;
		case BlendFactor::OneMinusConstantAlpha:
			return GL_ONE_MINUS_CONSTANT_ALPHA;
		case BlendFactor::SrcAlphaSaturate:
			return GL_SRC_ALPHA_SATURATE;
		case BlendFactor::ReciprocalSrcColour:
			return GL_SRC1_COLOR;
		case BlendFactor::OneMinusReciprocalSrcColour:
			return GL_ONE_MINUS_SRC1_COLOR;
		case BlendFactor::ReciprocalSrcAlpha:
			return GL_SRC1_ALPHA;
		case BlendFactor::OneMinusReciprocalSrcAlpha:
			return GL_ONE_MINUS_SRC1_ALPHA;
		default:
			return 0;
		}
	}

	GLenum OpenGLGraphicsPipeline::BlendOperationToGLenum(BlendOperation::Enum BlendOp)
	{
		switch (BlendOp)
		{
		case BlendOperation::Add:
			return GL_FUNC_ADD;
		case BlendOperation::Subtract:
			return GL_FUNC_SUBTRACT;
		case BlendOperation::ReverseSubtract:
			return GL_FUNC_REVERSE_SUBTRACT;
		case BlendOperation::Min:
			return GL_MIN;
		case BlendOperation::Max:
			return GL_MAX;
		default:
			return 0;
		}
	}
}
