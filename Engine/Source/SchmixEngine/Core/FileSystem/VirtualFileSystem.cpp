#include <SchmixEngine/Core/FileSystem/VirtualFileSystem.h>

namespace SchmixEngine
{
	VirtualFileSystem::VirtualFileSystem()
	{
	}

	VirtualFileSystem::~VirtualFileSystem()
	{
	}

	bool VirtualFileSystem::HasFile(const std::filesystem::path& FilePath)
	{
		std::string MountPoint = !FilePath.empty() ? FilePath.begin()->string() : std::string();

		auto Iterator = m_VFSMountPoints.find(MountPoint);

		if (Iterator != m_VFSMountPoints.end())
		{
			std::filesystem::path Path;
			
			for (auto PathIterator = FilePath.begin(); PathIterator != FilePath.end(); PathIterator++)
			{
				if (PathIterator != FilePath.begin())
					Path /= *PathIterator;
			}

			return Iterator->second.HasFile(Path);
		}

		return false;
	}

	std::unique_ptr<VFSFile> VirtualFileSystem::LoadFile(const std::filesystem::path& FilePath, VFSFileMode Mode)
	{
		std::string MountPoint = !FilePath.empty() ? FilePath.begin()->string() : std::string();

		auto Iterator = m_VFSMountPoints.find(MountPoint);

		if (Iterator != m_VFSMountPoints.end())
		{
			std::filesystem::path Path;

			for (auto PathIterator = FilePath.begin(); PathIterator != FilePath.end(); PathIterator++)
			{
				if (PathIterator != FilePath.begin())
					Path /= *PathIterator;
			}

			return Iterator->second.LoadFile(Path, Mode);
		}

		return std::unique_ptr<VFSFile>();
	}

	VFSMountPoint* VirtualFileSystem::AddMountPoint(const std::string& Name)
	{
		std::string MountPointName = GetValidMountPointName(Name);

		if (MountPointName.empty())
		{
			//TODO: MESSAGE ABOUT UNSUITABLE MOUNTPOINT NAME
		}
		else
		{
			auto Iterator = m_VFSMountPoints.find(MountPointName);

			if (Iterator == m_VFSMountPoints.end())
			{
				auto NewIterator = m_VFSMountPoints.insert({ MountPointName, std::move(VFSMountPoint(MountPointName)) });
				return &(NewIterator.first->second);
			}
			else
			{
				return &(Iterator->second);
			}
		}

		return nullptr;
	}

	VFSMountPoint* VirtualFileSystem::GetMountPoint(const std::string& Name)
	{
		std::string MountPointName = GetValidMountPointName(Name);

		if (MountPointName.empty())
		{
			//TODO: MESSAGE ABOUT UNSUITABLE MOUNTPOINT NAME
		}
		else
		{
			auto Iterator = m_VFSMountPoints.find(MountPointName);

			if (Iterator != m_VFSMountPoints.end())
			{
				return &(Iterator->second);
			}
		}

		return nullptr;
	}

	void VirtualFileSystem::RemoveMountPoint(const std::string& Name)
	{
		std::string MountPointName = GetValidMountPointName(Name);

		if (MountPointName.empty())
		{
			//TODO: MESSAGE ABOUT UNSUITABLE MOUNTPOINT NAME
		}
		else
		{
			auto Iterator = m_VFSMountPoints.find(MountPointName);

			if (Iterator != m_VFSMountPoints.end())
				m_VFSMountPoints.erase(Iterator);
		}
	}

	VirtualFileSystem& VirtualFileSystem::GetGlobalVFS()
	{
		static VirtualFileSystem VFS;
		return VFS;
	}

	std::string VirtualFileSystem::GetValidMountPointName(const std::string& Name)
	{
		std::filesystem::path Path(Name);

		if (!Path.empty())
		{
			if (++Path.begin() == Path.end())
			{
				return Name[Name.size() - 1] == ':' ? Name : Name + ":";
			}
		}

		return std::string();
	}
}