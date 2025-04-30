#pragma once
#include <stdint.h>

namespace SchmixEngine
{
	enum class VFSFileMode : uint8_t
	{
		Read,
		Write,
		ReadWrite
	};

	class VFSFile
	{
	public:
		VFSFile() = default;

		VFSFile(const VFSFile&) = delete;
		VFSFile& operator=(const VFSFile&) = delete;

		virtual ~VFSFile() = default;
		
		virtual VFSFileMode GetFileMode() = 0;

		virtual size_t GetSize() = 0;

		virtual void Read(char* Data, size_t StartByte, size_t Size) = 0;

		virtual void Write(const char* Data, size_t StartByte, size_t Size) = 0;
	};
}