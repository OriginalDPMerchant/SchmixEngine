#pragma once
#include <SchmixEngine/Core/FileSystem/VFSFile.h>
#include <SchmixEngine/Core/Debugging/Debug.h>
#include <filesystem>
#include <fstream>

namespace SchmixEngine
{
	class VFSDiskFile : public VFSFile
	{
	public:
		VFSDiskFile(const std::filesystem::path& FilePath, VFSFileMode FileMode);

		VFSDiskFile(const VFSDiskFile&) = delete;
		VFSDiskFile& operator=(const VFSDiskFile&) = delete;

		virtual ~VFSDiskFile() = default;

		virtual VFSFileMode GetFileMode();

		virtual size_t GetSize();

		virtual void Read(char* Data, size_t StartByte, size_t Size);

		virtual void Write(const char* Data, size_t StartByte, size_t Size);

	private:
		std::fstream m_FileStream;

		size_t m_FileSize;
		VFSFileMode m_FileMode;
	};
}