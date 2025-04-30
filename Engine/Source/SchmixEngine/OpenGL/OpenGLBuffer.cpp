#include <SchmixEngine/OpenGL/OpenGLBuffer.h>

namespace SchmixEngine
{
	OpenGLBuffer::OpenGLBuffer()
		: m_BufferSize(0),
		m_BufferType(BufferType::Unknown),
		m_BufferFlags(0),
		m_bIsBufferPersistentlyMapped(false),
		m_BufferID(0),
		m_pData(nullptr)
	{
	}

	OpenGLBuffer::OpenGLBuffer(OpenGLBuffer&& Other)
		: m_BufferSize(Other.m_BufferSize),
		m_BufferType(Other.m_BufferType),
		m_BufferFlags(Other.m_BufferFlags),
		m_bIsBufferPersistentlyMapped(Other.m_bIsBufferPersistentlyMapped),
		m_BufferID(Other.m_BufferID),
		m_pData(Other.m_pData)
	{
		Other.m_BufferSize = 0;
		Other.m_BufferType = BufferType::Unknown;
		Other.m_bIsBufferPersistentlyMapped = false;
		Other.m_BufferFlags = 0;
		Other.m_BufferID = 0;
		Other.m_pData = nullptr;
	}

	OpenGLBuffer& OpenGLBuffer::operator=(OpenGLBuffer&& Other)
	{
		if (this != &Other)
		{
			Destroy();

			m_BufferSize = Other.m_BufferSize;
			m_BufferType = Other.m_BufferType;
			m_bIsBufferPersistentlyMapped = Other.m_bIsBufferPersistentlyMapped;
			m_BufferFlags = Other.m_BufferFlags;
			m_BufferID = Other.m_BufferID;
			m_pData = Other.m_pData;

			Other.m_BufferSize = 0;
			Other.m_BufferType = BufferType::Unknown;
			Other.m_bIsBufferPersistentlyMapped = false;
			Other.m_BufferFlags = 0;
			Other.m_BufferID = 0;
			Other.m_pData = nullptr;
		}

		return *this;
	}

	OpenGLBuffer::~OpenGLBuffer()
	{
		Destroy();
	}

	void OpenGLBuffer::Init(const void* Data, size_t Size, BufferType Type, bool bIsBufferPersistentlyMapped)
	{
		if (!IsInitialized())
		{
			m_BufferSize = Size;
			m_BufferType = Type;
			m_bIsBufferPersistentlyMapped = bIsBufferPersistentlyMapped;

			m_BufferFlags = GL_MAP_WRITE_BIT;
			m_BufferFlags |= m_bIsBufferPersistentlyMapped ? GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT : 0;
			GLbitfield BufferCreationFlags = m_bIsBufferPersistentlyMapped ? m_BufferFlags : m_BufferFlags | GL_DYNAMIC_STORAGE_BIT;

			glCreateBuffers(1, &m_BufferID);

			if (m_BufferID != 0)
			{
				glNamedBufferStorage(m_BufferID, Size, Data, BufferCreationFlags);

				if (m_bIsBufferPersistentlyMapped)
					m_pData = glMapNamedBufferRange(m_BufferID, 0, m_BufferSize, m_BufferFlags);
			}
			else
			{
				Destroy();
			}
		}
	}

	void OpenGLBuffer::Destroy()
	{
		if (IsInitialized())
			glDeleteBuffers(1, &m_BufferID);

		m_BufferSize = 0;
		m_BufferType = BufferType::Unknown;
		m_bIsBufferPersistentlyMapped = false;
		m_BufferFlags = 0;
		m_BufferID = 0;
		m_pData = nullptr;
	}

	void* OpenGLBuffer::MapMemory(bool bInvalidateBuffer)
	{
		GLbitfield Flags = m_BufferFlags;
		Flags |= bInvalidateBuffer ? GL_MAP_INVALIDATE_BUFFER_BIT : 0;

		if (!m_bIsBufferPersistentlyMapped && IsInitialized())
			m_pData = glMapNamedBufferRange(m_BufferID, 0, m_BufferSize, m_BufferFlags);

		return m_pData;
	}

	void OpenGLBuffer::UnMapMemory()
	{
		if (!m_bIsBufferPersistentlyMapped && IsInitialized())
			glUnmapNamedBuffer(m_BufferID);
	}

	bool OpenGLBuffer::IsInitialized()
	{
		return m_BufferID != 0;
	}

	GLuint OpenGLBuffer::GetBufferID()
	{
		return m_BufferID;
	}

	size_t OpenGLBuffer::GetBufferSize()
	{
		return m_BufferSize;
	}

	BufferType OpenGLBuffer::GetBufferType()
	{
		return m_BufferType;
	}

	bool OpenGLBuffer::IsBufferPersistentlyMapped()
	{
		return m_bIsBufferPersistentlyMapped;
	}
}