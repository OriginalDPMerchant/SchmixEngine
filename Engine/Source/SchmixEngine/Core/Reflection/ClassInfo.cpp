#include "ClassInfo.h"

namespace SchmixEngine
{
	ClassInfo::ClassInfo()
		: m_bIsInitialized(false),
		m_ClassID(0),
		m_pParentClassInfo(nullptr)
	{
	}

	const bool ClassInfo::IsInitialized() const
	{
		return m_bIsInitialized;
	}

	void ClassInfo::SetIsInItialized(bool b_IsInitialized)
	{
		m_bIsInitialized = m_bIsInitialized;
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

	const ClassInfo* ClassInfo::GetParentClassInfo() const
	{
		return m_pParentClassInfo;
	}

	void ClassInfo::SetParentClassInfo(ClassInfo* pParentClassInfo)
	{
		if (m_pParentClassInfo != pParentClassInfo)
		{
			if (m_pParentClassInfo)
				const_cast<ClassInfo*>(m_pParentClassInfo)->RemoveChildClassInfo(this);

			m_pParentClassInfo = pParentClassInfo;

			const_cast<ClassInfo*>(m_pParentClassInfo)->AddChildClassInfo(this);
		}
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

	void ClassInfo::AddChildClassInfo(ClassInfo* pClassInfo)
	{
		m_ChildClassInfos.push_back(pClassInfo);
	}

	void ClassInfo::RemoveChildClassInfo(ClassInfo* pClassInfo)
	{
		auto Iterator = std::find(m_ChildClassInfos.begin(), m_ChildClassInfos.end(), pClassInfo);

		m_ChildClassInfos.erase(Iterator);
	}
}