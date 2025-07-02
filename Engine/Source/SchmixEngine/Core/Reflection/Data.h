#pragma once
#include <SchmixEngine/Core/Reflection/ClassDB_CT.h>
#include <SchmixEngine/Core/Debugging/Debug.h>
#include <optional>

namespace SchmixEngine
{
	enum class DataType : uint8_t
	{
		Unknown,
		Value,
		Ptr,
		SharedPtr,
		WeakPtr
	};

	//TODO: IMPLEMENT FOR SHARED AND WEAK POINTERS
	//TODO: ALLOW FOR GETVALUE TO CAST BETWEEN DEFINED VALID CASTS
	//TODO: ALLOW FOR GETPTR GETSHAREDPTR AND GETWEAKPTR TO RETURN POINTERS OF SUB AND SUPER CLASSES.

	class Data
	{
	public:
		Data();

		Data(const Data& Other);

		Data& operator=(const Data& Other);

		Data(Data&& Other);

		Data& operator=(Data&& Other);

		~Data();

		Data GetProperty(const std::string& PropertyName);

		void SetProperty(const std::string& PropertyName, const Data& Value);

		template<typename T>
		static Data Value(const T& Value);

		template<typename T>
		static Data Ptr(T* Ptr);

		//template<typename T>
		//Data SharedPtr(const std::shared_ptr<T>& Value);

		//template<typename T>
		//Data SharedPtr(const T& Value);

		//template<typename T>
		//Data WeakPtrWrapper(const std::shared_ptr<T>& Value);

		template<typename T>
		std::optional<T> GetValue() const;

		template<typename T>
		T* GetPtr();

		template<typename T>
		const T* GetPtr() const;

	private:
		typedef void (*DestroyFunction) (void*);
		typedef void* (*CopyFunction) (void*);

		Data(DataType Type, const ClassInfo* pClassInfo, void* pData, DestroyFunction pDestructor, CopyFunction pCopyFunction);

		void CleanUp();

		template<typename T>
		static void Destroy(void* ptr);

		template<typename T>
		static void* Copy(void* ptr);

		DataType m_DataType;
		const ClassInfo* m_pClassInfo;
		
		void* m_pData;
		DestroyFunction m_pDestructor;
		CopyFunction m_pCopyFunction;
	};
}

#include <SchmixEngine/Core/Reflection/Data.inl>