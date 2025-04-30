#pragma once
#include <SchmixEngine/Core/Graphics/Core/Texture.h>
#include <SchmixEngine/ThirdParty/GLAD/include/glad/glad.h>

namespace SchmixEngine
{
	class OpenGLTexture
	{
	public:
		OpenGLTexture();

		OpenGLTexture(const OpenGLTexture&) = delete;
		OpenGLTexture& operator=(const OpenGLTexture&) = delete;

		OpenGLTexture(OpenGLTexture&& Other);
		OpenGLTexture& operator=(OpenGLTexture&& Other);

		~OpenGLTexture();

		void Init(const TextureInfo& Info, void* Data);
		void Destroy();

		void UploadData(void* Data, TextureFormat::Enum Format, uint32_t MipMapLevels, uint32_t Width, uint32_t Height, uint32_t Depth);

		void SetFilterMode(TextureFilteringMode::Enum Mode);
		void SetWrappingMode(TextureWrappingMode::Enum Mode);

		bool IsInitialized();

		TextureInfo GetTextureInfo();
		GLuint GetTextureID();

	private:
		void CreateTexture();
		void UpdateTextureData(void* Data);

		GLenum TextureTypeToGLenum(TextureType::Enum Type);
		GLenum GetTextureFormatInternalFormat(TextureFormat::Enum Format);
		GLenum GetTextureFormatBaseFormat(TextureFormat::Enum Format);
		GLenum GetTextureFormatDataType(TextureFormat::Enum Format);
		GLenum TextureFilteringModeToGLenum(TextureFilteringMode::Enum Mode);
		GLenum TextureWrappingModeToGLenum(TextureWrappingMode::Enum Mode);

		GLuint m_TextureID;
		TextureInfo m_TextureInfo;
	};
}