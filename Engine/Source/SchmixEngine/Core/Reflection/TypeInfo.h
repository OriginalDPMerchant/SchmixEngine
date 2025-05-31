#pragma once
#include <SchmixEngine/Core/Reflection/ClassInfo.h>

namespace SchmixEngine
{
	enum class TypeVariant
	{
		Unknown,
		Value,
		Ptr,
		UniquePtr,
		SharedPtr,
		WeakPtr
	};

	struct TypeInfo
	{
		TypeVariant Variant = TypeVariant::Unknown;
		const ClassInfo* pClassInfo = nullptr;

		inline bool IsValid() const
		{
			return Variant != TypeVariant::Unknown && pClassInfo;
		}

		inline bool operator==(const TypeInfo& Other) const
		{
			return Other.Variant == Variant && Other.pClassInfo == pClassInfo;
		}
	};
}