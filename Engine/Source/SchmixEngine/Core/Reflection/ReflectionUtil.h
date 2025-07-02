#pragma once
#include <SchmixEngine/Core/Reflection/ClassDB_CT.h>
#include <SchmixEngine/Core/Debugging/Debug.h>

namespace SchmixEngine
{
	class ReflectionUtil
	{
	public:

		// T = Class
		template<typename T>
		static void RegisterClassName(const std::string& ClassName);

		// T = Derived, S = Base
		template<typename T, typename S>
		static void RegisterParentClass();

		// T = Src, S = Dst 
		template<typename T, typename S>
		static void RegisterCast(ClassInfo::CastFunction pCastFunction);

		// T = Src, S = Dst 
		template<typename T, typename S>
		static void RegisterStaticCast();

		template<typename T>
		static void RegisterProperty(Property Prop);

		template<typename T, typename S>
		static void RegisterProperty(std::string PropertyName, S T::*pMemberVariable);

	private:
		template<typename T, typename S>
		static void StaticValueCast(void* pSrc, void* pDst);

		template<typename T, typename S>
		static const void* StaticPtrCast(const void* pSrc);

		template<typename T, typename S>
		static const void* DynamicPtrCast(const void* pSrc);

		//static int s_InheritanceTreeDepthLimit;
	};
}

#include <SchmixEngine/Core/Reflection/ReflectionUtil.inl>