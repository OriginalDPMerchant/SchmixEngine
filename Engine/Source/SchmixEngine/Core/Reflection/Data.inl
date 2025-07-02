#include <SchmixEngine/Core/Reflection/Data.h>

namespace SchmixEngine
{
	inline Data::Data()
		: m_DataType(DataType::Unknown),
		m_pClassInfo(nullptr),
		m_pData(nullptr),
		m_pDestructor(nullptr),
		m_pCopyFunction(nullptr)
	{
	}

	inline Data::Data(const Data& Other)
		: m_DataType(Other.m_DataType),
		m_pClassInfo(Other.m_pClassInfo),
		m_pData(nullptr),
		m_pDestructor(Other.m_pDestructor),
		m_pCopyFunction(Other.m_pCopyFunction)
	{
		switch (m_DataType)
		{
		case DataType::Value:
			m_pData = m_pCopyFunction(Other.m_pData);
			break;
		case DataType::Ptr:
			m_pData = Other.m_pData;
			break;
		default:
			break;
		}
	}

	inline Data& Data::operator=(const Data& Other)
	{
		if (this != &Other)
		{
			SMX_LOG("copy operator", LogLevel::Debug);

			CleanUp();

			m_DataType = Other.m_DataType;
			m_pClassInfo = Other.m_pClassInfo;
			m_pDestructor = Other.m_pDestructor;
			m_pCopyFunction = Other.m_pCopyFunction;

			switch (m_DataType)
			{
			case DataType::Value:
				m_pData = m_pCopyFunction(Other.m_pData);
				break;
			case DataType::Ptr:
				m_pData = Other.m_pData;
				break;
			default:
				break;
			}
		}
		
		return *this;
	}

	inline Data::Data(Data&& Other)
		: m_DataType(Other.m_DataType),
		m_pClassInfo(Other.m_pClassInfo),
		m_pData(Other.m_pData),
		m_pDestructor(Other.m_pDestructor),
		m_pCopyFunction(Other.m_pCopyFunction)
	{
		Other.m_DataType = DataType::Unknown;
		Other.m_pClassInfo = nullptr;
		Other.m_pData = nullptr;
		Other.m_pDestructor = nullptr;
		Other.m_pCopyFunction = nullptr;
	}

	inline Data& Data::operator=(Data&& Other)
	{
		if (this != &Other)
		{
			CleanUp();

			m_DataType = Other.m_DataType;
			m_pClassInfo = Other.m_pClassInfo;
			m_pData = Other.m_pData;
			m_pDestructor = Other.m_pDestructor;
			m_pCopyFunction = Other.m_pCopyFunction;

			Other.m_DataType = DataType::Unknown;
			Other.m_pClassInfo = nullptr;
			Other.m_pData = nullptr;
			Other.m_pDestructor = nullptr;
			Other.m_pCopyFunction = nullptr;
		}

		return *this;
	}

	inline Data::~Data()
	{
		CleanUp();
	}

	inline Data Data::GetProperty(const std::string& PropertyName)
	{
		if (m_pClassInfo)
		{
			const Property* pProperty = m_pClassInfo->GetProperty(PropertyName);

			if (pProperty)
			{
				if (pProperty->GetValue)
					return pProperty->GetValue(*this);
			}
		}

		return Data();
	}

	inline void Data::SetProperty(const std::string& PropertyName, const Data& Value)
	{
		if (m_pClassInfo)
		{
			const Property* pProperty = m_pClassInfo->GetProperty(PropertyName);

			if (pProperty)
			{
				if (pProperty->SetValue)
					pProperty->SetValue(*this, Value);
			}
		}
	}

	inline Data::Data(DataType Type, const ClassInfo* pClassInfo, void* pData, DestroyFunction pDestructor, CopyFunction pCopyFunction)
		: m_DataType(Type),
		m_pClassInfo(pClassInfo),
		m_pData(pData),
		m_pDestructor(pDestructor),
		m_pCopyFunction(pCopyFunction)
	{
		SMX_ASSERT(m_DataType != DataType::Unknown);
		SMX_ASSERT(m_pClassInfo != nullptr);

		if (m_DataType != DataType::Ptr)
			SMX_ASSERT(m_pData != nullptr && m_pDestructor != nullptr && m_pCopyFunction != nullptr);

	}

	inline void Data::CleanUp()
	{
		if (m_pDestructor && m_pData)
		{
			m_pDestructor(m_pData);
			m_pData = nullptr;
		}
	}

	template<typename T>
	inline Data Data::Value(const T& Value)
	{
		return Data(DataType::Value, ClassDB_CT::GetClassInfo<T>(), new T(Value), Data::Destroy<T>, Data::Copy<T>);
	}

	template<typename T>
	inline Data Data::Ptr(T* Ptr)
	{
		return Data(DataType::Ptr, ClassDB_CT::GetClassInfo<T>(), Ptr, nullptr, nullptr);
	}

	//TODO: MAKE WORK FOR SINGLE INHERITANCE
	template<typename T>
	inline std::optional<T> Data::GetValue() const
	{
		if (m_pData)
		{
			if (m_pClassInfo == ClassDB_CT::GetClassInfo<T>())
			{
				switch (m_DataType)
				{
				case DataType::Value:
				case DataType::Ptr:
					return *static_cast<T*>(m_pData);
				}
			}
			else
			{
				ClassInfo::CastFunction pCastFunction = m_pClassInfo->GetCast(ClassDB_CT::GetClassInfo<T>());

				if (pCastFunction)
				{
					T CastResult = T();
					pCastFunction(m_pData, &CastResult);

					return CastResult;
				}
			}
		}

		return std::optional<T>();
	}

	template<typename T>
	inline T* Data::GetPtr()
	{
		if (m_pClassInfo == ClassDB_CT::GetClassInfo<T>())
		{
			switch (m_DataType)
			{
			case DataType::Value:
			case DataType::Ptr:
				return static_cast<T*>(m_pData);
			}
		}

		return nullptr;
	}

	template<typename T>
	inline const T* Data::GetPtr() const
	{
		return const_cast<Data*>(this)->GetPtr<T>();
	}

	template<typename T>
	inline void Data::Destroy(void* ptr)
	{
		T* TypedPtr = static_cast<T*>(ptr);
		delete TypedPtr;
	}

	template<typename T>
	inline void* Data::Copy(void* ptr)
	{
		T* Copy = new T(*static_cast<T*>(ptr));
		return Copy;
	}
}