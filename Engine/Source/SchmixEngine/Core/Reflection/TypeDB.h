#pragma once
#include <SchmixEngine/Core/Reflection/TypeInfo.h>

namespace SchmixEngine
{
	template<typename T>
	class TypeDB
	{
	public:
		static const TypeInfo* GetTypeInfo();
	};
}