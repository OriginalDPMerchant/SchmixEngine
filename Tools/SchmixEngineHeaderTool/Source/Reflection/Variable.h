#pragma once
#include <string>
#include <vector>

namespace SchmixEngineReflection
{
	template<typename T>
	struct Type
	{
		static std::string GetTypeName();

		static int64_t GetTypeID();

		static Value MakeInstance()
		{
			return Value(Type<T>::GetTypeName(), Type<T>::GetTypeID(), new T(), Type<T>::Destroy);
		}

		static T* Cast(Value Instance)
		{
			if (GetTypeID() == Instance.GetTypeID())
				return reinterpret_cast<T*>(Instance.GetData());
		}

	private:
		
		static void Destroy(void* pInstance)
		{
			delete reinterpret_cast<T*>(pInstance);
		}
	};

	class Value
	{
	public:
		Value(std::string TypeName, int64_t TypeID, void* pData)
			: m_TypeName(TypeName),
			m_TypeID(TypeID),
			m_pData(pData),
			m_pDestructor(nullptr) 
		{
		}

		Value(std::string TypeName, int64_t TypeID, void* pData, void (pDestructor)(void*))
			: m_TypeName(TypeName),
			m_TypeID(TypeID),
			m_pData(pData),
			m_pDestructor(pDestructor)
		{

		}

		Value(const Value&) = delete;

		Value& operator=(const Value&) = delete;

		~Value()
		{
			if (m_pDestructor)
				m_pDestructor(m_pData);
		}

		int64_t GetTypeID()
		{
			return m_TypeID;
		}

		void* GetData()
		{
			return m_pData;
		}

	private:
		std::string m_TypeName;
		int64_t m_TypeID;
		void* m_pData;

		void (m_pDestructor) (void*);
	}

	class Class
	{
		const std::vector<std::pair<std::string, Value>>& GetVariables()
		{
			return m_Variables;
		}

		Value* GetVariable(const std::string& Name)
		{
			auto Iterator = std::find_if(m_Variables.begin(), m_Variables.end(),
				[Name](const std::pair<std::string, Value>& Current)
				{
					return Current.first == Name;
				}
			);

			return Iterator != m_Variables.end() ? &(Iterator->first) : nullptr;
		}

		bool SetVariable(const std::string& Name, Value NewValue)
		{
			auto Iterator = std::find_if(m_Variables.begin(), m_Variables.end(),
				[Name](const std::pair<std::string, Value>& Current)
				{
					return Current.first == Name;
				}
			);

			if (Iterator != m_Variables.end())
			{

			}
		}

	private:
		std::vector<std::pair<std::string, Value>> m_Variables;
	};
}