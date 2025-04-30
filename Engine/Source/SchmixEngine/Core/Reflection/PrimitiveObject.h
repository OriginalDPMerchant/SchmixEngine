#pragma once
#include <any>
#include <string>
#include <SchmixEngine/Core/Reflection/Object.h>

namespace SchmixEngine
{
	class PrimitiveObject : public Object
	{
	public:
		PrimitiveObject();

		PrimitiveObject(const PrimitiveObject& Other);

		PrimitiveObject& operator=(const PrimitiveObject& Other);

		PrimitiveObject(PrimitiveObject&& Other);

		PrimitiveObject& operator=(PrimitiveObject&& Other);

		template<typename T>
		PrimitiveObject(const T& Other);

		template<typename T>
		PrimitiveObject& operator=(const T& Other);

		virtual ~PrimitiveObject() = default;

		virtual const TypeInfo* GetTypeInfo() const override;

		template<typename T>
		T Cast() const;

	private:
		std::any m_Instance;
		const TypeInfo* m_pTypeInfo;
	};
}

#include <SchmixEngine/Core/Reflection/PrimitiveObject.inl>