#pragma once
#include <SchmixEngine/Core/FileSystem/VFSMountPoint.h>
#include <unordered_map>

namespace SchmixEngine
{
	class VirtualFileSystem
	{
	public:
		VirtualFileSystem();

		VirtualFileSystem(const VirtualFileSystem&) = delete;
		VirtualFileSystem& operator=(const VirtualFileSystem&) = delete;

		virtual ~VirtualFileSystem();

		bool HasFile(const std::filesystem::path& FilePath);
		std::unique_ptr<VFSFile> LoadFile(const std::filesystem::path& FilePath, VFSFileMode Mode);

		VFSMountPoint* AddMountPoint(const std::string& Name);
		VFSMountPoint* GetMountPoint(const std::string& Name);

		void RemoveMountPoint(const std::string& Name);

		static VirtualFileSystem& GetGlobalVFS();

	private:
		std::string GetValidMountPointName(const std::string& Name);

		std::unordered_map<std::string, VFSMountPoint> m_VFSMountPoints;
	};
}