#pragma once
#include <SchmixEngine/Core/FileSystem/VFSDrive.h>
#include <SchmixEngine/Core/Debugging/Debug.h>
#include <vector>
#include <string>

namespace SchmixEngine
{
	class VFSMountPoint
	{
	public:
		VFSMountPoint(const std::string& Name);

		VFSMountPoint(const VFSMountPoint&) = delete;
		VFSMountPoint& operator=(const VFSMountPoint&) = delete;

		VFSMountPoint(VFSMountPoint&& Other);
		VFSMountPoint& operator=(VFSMountPoint&& Other);

		virtual ~VFSMountPoint();

		bool HasFile(const std::filesystem::path& FilePath);

		std::unique_ptr<VFSFile> LoadFile(const std::filesystem::path& FilePath, VFSFileMode Mode);

		void AddDrive(std::unique_ptr<VFSDrive> Drive, const std::string& DriveName, uint32_t Priority);

		void RemoveDrive(const std::string& DriveName);

	private:
		std::string m_MountPointName;
		std::vector<std::pair<std::string, std::unique_ptr<VFSDrive>>> m_Drives;
	};
}