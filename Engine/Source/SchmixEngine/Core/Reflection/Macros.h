#pragma once
#include <memory>

#define SMX_BEGIN_REGISTER_CLASS_CT(Class)\
template<>\
const SchmixEngine::ClassInfo* SchmixEngine::ClassDB_CT::GetClassInfo<Class>()\
{\
	static ClassInfo s_ClassInfo;\
	\
	if (!s_ClassInfo.IsInitialized())\
	{\
		s_ClassInfo.SetClassName(#Class);

//TODO : REGISTER PROPERTY MACRO

#define SMX_END_REGISTER_CLASS_CT\
		s_ClassInfo.SetIsInItialized(true);\
	}\
	\
	return &s_ClassInfo;\
}

#define SMX_REGISTER_VALUE_TYPE_FOR_CLASS_CT(Class)\
template<>\
const SchmixEngine::TypeInfo SchmixEngine::ClassDB_CT::GetTypeInfo<Class>()\
{\
	return { TypeVariant::Value, SchmixEngine::ClassDB_CT::GetClassInfo<Class>() };\
}

#define SMX_REGISTER_POINTER_TYPES_FOR_CLASS_CT(Class)\
template<>\
const SchmixEngine::TypeInfo SchmixEngine::ClassDB_CT::GetTypeInfo<Class*>()\
{\
	return { TypeVariant::Ptr, SchmixEngine::ClassDB_CT::GetClassInfo<Class>() };\
}\
\
template<>\
const SchmixEngine::TypeInfo SchmixEngine::ClassDB_CT::GetTypeInfo<std::unique_ptr<Class>>()\
{\
	return { TypeVariant::UniquePtr, SchmixEngine::ClassDB_CT::GetClassInfo<Class>() };\
}\
\
template<>\
const SchmixEngine::TypeInfo SchmixEngine::ClassDB_CT::GetTypeInfo<std::shared_ptr<Class>>()\
{\
	return { TypeVariant::SharedPtr, SchmixEngine::ClassDB_CT::GetClassInfo<Class>() };\
}\
\
template<>\
const SchmixEngine::TypeInfo SchmixEngine::ClassDB_CT::GetTypeInfo<std::weak_ptr<Class>>()\
{\
	return { TypeVariant::WeakPtr, SchmixEngine::ClassDB_CT::GetClassInfo<Class>() };\
}

