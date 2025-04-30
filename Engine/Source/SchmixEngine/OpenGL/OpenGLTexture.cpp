#include <SchmixEngine/OpenGL/OpenGLTexture.h>

namespace SchmixEngine
{
	OpenGLTexture::OpenGLTexture()
		: m_TextureID(0),
		m_TextureInfo(TextureInfo{})
	{
	}

	OpenGLTexture::OpenGLTexture(OpenGLTexture&& Other)
		: m_TextureID(Other.m_TextureID),
		m_TextureInfo(Other.m_TextureInfo)
	{
		Other.m_TextureID = 0;
		Other.m_TextureInfo = TextureInfo{};
	}

	OpenGLTexture& OpenGLTexture::operator=(OpenGLTexture&& Other)
	{
		if (this != &Other)
		{
			Destroy();
			m_TextureID = Other.m_TextureID;
			m_TextureInfo = Other.m_TextureInfo;

			Other.m_TextureID = 0;
			Other.m_TextureInfo = TextureInfo{};
		}

		return *this;
	}

	OpenGLTexture::~OpenGLTexture()
	{
		Destroy();
	}

	void OpenGLTexture::Init(const TextureInfo& Info, void* Data)
	{
		if (!IsInitialized())
		{
			m_TextureInfo.Type = Info.Type;
			UploadData(Data, Info.Format, Info.MipMapLevels, Info.Width, Info.Height, Info.Depth);
			SetFilterMode(Info.FilterMode);
			SetWrappingMode(Info.WrappingMode);

			if (m_TextureID == 0)
				Destroy();
		}
	}

	void OpenGLTexture::Destroy()
	{
		if (IsInitialized())
			glDeleteTextures(1, &m_TextureID);

		m_TextureID = 0;
		m_TextureInfo = TextureInfo{};
	}

	void OpenGLTexture::UploadData(void* Data, TextureFormat::Enum Format, uint32_t MipMapLevels, uint32_t Width, uint32_t Height, uint32_t Depth)
	{
		bool RecreateTexture = m_TextureInfo.Format != Format
			|| m_TextureInfo.MipMapLevels != MipMapLevels
			|| m_TextureInfo.Width != Width
			|| m_TextureInfo.Height != Height
			|| m_TextureInfo.Depth != Depth;

		if (RecreateTexture)
		{
			m_TextureInfo.Format = Format;
			m_TextureInfo.MipMapLevels = MipMapLevels;
			m_TextureInfo.Width = Width;
			m_TextureInfo.Height = Height;
			m_TextureInfo.Depth = Depth;

			if (m_TextureID != 0)
				glDeleteTextures(1, &m_TextureID);

			CreateTexture();
		}

		UpdateTextureData(Data);
	}

