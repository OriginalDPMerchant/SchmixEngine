#include "ClassInfo.h"

namespace SchmixEngine
{
	ClassInfo::ClassInfo()
		: m_bIsInitialized(false),
		m_ClassID(0),
		m_pParentClassInfo(nullptr),
		m_InheritanceTreeNode(this)
	{
	}

	const bool ClassInfo::IsInitialized() const
	{
		return m_bIsInitialized;
	}

	void ClassInfo::SetIsInItialized(bool bIsInitialized)
	{
		m_bIsInitialized = bIsInitialized;
	}

	InheritanceTreeNode& ClassInfo::GetInheritanceTreeNode()
	{
		return m_InheritanceTreeNode;
	}

	const InheritanceTreeNode& ClassInfo::GetInheritanceTreeNode() const
	{
		return m_InheritanceTreeNode;
	}

	const std::string& ClassInfo::GetClassName() const
	{
		return m_ClassName;
	}

	void ClassInfo::SetClassName(const std::string& ClassName)
	{
		m_ClassName = ClassName;
	}

	const uint64_t ClassInfo::GetClassID() const
	{
		return m_ClassID;
	}

	void ClassInfo::SetClassID(uint64_t ClassID)
	{
		m_ClassID = ClassID;
	}

	const FlatMap<std::string, Property>& ClassInfo::GetAllProperties() const
	{
		return m_PropertyMap;
	}

	const Property* ClassInfo::GetProperty(const std::string& PropertyName) const
	{
		auto Iterator = m_PropertyMap.find(PropertyName);

		if (Iterator != m_PropertyMap.end())
			return &(Iterator->second);
		else if (m_pParentClassInfo != nullptr)
			return m_pParentClassInfo->GetProperty(PropertyName);

		return nullptr;
	}

	void ClassInfo::SetProperty(const Property& NewProperty, bool bShouldOverwrite)
	{
		Property* pProperty = const_cast<Property*>(GetProperty(NewProperty.PropertyName));

		if (pProperty)
		{
			if (bShouldOverwrite)
				*pProperty = NewProperty;
		}
		else
		{
			m_PropertyMap.insert({ NewProperty.PropertyName, NewProperty });
		}
	}

	ClassInfo::CastFunction ClassInfo::GetCast(const ClassInfo* pClassInfo) const
	{
		auto Iterator = m_CastMap.find(pClassInfo);

		if (Iterator != m_CastMap.end())
			return Iterator->second;

		return nullptr;
	}

	void ClassInfo::SetCast(const ClassInfo* pClassInfo, CastFunction pCastFunction)
	{
		m_CastMap[pClassInfo] = pCastFunction;
	}
}