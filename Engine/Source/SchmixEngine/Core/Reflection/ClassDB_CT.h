#pragma once
#include <SchmixEngine/Core/Reflection/ClassInfo.h>

namespace SchmixEngine
{
	class ClassDB_CT
	{
	public:
		template<typename T>
		static inline const ClassInfo* GetClassInfo(bool bSkipInit = false)
		{
			if (HasClassInfo<T>())
			{
				static ClassInfo s_ClassInfo;

				if (!bSkipInit && !s_ClassInfo.IsInitialized())
				{
					s_ClassInfo.SetIsInItialized(InitClassInfo<T>());
				}

				return &s_ClassInfo;
			}

			return nullptr;
		}

		template<typename T>
		static inline bool HasClassInfo()
		{
			return false;
		}

		template<typename T>
		static inline bool InitClassInfo()
		{
			return false;
		}
	};
}

#include <SchmixEngine/Core/Reflection/Primitives.inl>