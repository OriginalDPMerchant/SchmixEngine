#include <SchmixEngine/Core/FileSystem/VFSDiskDrive.h>

namespace SchmixEngine
{
	VFSDiskDrive::VFSDiskDrive(const std::filesystem::path& RootPath)
		: VFSDrive(),
		m_RootPath(RootPath)
	{
	}

	bool VFSDiskDrive::HasFile(const std::filesystem::path& FilePath)
	{
		return std::filesystem::status(m_RootPath / FilePath).type() == std::filesystem::file_type::regular;
	}

	std::unique_ptr<VFSFile> VFSDiskDrive::LoadFile(const std::filesystem::path& FilePath, VFSFileMode Mode)
	{
		return HasFile(FilePath) ? std::make_unique<VFSDiskFile>(m_RootPath / FilePath, Mode) : std::unique_ptr<VFSFile>();
	}
}