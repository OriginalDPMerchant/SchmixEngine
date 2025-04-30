#pragma once
#include <vector>
#include <string>
#include <any>
#include <SchmixEngine/Core/Reflection/Property.h>

namespace SchmixEngine
{
	class TypeInfo
	{
	public:
		TypeInfo();

		const std::string& GetTypeName() const;

		void SetTypeName(const std::string& TypeName);

		const uint64_t GetTypeID() const;

		void SetTypeID(uint64_t TypeID);

		const TypeInfo* GetParentTypeInfo() const;

		void SetParentTypeInfo(TypeInfo* pParentTypeInfo);

		const std::vector<const TypeInfo*>& GetChildTypeInfos() const;

		const std::vector<Property>& GetAllProperties() const;

		const Property* GetProperty(const std::string& PropertyName) const;

		void SetProperty(const Property& NewProperty, bool bShouldOverwrite = false);

	private:

		void AddChildTypeInfo(TypeInfo* pTypeInfo);

		void RemoveChildTypeInfo(TypeInfo* pTypeInfo);

		std::string m_TypeName;
		uint64_t m_TypeID;
		const TypeInfo* m_pParentTypeInfo;
		std::vector<const TypeInfo*> m_ChildTypeInfos;

		std::vector<Property> m_Properties;
	};
}