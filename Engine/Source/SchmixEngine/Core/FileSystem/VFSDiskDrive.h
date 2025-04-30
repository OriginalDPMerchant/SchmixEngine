#pragma once
#include <SchmixEngine/Core/FileSystem/VFSDrive.h>
#include <SchmixEngine/Core/FileSystem/VFSDiskFile.h>

namespace SchmixEngine
{
	class VFSDiskDrive : public VFSDrive
	{
	public:
		VFSDiskDrive(const std::filesystem::path& RootPath);

		virtual ~VFSDiskDrive() = default;

		virtual bool HasFile(const std::filesystem::path& FilePath);

		virtual std::unique_ptr<VFSFile> LoadFile(const std::filesystem::path& FilePath, VFSFileMode Mode);

	private:
		std::filesystem::path m_RootPath;
	};
}