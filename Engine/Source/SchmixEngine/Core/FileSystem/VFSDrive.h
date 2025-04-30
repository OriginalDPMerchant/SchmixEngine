#pragma once
#include <SchmixEngine/Core/FileSystem/VFSFile.h>
#include <filesystem>
#include <memory>

namespace SchmixEngine
{
	class VFSDrive
	{
	public:
		VFSDrive() = default;

		virtual ~VFSDrive() = default;
		
		virtual bool HasFile(const std::filesystem::path& FilePath) = 0;

		virtual std::unique_ptr<VFSFile> LoadFile(const std::filesystem::path& FilePath, VFSFileMode Mode) = 0;
	};
}