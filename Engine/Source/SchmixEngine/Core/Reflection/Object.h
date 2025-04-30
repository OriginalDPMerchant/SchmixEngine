#pragma once
#include <memory>
#include <string>
#include <SchmixEngine/Core/Reflection/TypeDB.h>

namespace SchmixEngine
{
	struct Property;

	class Object
	{
	public:
		Object() = default;
		
		virtual ~Object() = default;

		Object* GetValue(const std::string& PropertyName);

		void SetValue(const std::string& PropertyName, const Object* Value);

		virtual const TypeInfo* GetTypeInfo() const = 0;
	};
}