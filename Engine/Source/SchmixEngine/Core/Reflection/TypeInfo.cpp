#include "TypeInfo.h"

namespace SchmixEngine
{
	TypeInfo::TypeInfo()
		: m_TypeID(0),
		m_pParentTypeInfo(nullptr)
	{
	}

	const std::string& TypeInfo::GetTypeName() const
	{
		return m_TypeName;
	}

	void TypeInfo::SetTypeName(const std::string& TypeName)
	{
		m_TypeName = TypeName;
	}

	const uint64_t TypeInfo::GetTypeID() const
	{
		return m_TypeID;
	}

	void TypeInfo::SetTypeID(uint64_t TypeID)
	{
		m_TypeID = TypeID;
	}

	const TypeInfo* TypeInfo::GetParentTypeInfo() const
	{
		return m_pParentTypeInfo;
	}

	void TypeInfo::SetParentTypeInfo(TypeInfo* pParentTypeInfo)
	{
		if (m_pParentTypeInfo != pParentTypeInfo)
		{
			if (m_pParentTypeInfo)
				const_cast<TypeInfo*>(m_pParentTypeInfo)->RemoveChildTypeInfo(this);

			m_pParentTypeInfo = pParentTypeInfo;

			const_cast<TypeInfo*>(m_pParentTypeInfo)->AddChildTypeInfo(this);
		}
	}

	const std::vector<Property>& TypeInfo::GetAllProperties() const
	{
		return m_Properties;
	}

	const Property* TypeInfo::GetProperty(const std::string& PropertyName) const
	{
		auto Iterator = std::find_if(m_Properties.begin(), m_Properties.end(),
				[PropertyName] (const Property& CurrentProperty)
				{
					return CurrentProperty.PropertyName == PropertyName;
				});

		if (Iterator != m_Properties.end())
			return &(*Iterator);
		else if (m_pParentTypeInfo != nullptr)
			return m_pParentTypeInfo->GetProperty(PropertyName);

		return nullptr;
	}

	void TypeInfo::SetProperty(const Property& NewProperty, bool bShouldOverwrite)
	{
		Property* pProperty = const_cast<Property*>(GetProperty(NewProperty.PropertyName));

		if (pProperty)
		{
			if (bShouldOverwrite)
				*pProperty = NewProperty;
		}
		else
		{
			m_Properties.push_back(NewProperty);
		}
	}

	void TypeInfo::AddChildTypeInfo(TypeInfo* pTypeInfo)
	{
		m_ChildTypeInfos.push_back(pTypeInfo);
	}

	void TypeInfo::RemoveChildTypeInfo(TypeInfo* pTypeInfo)
	{
		auto Iterator = std::find(m_ChildTypeInfos.begin(), m_ChildTypeInfos.end(), pTypeInfo);

		m_ChildTypeInfos.erase(Iterator);
	}
}