#include <SchmixEngine/Core/Reflection/ReflectionUtil.h>

namespace SchmixEngine
{
	template<typename T>
	inline void ReflectionUtil::RegisterClassName(const std::string& ClassName)
	{
		ClassInfo* pClassInfo = const_cast<ClassInfo*>(ClassDB_CT::GetClassInfo<T>(true));

		SMX_DBG_ASSERT(pClassInfo && "Please Provide specializations of SchmixEngine::ClassDB_CT::HasClassInfo() and SchmixEngine::ClassDB_CT::InitClassInfo()");
		
		if (pClassInfo)
			pClassInfo->SetClassName(ClassName);
	}

	template<typename T, typename S>
	inline void ReflectionUtil::RegisterParentClass()
	{
		static_assert(std::is_base_of_v<S, T>);

		const ClassInfo* pBaseClassInfo = ClassDB_CT::GetClassInfo<S>(true);
		ClassInfo* pDerivedClassInfo = const_cast<ClassInfo*>(ClassDB_CT::GetClassInfo<T>(true));

		SMX_DBG_ASSERT(pBaseClassInfo && pDerivedClassInfo && "Please Provide specializations of SchmixEngine::ClassDB_CT::HasClassInfo() and SchmixEngine::ClassDB_CT::InitClassInfo()");

		if (pBaseClassInfo && pDerivedClassInfo)
		{
			const InheritanceTreeNode& BaseClassNode = pBaseClassInfo->GetInheritanceTreeNode();
			InheritanceTreeNode& DerivedClassNode = pDerivedClassInfo->GetInheritanceTreeNode();

			SMX_DBG_ASSERT(!DerivedClassNode.GetParent() && "Attempted to Register A Parent Class To A Class Which Already Has a Parent");

			if (!DerivedClassNode.GetParent())
			{
				DerivedClassNode.SetParent(&BaseClassNode);

				DerivedClassNode.SetCastPtrToParentTypeFunction(StaticPtrCast<T, S>);

				if constexpr (std::is_polymorphic_v<S> && std::is_polymorphic_v<T>)
					DerivedClassNode.SetCastPtrFromParentTypeFunction(DynamicPtrCast<S, T>);
			}
		}
	}

	template<typename T, typename S>
	inline void ReflectionUtil::RegisterCast(ClassInfo::CastFunction pCastFunction)
	{
		ClassInfo* pSrcClass = const_cast<ClassInfo*>(ClassDB_CT::GetClassInfo<T>(true));
		const ClassInfo* pDstClass = ClassDB_CT::GetClassInfo<S>(true);

		SMX_DBG_ASSERT(pSrcClass && pDstClass && "Please Provide specializations of SchmixEngine::ClassDB_CT::HasClassInfo() and SchmixEngine::ClassDB_CT::InitClassInfo()");

		if (pSrcClass && pDstClass)
			pSrcClass->SetCast(pDstClass, pCastFunction);
	}

	template<typename T, typename S>
	inline void ReflectionUtil::RegisterStaticCast()
	{
		RegisterCast<T, S>(StaticValueCast<T, S>);
	}

	template<typename T>
	inline void ReflectionUtil::RegisterProperty(Property Prop)
	{
		ClassInfo* pClassInfo = const_cast<ClassInfo*>(ClassDB_CT::GetClassInfo<T>(true));

		SMX_DBG_ASSERT(pClassInfo && "Please Provide specializations of SchmixEngine::ClassDB_CT::HasClassInfo() and SchmixEngine::ClassDB_CT::InitClassInfo()");

		if (pClassInfo)
			pClassInfo->SetProperty(Prop);
	}

	//TODO: MAKE WORK FOR PTR, SHARED_PTR, WEAK_PTR
	template<typename T, typename S>
	inline void ReflectionUtil::RegisterProperty(std::string PropertyName, S T::* pMemberVariable)
	{
		RegisterProperty<T>
		({
			PropertyName,
			[pMemberVariable](const Data& Object) -> Data
			{
				const T* pTObject = Object.GetPtr<T>();

				return pTObject ? Data::Value(pTObject->*pMemberVariable) : Data();
			},
			[pMemberVariable](Data& Object, const Data& Value) -> void
			{
				T* pTObject = Object.GetPtr<T>();
				std::optional<S> SValue = Value.GetValue<S>();

				if (pTObject && SValue.has_value())
					pTObject->*pMemberVariable = SValue.value();
			}
		});
	}

	template<typename T, typename S>
	inline void ReflectionUtil::StaticValueCast(void* pSrc, void* pDst)
	{
		T* pTSrc = static_cast<T*>(pSrc);
		S* pSDst = static_cast<S*>(pDst);
		*pSDst = static_cast<S>(*pTSrc);
	}

	template<typename T, typename S>
	inline const void* ReflectionUtil::StaticPtrCast(const void* pSrc)
	{
		return static_cast<const S*>(static_cast<const T*>(pSrc));
	}

	template<typename T, typename S>
	inline const void* ReflectionUtil::DynamicPtrCast(const void* pSrc)
	{
		static_assert(std::is_polymorphic_v<T> && std::is_polymorphic_v<S>);

		const T* pTSrc = static_cast<const T*>(pSrc);
		return pTSrc ? dynamic_cast<const S*>(pTSrc) : nullptr;
	}
}