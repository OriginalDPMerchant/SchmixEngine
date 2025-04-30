#include <SchmixEngine/Core/Reflection/PrimitiveObject.h>

namespace SchmixEngine
{
	PrimitiveObject::PrimitiveObject()
		: m_pTypeInfo(nullptr)
	{
	}

	inline PrimitiveObject::PrimitiveObject(const PrimitiveObject& Other)
	{
		m_Instance = Other.m_Instance;
		m_pTypeInfo = Other.m_pTypeInfo;
	}

	inline PrimitiveObject& PrimitiveObject::operator=(const PrimitiveObject& Other)
	{
		m_Instance = Other.m_Instance;
		m_pTypeInfo = Other.m_pTypeInfo;
		return *this;
	}

	inline PrimitiveObject::PrimitiveObject(PrimitiveObject&& Other)
		: m_Instance(std::move(Other.m_Instance)),
		  m_pTypeInfo(Other.m_pTypeInfo)
	{
		Other.m_pTypeInfo = nullptr;
	}

	inline PrimitiveObject& PrimitiveObject::operator=(PrimitiveObject&& Other)
	{
		if (this != &Other)
		{
			m_Instance = std::move(Other.m_Instance);
			m_pTypeInfo = Other.m_pTypeInfo;

			Other.m_pTypeInfo = nullptr;
		}

		return *this;
	}

	template<typename T>
	inline PrimitiveObject::PrimitiveObject(const T& Instance)
		: m_Instance(Instance),
		m_pTypeInfo(TypeDB<T>::GetTypeInfo())
	{
	}

	template<typename T>
	inline PrimitiveObject& PrimitiveObject::operator=(const T& Instance)
	{
		m_Instance = Instance;
		m_pTypeInfo = TypeDB<T>::GetTypeInfo();
	}

	inline const TypeInfo* PrimitiveObject::GetTypeInfo() const
	{
		return m_pTypeInfo;
	}

	template<typename T>
	inline T PrimitiveObject::Cast() const
	{
		return std::any_cast<T>(m_Instance);
	}
}
