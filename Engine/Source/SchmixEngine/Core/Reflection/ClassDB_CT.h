#pragma once
#include <SchmixEngine/Core/Reflection/TypeInfo.h>

namespace SchmixEngine
{
	class ClassDB_CT
	{
	public:
		template<typename T>
		static inline const ClassInfo* GetClassInfo()
		{
			return nullptr;
		}

		template<typename T>
		static inline const TypeInfo GetTypeInfo()
		{
			return { TypeVariant::Unknown, nullptr };
		}
	};
}

#include <SchmixEngine/Core/Reflection/Primitives.inl>