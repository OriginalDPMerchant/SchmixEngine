#include <SchmixEngine/Core/Reflection/Object.h>

namespace SchmixEngine
{
	Object* Object::GetValue(const std::string& PropertyName)
	{
		const TypeInfo* pTypeInfo = GetTypeInfo();

		if (pTypeInfo)
		{
			const Property* pProperty = pTypeInfo->GetProperty(PropertyName);

			if (pProperty)
				return pProperty->GetValue(this);
		}

		return nullptr;
	}

	void Object::SetValue(const std::string& PropertyName, const Object* Value)
	{
		const TypeInfo* pTypeInfo = GetTypeInfo();

		if (pTypeInfo)
		{
			const Property* pProperty = pTypeInfo->GetProperty(PropertyName);

			if (pProperty)
				pProperty->SetValue(this, Value);
		}
	}
}
