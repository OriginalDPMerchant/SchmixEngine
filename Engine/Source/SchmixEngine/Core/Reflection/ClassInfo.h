#pragma once
#include <vector>
#include <string>
#include <SchmixEngine/Core/Reflection/Property.h>
#include <SchmixEngine/Core/Reflection/InheritanceTreeNode.h>
#include <SchmixEngine/Core/Util/FlatMap.h>

namespace SchmixEngine
{
	class ClassInfo
	{
	public:
		typedef void (*CastFunction) (void*, void*);

		ClassInfo();

		const bool IsInitialized() const;

		void SetIsInItialized(bool b_IsInitialized);

		InheritanceTreeNode& GetInheritanceTreeNode();
		const InheritanceTreeNode& GetInheritanceTreeNode() const;

		const std::string& GetClassName() const;

		void SetClassName(const std::string& ClassName);

		const uint64_t GetClassID() const;

		void SetClassID(uint64_t ClassID);

		const FlatMap<std::string, Property>& GetAllProperties() const;

		const Property* GetProperty(const std::string& PropertyName) const;

		void SetProperty(const Property& NewProperty, bool bShouldOverwrite = false);

		CastFunction GetCast(const ClassInfo* pClassInfo) const;

		void SetCast(const ClassInfo* pClassInfo, CastFunction pCastFunction);

	private:

		bool m_bIsInitialized;

		InheritanceTreeNode m_InheritanceTreeNode;

		std::string m_ClassName;
		uint64_t m_ClassID;
		const ClassInfo* m_pParentClassInfo;
		
		std::vector<const ClassInfo*> m_ChildClassInfos;

		FlatMap<std::string, Property> m_PropertyMap;
		FlatMap<const ClassInfo*, CastFunction> m_CastMap;
	};
}