	void OpenGLTexture::SetFilterMode(TextureFilteringMode::Enum Mode)
	{
		if (IsInitialized())
		{
			m_TextureInfo.FilterMode = Mode;

			GLenum GLFilteringMode = TextureFilteringModeToGLenum(Mode);

			glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GLFilteringMode);
			glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GLFilteringMode);
		}
	}

	void OpenGLTexture::SetWrappingMode(TextureWrappingMode::Enum Mode)
	{
		if (IsInitialized())
		{
			m_TextureInfo.WrappingMode = Mode;

			GLenum GLWrappingMode = TextureWrappingModeToGLenum(Mode);

			glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GLWrappingMode);
			glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GLWrappingMode);
		}
	}

	bool OpenGLTexture::IsInitialized()
	{
		return m_TextureID != 0;
	}

	TextureInfo OpenGLTexture::GetTextureInfo()
	{
		return m_TextureInfo;
	}

	GLuint OpenGLTexture::GetTextureID()
	{
		return m_TextureID;
	}

	void OpenGLTexture::CreateTexture()
	{
		//TODO: Add Support for ArrayTextures/Cubemaps
		GLenum GLTextureType = TextureTypeToGLenum(m_TextureInfo.Type);
		GLenum GLInternalTextureFormat = GetTextureFormatInternalFormat(m_TextureInfo.Format);

		glCreateTextures(GLTextureType, 1, &m_TextureID);

		switch (m_TextureInfo.Type)
		{
		case TextureType::Texture1D:
			glTextureStorage1D(m_TextureID, m_TextureInfo.MipMapLevels, GLInternalTextureFormat, m_TextureInfo.Width);
			break;
		case TextureType::Texture1D_Array:
			//glTextureStorage2D(m_TextureID, m_TextureInfo.MipMapLevels, GLInternalTextureFormat, m_TextureInfo.Width, m_TextureInfo.Height);
			break;
		case TextureType::Texture2D:
			glTextureStorage2D(m_TextureID, m_TextureInfo.MipMapLevels, GLInternalTextureFormat, m_TextureInfo.Width, m_TextureInfo.Height);
			break;
		case TextureType::Texture2D_Array:
			//glTextureStorage3D(m_TextureID, m_TextureInfo.MipMapLevels, GLInternalTextureFormat, m_TextureInfo.Width, m_TextureInfo.Height, m_TextureInfo.Depth);
			break;
		case TextureType::CubeMap:
			//glTextureStorage2D(m_TextureID, m_TextureInfo.MipMapLevels, GLInternalTextureFormat, m_TextureInfo.Width, m_TextureInfo.Height);
			break;
		case TextureType::CubeMap_Array:
			//glTextureStorage3D(m_TextureID, m_TextureInfo.MipMapLevels, GLInternalTextureFormat, m_TextureInfo.Width, m_TextureInfo.Height, m_TextureInfo.Depth * 6);
			break;
		case TextureType::Texture3D:
			glTextureStorage3D(m_TextureID, m_TextureInfo.MipMapLevels, GLInternalTextureFormat, m_TextureInfo.Width, m_TextureInfo.Height, m_TextureInfo.Depth);
			break;
		}
	}

	//todo:change this function to be map texture memory/unmap memory
	void OpenGLTexture::UpdateTextureData(void* Data)
	{
		//TODO: Add Support for ArrayTextures/Cubemaps
		GLenum GLBaseTextureFormat = GetTextureFormatBaseFormat(m_TextureInfo.Format);
		GLenum GLTextureDataType = GetTextureFormatDataType(m_TextureInfo.Format);

		switch (m_TextureInfo.Type)
		{
		case TextureType::Texture1D:
			glTextureSubImage1D(m_TextureID, 0, 0, m_TextureInfo.Width, GLBaseTextureFormat, GLTextureDataType, Data);
			break;
		case TextureType::Texture1D_Array:
			//glTextureSubImage2D(m_TextureID, 0, 0, 0, m_TextureInfo.Width, m_TextureInfo.Height, GLBaseTextureFormat, GLTextureDataType, Data);
			break;
		case TextureType::Texture2D:
			glTextureSubImage2D(m_TextureID, 0, 0, 0, m_TextureInfo.Width, m_TextureInfo.Height, GLBaseTextureFormat, GLTextureDataType, Data);
			break;
		case TextureType::Texture2D_Array:
			//glTextureSubImage3D(m_TextureID, 0, 0, 0, 0, m_TextureInfo.Width, m_TextureInfo.Height, m_TextureInfo.Depth, GLBaseTextureFormat, GLTextureDataType, Data);
			break;
		case TextureType::CubeMap:
			//glTextureStorage2D(m_TextureID, m_TextureInfo.MipMapLevels, GLInternalTextureFormat, m_TextureInfo.Width, m_TextureInfo.Height);
			break;
		case TextureType::CubeMap_Array:
			//glTextureStorage3D(m_TextureID, m_TextureInfo.MipMapLevels, GLInternalTextureFormat, m_TextureInfo.Width, m_TextureInfo.Height, m_TextureInfo.Depth * 6);
			break;
		case TextureType::Texture3D:
			glTextureSubImage3D(m_TextureID, 0, 0, 0, 0, m_TextureInfo.Width, m_TextureInfo.Height, m_TextureInfo.Depth, GLBaseTextureFormat, GLTextureDataType, Data);
			break;
		}
	}

	GLenum OpenGLTexture::TextureTypeToGLenum(TextureType::Enum Type)
	{
		switch (Type)
		{
		case TextureType::Texture1D:
			return GL_TEXTURE_1D;
		case TextureType::Texture1D_Array:
			return GL_TEXTURE_1D_ARRAY;
		case TextureType::Texture2D:
			return GL_TEXTURE_2D;
		case TextureType::Texture2D_Array:
			return GL_TEXTURE_2D_ARRAY;
		case TextureType::CubeMap:
			return GL_TEXTURE_CUBE_MAP;
		case TextureType::CubeMap_Array:
			return GL_TEXTURE_CUBE_MAP_ARRAY;
		case TextureType::Texture3D:
			return GL_TEXTURE_1D;
		default:
			return 0;
		}
	}

	GLenum OpenGLTexture::GetTextureFormatInternalFormat(TextureFormat::Enum Format)
	{
		switch (Format)
		{
		//8 Bits Per Channel
		case TextureFormat::R8:
			return GL_R8;
		case TextureFormat::R8_G8:
			return GL_RG8;
		case TextureFormat::R8_G8_B8:
			return GL_RGB8;
		case TextureFormat::R8_G8_B8_A8:
			return GL_RGBA8;
		case TextureFormat::R8_G8_B8_SRGB:
			return GL_SRGB8;
		case TextureFormat::R8_G8_B8_A8_SRGB:
			return GL_SRGB8_ALPHA8;

		//16 Bits Per Channel
		case TextureFormat::R16:
			return GL_R16;
		case TextureFormat::R16_G16:
			return GL_RG16;
		case TextureFormat::R16_G16_B16:
			return GL_RGB16;
		case TextureFormat::R16_G16_B16_A16:
			return GL_RGBA16;

		//Half Floating-Point Channels
		case TextureFormat::R16_FLOAT:
			return GL_R16F;
		case TextureFormat::R16_G16_FLOAT:
			return GL_RG16F;
		case TextureFormat::R16_G16_B16_FLOAT:
			return GL_RGB16F;
		case TextureFormat::R16_G16_B16_A16_FLOAT:
			return GL_RGBA16F;

		//Floating-Point Channels
		case TextureFormat::R32_FLOAT:
			return GL_R32F;
		case TextureFormat::R32_G32_FLOAT:
			return GL_RG32F;
		case TextureFormat::R32_G32_B32_FLOAT:
			return GL_RGB32F;
		case TextureFormat::R32_G32_B32_A32_FLOAT:
			return GL_RGBA32F;

		default:
			return 0;
		}
	}

	GLenum OpenGLTexture::GetTextureFormatBaseFormat(TextureFormat::Enum Format)
	{
		switch (Format)
		{
		case TextureFormat::R8:
		case TextureFormat::R16:
		case TextureFormat::R16_FLOAT:
		case TextureFormat::R32_FLOAT:
			return GL_RED;

		case TextureFormat::R8_G8:
		case TextureFormat::R16_G16:
		case TextureFormat::R16_G16_FLOAT:
		case TextureFormat::R32_G32_FLOAT:
			return GL_RG;

		case TextureFormat::R8_G8_B8:
		case TextureFormat::R8_G8_B8_SRGB:
		case TextureFormat::R16_G16_B16:
		case TextureFormat::R16_G16_B16_FLOAT:
		case TextureFormat::R32_G32_B32_FLOAT:
			return GL_RGB;

		case TextureFormat::R8_G8_B8_A8:
		case TextureFormat::R8_G8_B8_A8_SRGB:
		case TextureFormat::R16_G16_B16_A16:
		case TextureFormat::R16_G16_B16_A16_FLOAT:
		case TextureFormat::R32_G32_B32_A32_FLOAT:
			return GL_RGBA;
		default:
			return 0;
		}
	}

	GLenum OpenGLTexture::GetTextureFormatDataType(TextureFormat::Enum Format)
	{
		switch (Format)
		{
			//8 Bits Per Channel
		case TextureFormat::R8:
		case TextureFormat::R8_G8:
		case TextureFormat::R8_G8_B8:
		case TextureFormat::R8_G8_B8_A8:
		case TextureFormat::R8_G8_B8_SRGB:
		case TextureFormat::R8_G8_B8_A8_SRGB:
			return GL_UNSIGNED_BYTE;

			//16 Bits Per Channel
		case TextureFormat::R16:
		case TextureFormat::R16_G16:
		case TextureFormat::R16_G16_B16:
		case TextureFormat::R16_G16_B16_A16:
			return GL_UNSIGNED_SHORT;

			//Half Floating-Point Channels
		case TextureFormat::R16_FLOAT:
		case TextureFormat::R16_G16_FLOAT:
		case TextureFormat::R16_G16_B16_FLOAT:
		case TextureFormat::R16_G16_B16_A16_FLOAT:
			return GL_HALF_FLOAT;

			//Floating-Point Channels
		case TextureFormat::R32_FLOAT:
		case TextureFormat::R32_G32_FLOAT:
		case TextureFormat::R32_G32_B32_FLOAT:
		case TextureFormat::R32_G32_B32_A32_FLOAT:
			return GL_FLOAT;

		default:
			return 0;
		}
	}

	GLenum OpenGLTexture::TextureFilteringModeToGLenum(TextureFilteringMode::Enum Mode)
	{
		switch (Mode)
		{
		case TextureFilteringMode::Linear:
			return GL_LINEAR;
		case TextureFilteringMode::Nearest:
			return GL_NEAREST;
		default:
			return 0;
		}
	}

	GLenum OpenGLTexture::TextureWrappingModeToGLenum(TextureWrappingMode::Enum Mode)
	{
		switch (Mode)
		{
		case TextureWrappingMode::Repeat:
			return GL_REPEAT;
		case TextureWrappingMode::MirroredRepeat:
			return GL_MIRRORED_REPEAT;
		case TextureWrappingMode::ClampToEdge:
			return GL_CLAMP_TO_EDGE;
		case TextureWrappingMode::ClampToBorder:
			return GL_CLAMP_TO_BORDER;
		default:
			return 0;
		}
	}
}