#pragma once
#include <vector>
#include <string>
#include <SchmixEngine/Core/Reflection/Property.h>
#include <SchmixEngine/Core/Util/FlatMap.h>


namespace SchmixEngine
{
	class ClassInfo
	{
	public:
		ClassInfo();

		const bool IsInitialized() const;

		void SetIsInItialized(bool b_IsInitialized);

		const std::string& GetClassName() const;

		void SetClassName(const std::string& ClassName);

		const uint64_t GetClassID() const;

		void SetClassID(uint64_t ClassID);

		const ClassInfo* GetParentClassInfo() const;

		void SetParentClassInfo(ClassInfo* pParentClassInfo);

		const std::vector<const ClassInfo*>& GetChildClassInfos() const;

		const FlatMap<std::string, Property>& GetAllProperties() const;

		const Property* GetProperty(const std::string& PropertyName) const;

		void SetProperty(const Property& NewProperty, bool bShouldOverwrite = false);

	private:

		void AddChildClassInfo(ClassInfo* pClassInfo);

		void RemoveChildClassInfo(ClassInfo* pClassInfo);

		bool m_bIsInitialized;

		std::string m_ClassName;
		uint64_t m_ClassID;
		const ClassInfo* m_pParentClassInfo;
		
		std::vector<const ClassInfo*> m_ChildClassInfos;

		FlatMap<std::string, Property> m_PropertyMap;
	};
}