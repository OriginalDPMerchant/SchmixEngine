#pragma once
#include <any>
#include <string>
#include <SchmixEngine/Core/Debugging/Debug.h>
#include <SchmixEngine/Core/Reflection/ClassDB_CT.h>

namespace SchmixEngine
{
	class Value
	{
	public:
		Value();

		virtual ~Value() = default;

		Value(const Value& Other);

		Value& operator=(const Value& Other);

		Value(Value&& Other);

		Value& operator=(Value&& Other);

		template<typename T>
		Value(const T& Instance);

		template<typename T>
		Value& operator=(const T& Instance);

		Value(const char* Instance);
		Value& operator=(const char* Instance);

		Value GetProperty(const std::string& PropertyName) const;

		void SetProperty(const std::string& PropertyName, const Value& Data);

		const TypeInfo& GetTypeInfo() const;

		template<typename T>
		bool IsCastSafe() const;

		template<typename T>
		T Cast() const;

	private:
		template<typename T>
		static constexpr bool IsNumberType();

		bool IsNumberCastSafe() const;

		template<typename T>
		T NumberCast() const;

		std::any m_Instance;
		TypeInfo m_TypeInfo;
	};
}

#include <SchmixEngine/Core/Reflection/Value.inl>
