#include <SchmixEngine/Core/FileSystem/VFSDiskFile.h>

namespace SchmixEngine
{
	VFSDiskFile::VFSDiskFile(const std::filesystem::path& FilePath, VFSFileMode FileMode)
		: VFSFile(),
		m_FileMode(FileMode),
		m_FileSize(0)
	{
		std::ios::openmode OpenMode = std::ios::binary | std::ios::ate;
		OpenMode |= FileMode == VFSFileMode::Read || m_FileMode == VFSFileMode::ReadWrite ? std::ios::in : 0;
		OpenMode |= FileMode == VFSFileMode::Write || m_FileMode == VFSFileMode::ReadWrite ? std::ios::out : 0;

		m_FileStream = std::fstream(FilePath, OpenMode);
		m_FileSize = static_cast<size_t>(m_FileStream.tellg());
	}

	VFSFileMode VFSDiskFile::GetFileMode()
	{
		return m_FileMode;
	}

	size_t VFSDiskFile::GetSize()
	{
		return m_FileSize;
	}

	void VFSDiskFile::Read(char* Data, size_t StartByte, size_t Size)
	{
		if (m_FileMode == VFSFileMode::Read || m_FileMode == VFSFileMode::ReadWrite)
		{
			StartByte = StartByte < m_FileSize ? StartByte : m_FileSize - 1;
			Size = Size + StartByte <= m_FileSize ? Size : m_FileSize - StartByte;

			m_FileStream.seekg(StartByte);
			m_FileStream.read(Data, Size);
		}
		else
		{
			SMX_DBG_LOG("Cannot Read as the File is Write Only", LogLevel::Warning);
		}
	}

	void VFSDiskFile::Write(const char* Data, size_t StartByte, size_t Size)
	{
		if (m_FileMode == VFSFileMode::Write || m_FileMode == VFSFileMode::ReadWrite)
		{
			StartByte = StartByte <= m_FileSize ? StartByte : m_FileSize;

			m_FileStream.seekp(StartByte);
			m_FileStream.write(Data, Size);

			m_FileSize = std::max(Size + StartByte, m_FileSize);
		}
		else
		{
			SMX_DBG_LOG("Cannot Write as the File is Read Only", LogLevel::Warning);
		}
	}
}