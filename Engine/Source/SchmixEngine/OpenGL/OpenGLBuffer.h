#pragma once
#include <SchmixEngine/Core/Graphics/Core/Buffer.h>
#include <SchmixEngine/ThirdParty/GLAD/include/glad/glad.h>

namespace SchmixEngine
{
	class OpenGLBuffer final
	{
	public:
		OpenGLBuffer();

		OpenGLBuffer(const OpenGLBuffer&) = delete;
		OpenGLBuffer& operator=(const OpenGLBuffer&) = delete;

		OpenGLBuffer(OpenGLBuffer&& Other);
		OpenGLBuffer& operator=(OpenGLBuffer&& Other);

		~OpenGLBuffer();

		void Init(const void* Data, size_t Size, BufferType Type, bool bIsBufferPersistentlyMapped = false);
		void Destroy();

		void* MapMemory(bool bInvalidateBuffer);
		void UnMapMemory();

		bool IsInitialized();

		GLuint GetBufferID();
		size_t GetBufferSize();
		BufferType GetBufferType();
		bool IsBufferPersistentlyMapped();

	private:
		size_t m_BufferSize;
		BufferType m_BufferType;
		GLbitfield m_BufferFlags;

		bool m_bIsBufferPersistentlyMapped;

		GLuint m_BufferID;
		void* m_pData;
	};
}