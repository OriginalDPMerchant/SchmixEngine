#include <SchmixEngine/Core/Reflection/Value.h>

namespace SchmixEngine
{
	Value::Value()
	{
	}

	inline Value::Value(const Value& Other)
	{
		m_Instance = Other.m_Instance;
		m_TypeInfo = Other.m_TypeInfo;
	}

	inline Value& Value::operator=(const Value& Other)
	{
		m_Instance = Other.m_Instance;
		m_TypeInfo = Other.m_TypeInfo;
		return *this;
	}

	inline Value::Value(Value&& Other)
		: m_Instance(std::move(Other.m_Instance)),
		m_TypeInfo(Other.m_TypeInfo)
	{
		Other.m_TypeInfo = { TypeVariant::Unknown, nullptr };
	}

	inline Value& Value::operator=(Value&& Other)
	{
		if (this != &Other)
		{
			m_Instance = std::move(Other.m_Instance);
			m_TypeInfo = Other.m_TypeInfo;

			Other.m_TypeInfo = { TypeVariant::Unknown, nullptr };
		}

		return *this;
	}

	template<typename T>
	inline Value::Value(const T& Instance)
		: m_Instance(Instance),
		m_TypeInfo(ClassDB_CT::GetTypeInfo<std::decay_t<T>>())
	{
		SMX_ASSERT(m_TypeInfo.IsValid());
	}

	template<typename T>
	inline Value& Value::operator=(const T& Instance)
	{
		m_Instance = Instance;
		m_pTypeInfo = ClassDB_CT::GetTypeInfo<std::decay_t<T>>();

		SMX_ASSERT(m_TypeInfo.IsValid());

		return *this;
	}

	inline Value::Value(const char* Instance)
		: m_Instance(std::string(Instance)),
		m_TypeInfo(ClassDB_CT::GetTypeInfo<std::string>())
	{
		SMX_ASSERT(m_TypeInfo.IsValid());
	}

	inline Value& Value::operator=(const char* Instance)
	{
		m_Instance = std::string(Instance);
		m_TypeInfo = ClassDB_CT::GetTypeInfo<std::string>();

		SMX_ASSERT(m_TypeInfo.IsValid());

		return *this;
	}

	inline Value Value::GetProperty(const std::string& PropertyName) const
	{
		if (m_TypeInfo.IsValid())
		{
			const Property* Prop = m_TypeInfo.pClassInfo->GetProperty(PropertyName);

			if (Prop)
			{
				if (Prop->GetValue)
					return Prop->GetValue(*this);
			}

		}

		return Value();
	}

	inline void Value::SetProperty(const std::string& PropertyName, const Value& Data)
	{
		if (m_TypeInfo.IsValid())
		{
			const Property* Prop = m_TypeInfo.pClassInfo->GetProperty(PropertyName);

			if (Prop)
			{
				if (Prop->GetValue)
					Prop->SetValue(*this, Data);
			}

		}
	}

	inline const TypeInfo& Value::GetTypeInfo() const
	{
		return m_TypeInfo;
	}

	template<typename T>
	inline bool Value::IsCastSafe() const
	{
		if constexpr (IsNumberType<T>())
			return IsNumberCastSafe();
		else
			return m_TypeInfo.IsValid() && m_TypeInfo == ClassDB_CT::GetTypeInfo<std::decay_t<T>>();
	}

	template<typename T>
	inline T Value::Cast() const
	{
		SMX_ASSERT(IsCastSafe<T>());

		if constexpr (IsNumberType<T>())
			return NumberCast<T>();
		else
			return std::any_cast<T>(m_Instance);
	}

	template<typename T>
	inline constexpr bool Value::IsNumberType()
	{
		return std::is_fundamental_v<T> && std::is_arithmetic_v<T>;
	}

	inline bool Value::IsNumberCastSafe() const
	{
		return m_TypeInfo == ClassDB_CT::GetTypeInfo<uint8_t>()
			|| m_TypeInfo == ClassDB_CT::GetTypeInfo<uint16_t>()
			|| m_TypeInfo == ClassDB_CT::GetTypeInfo<uint32_t>()
			|| m_TypeInfo == ClassDB_CT::GetTypeInfo<uint64_t>()
			|| m_TypeInfo == ClassDB_CT::GetTypeInfo<int8_t>()
			|| m_TypeInfo == ClassDB_CT::GetTypeInfo<int16_t>()
			|| m_TypeInfo == ClassDB_CT::GetTypeInfo<int32_t>()
			|| m_TypeInfo == ClassDB_CT::GetTypeInfo<int64_t>()
			|| m_TypeInfo == ClassDB_CT::GetTypeInfo<float>()
			|| m_TypeInfo == ClassDB_CT::GetTypeInfo<double>();
	}

	template<typename T>
	inline T Value::NumberCast() const
	{
		if (m_TypeInfo == ClassDB_CT::GetTypeInfo<uint8_t>())
			return static_cast<T>(std::any_cast<uint8_t>(m_Instance));

		if (m_TypeInfo == ClassDB_CT::GetTypeInfo<uint16_t>())
			return static_cast<T>(std::any_cast<uint16_t>(m_Instance));

		if (m_TypeInfo == ClassDB_CT::GetTypeInfo<uint32_t>())
			return static_cast<T>(std::any_cast<uint32_t>(m_Instance));

		if (m_TypeInfo == ClassDB_CT::GetTypeInfo<uint64_t>())
			return static_cast<T>(std::any_cast<uint64_t>(m_Instance));

		if (m_TypeInfo == ClassDB_CT::GetTypeInfo<int8_t>())
			return static_cast<T>(std::any_cast<int8_t>(m_Instance));

		if (m_TypeInfo == ClassDB_CT::GetTypeInfo<int16_t>())
			return static_cast<T>(std::any_cast<int16_t>(m_Instance));

		if (m_TypeInfo == ClassDB_CT::GetTypeInfo<int32_t>())
			return static_cast<T>(std::any_cast<int32_t>(m_Instance));

		if (m_TypeInfo == ClassDB_CT::GetTypeInfo<int64_t>())
			return static_cast<T>(std::any_cast<int64_t>(m_Instance));

		if (m_TypeInfo == ClassDB_CT::GetTypeInfo<float>())
			return static_cast<T>(std::any_cast<float>(m_Instance));

		if (m_TypeInfo == ClassDB_CT::GetTypeInfo<double>())
			return static_cast<T>(std::any_cast<double>(m_Instance));

		return 0;
	}
}
