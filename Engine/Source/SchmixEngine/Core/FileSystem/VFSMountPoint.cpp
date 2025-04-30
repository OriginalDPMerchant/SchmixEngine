#include <SchmixEngine/Core/FileSystem/VFSMountPoint.h>

namespace SchmixEngine
{
	VFSMountPoint::VFSMountPoint(const std::string& Name)
		: m_MountPointName(Name)
	{
	}

	VFSMountPoint::VFSMountPoint(VFSMountPoint&& Other)
		: m_MountPointName(Other.m_MountPointName),
		m_Drives(std::move(Other.m_Drives))
	{
		Other.m_MountPointName = std::string();
		Other.m_Drives.clear();
	}

	VFSMountPoint& VFSMountPoint::operator=(VFSMountPoint&& Other)
	{
		if (this != &Other)
		{
			m_MountPointName = Other.m_MountPointName;
			m_Drives = std::move(Other.m_Drives);

			Other.m_MountPointName = std::string();
			Other.m_Drives.clear();
		}

		return *this;
	}

	VFSMountPoint::~VFSMountPoint()
	{
	}

	bool VFSMountPoint::HasFile(const std::filesystem::path& FilePath)
	{
		bool bHasFile = false;

		for (std::pair<std::string, std::unique_ptr<VFSDrive>>& Drive : m_Drives)
		{
			if (Drive.second)
			{
				bHasFile = Drive.second->HasFile(FilePath);

				if (bHasFile)
					break;
			}
		}

		return bHasFile;
	}

	std::unique_ptr<VFSFile> VFSMountPoint::LoadFile(const std::filesystem::path& FilePath, VFSFileMode Mode)
	{
		std::unique_ptr<VFSFile> File;

		for (std::pair<std::string, std::unique_ptr<VFSDrive>>& Drive : m_Drives)
		{
			if (Drive.second)
			{
				if (Drive.second->HasFile(FilePath))
				{
					File = Drive.second->LoadFile(FilePath, Mode);
					break;
				}
			}
		}

		return File;
	}

	void VFSMountPoint::AddDrive(std::unique_ptr<VFSDrive> Drive, const std::string& DriveName, uint32_t Priority)
	{
		auto Iterator = std::find_if(m_Drives.begin(), m_Drives.end(),
			[DriveName](const std::pair<std::string, std::unique_ptr<VFSDrive>>& CurrentDrive)
			{
				return DriveName == CurrentDrive.first;
			});

		if (Iterator == m_Drives.end())
		{
			Priority = Priority < m_Drives.size() ? Priority : m_Drives.size();

			m_Drives.insert(m_Drives.begin() + Priority, { DriveName, std::move(Drive) });
		}
		else
		{
			std::string Msg = "VFSMountPoint already has a drive named \"" + DriveName + "\"";
			SMX_DBG_LOG_WSRC(Msg, m_MountPointName, LogLevel::Warning);
		}
	}

	void VFSMountPoint::RemoveDrive(const std::string& DriveName)
	{
		auto Iterator = std::find_if(m_Drives.begin(), m_Drives.end(),
			[DriveName](const std::pair<std::string, std::unique_ptr<VFSDrive>>& CurrentDrive)
			{
				return DriveName == CurrentDrive.first;
			});

		if (Iterator != m_Drives.end())
			m_Drives.erase(Iterator);
	}
}