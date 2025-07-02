#include <SchmixEngine/Core/Reflection/ClassDB_CT.h>
#include <SchmixEngine/Core/Reflection/ReflectionUtil.h>

namespace SchmixEngine
{
	template<>
	inline bool ClassDB_CT::HasClassInfo<uint8_t>() { return true; }

	template<>
	inline bool ClassDB_CT::InitClassInfo<uint8_t>()
	{
		ReflectionUtil::RegisterClassName<uint8_t>("uint8_t");

		ReflectionUtil::RegisterStaticCast<uint8_t, uint16_t>();
		ReflectionUtil::RegisterStaticCast<uint8_t, uint32_t>();
		ReflectionUtil::RegisterStaticCast<uint8_t, uint64_t>();

		ReflectionUtil::RegisterStaticCast<uint8_t, int8_t>();
		ReflectionUtil::RegisterStaticCast<uint8_t, int16_t>();
		ReflectionUtil::RegisterStaticCast<uint8_t, int32_t>();
		ReflectionUtil::RegisterStaticCast<uint8_t, int64_t>();

		ReflectionUtil::RegisterStaticCast<uint8_t, float>();
		ReflectionUtil::RegisterStaticCast<uint8_t, double>();

		return true;
	}

	template<>
	inline bool ClassDB_CT::HasClassInfo<uint16_t>() { return true; }

	template<>
	inline bool ClassDB_CT::InitClassInfo<uint16_t>()
	{
		ReflectionUtil::RegisterClassName<uint16_t>("uint16_t");

		ReflectionUtil::RegisterStaticCast<uint16_t, uint8_t>();
		ReflectionUtil::RegisterStaticCast<uint16_t, uint32_t>();
		ReflectionUtil::RegisterStaticCast<uint16_t, uint64_t>();

		ReflectionUtil::RegisterStaticCast<uint16_t, int8_t>();
		ReflectionUtil::RegisterStaticCast<uint16_t, int16_t>();
		ReflectionUtil::RegisterStaticCast<uint16_t, int32_t>();
		ReflectionUtil::RegisterStaticCast<uint16_t, int64_t>();

		ReflectionUtil::RegisterStaticCast<uint16_t, float>();
		ReflectionUtil::RegisterStaticCast<uint16_t, double>();

		return true;
	}

	template<>
	inline bool ClassDB_CT::HasClassInfo<uint32_t>() { return true; }

	template<>
	inline bool ClassDB_CT::InitClassInfo<uint32_t>()
	{
		ReflectionUtil::RegisterClassName<uint32_t>("uint32_t");

		ReflectionUtil::RegisterStaticCast<uint32_t, uint8_t>();
		ReflectionUtil::RegisterStaticCast<uint32_t, uint16_t>();
		ReflectionUtil::RegisterStaticCast<uint32_t, uint64_t>();

		ReflectionUtil::RegisterStaticCast<uint32_t, int8_t>();
		ReflectionUtil::RegisterStaticCast<uint32_t, int16_t>();
		ReflectionUtil::RegisterStaticCast<uint32_t, int32_t>();
		ReflectionUtil::RegisterStaticCast<uint32_t, int64_t>();

		ReflectionUtil::RegisterStaticCast<uint32_t, float>();
		ReflectionUtil::RegisterStaticCast<uint32_t, double>();

		return true;
	}

	template<>
	inline bool ClassDB_CT::HasClassInfo<uint64_t>() { return true; }

	template<>
	inline bool ClassDB_CT::InitClassInfo<uint64_t>()
	{
		ReflectionUtil::RegisterClassName<uint64_t>("uint64_t");

		ReflectionUtil::RegisterStaticCast<uint64_t, uint8_t>();
		ReflectionUtil::RegisterStaticCast<uint64_t, uint16_t>();
		ReflectionUtil::RegisterStaticCast<uint64_t, uint32_t>();

		ReflectionUtil::RegisterStaticCast<uint64_t, int8_t>();
		ReflectionUtil::RegisterStaticCast<uint64_t, int16_t>();
		ReflectionUtil::RegisterStaticCast<uint64_t, int32_t>();
		ReflectionUtil::RegisterStaticCast<uint64_t, int64_t>();

		ReflectionUtil::RegisterStaticCast<uint64_t, float>();
		ReflectionUtil::RegisterStaticCast<uint64_t, double>();

		return true;
	}

	template<>
	inline bool ClassDB_CT::HasClassInfo<int8_t>() { return true; }

	template<>
	inline bool ClassDB_CT::InitClassInfo<int8_t>()
	{
		ReflectionUtil::RegisterClassName<int8_t>("int8_t");

		ReflectionUtil::RegisterStaticCast<int8_t, int16_t>();
		ReflectionUtil::RegisterStaticCast<int8_t, int32_t>();
		ReflectionUtil::RegisterStaticCast<int8_t, int64_t>();

		ReflectionUtil::RegisterStaticCast<int8_t, uint8_t>();
		ReflectionUtil::RegisterStaticCast<int8_t, uint16_t>();
		ReflectionUtil::RegisterStaticCast<int8_t, uint32_t>();
		ReflectionUtil::RegisterStaticCast<int8_t, uint64_t>();

		ReflectionUtil::RegisterStaticCast<int8_t, float>();
		ReflectionUtil::RegisterStaticCast<int8_t, double>();

		return true;
	}

	template<>
	inline bool ClassDB_CT::HasClassInfo<int16_t>() { return true; }

	template<>
	inline bool ClassDB_CT::InitClassInfo<int16_t>()
	{
		ReflectionUtil::RegisterClassName<int16_t>("int16_t");

		ReflectionUtil::RegisterStaticCast<int16_t, int8_t>();
		ReflectionUtil::RegisterStaticCast<int16_t, int32_t>();
		ReflectionUtil::RegisterStaticCast<int16_t, int64_t>();

		ReflectionUtil::RegisterStaticCast<int16_t, uint8_t>();
		ReflectionUtil::RegisterStaticCast<int16_t, uint16_t>();
		ReflectionUtil::RegisterStaticCast<int16_t, uint32_t>();
		ReflectionUtil::RegisterStaticCast<int16_t, uint64_t>();

		ReflectionUtil::RegisterStaticCast<int16_t, float>();
		ReflectionUtil::RegisterStaticCast<int16_t, double>();

		return true;
	}

	template<>
	inline bool ClassDB_CT::HasClassInfo<int32_t>() { return true; }

	template<>
	inline bool ClassDB_CT::InitClassInfo<int32_t>()
	{
		ReflectionUtil::RegisterClassName<int32_t>("int32_t");

		ReflectionUtil::RegisterStaticCast<int32_t, int8_t>();
		ReflectionUtil::RegisterStaticCast<int32_t, int16_t>();
		ReflectionUtil::RegisterStaticCast<int32_t, int64_t>();

		ReflectionUtil::RegisterStaticCast<int32_t, uint8_t>();
		ReflectionUtil::RegisterStaticCast<int32_t, uint16_t>();
		ReflectionUtil::RegisterStaticCast<int32_t, uint32_t>();
		ReflectionUtil::RegisterStaticCast<int32_t, uint64_t>();

		ReflectionUtil::RegisterStaticCast<int32_t, float>();
		ReflectionUtil::RegisterStaticCast<int32_t, double>();

		return true;
	}

	template<>
	inline bool ClassDB_CT::HasClassInfo<int64_t>() { return true; }

	template<>
	inline bool ClassDB_CT::InitClassInfo<int64_t>()
	{
		ReflectionUtil::RegisterClassName<int64_t>("int64_t");

		ReflectionUtil::RegisterStaticCast<int64_t, int8_t>();
		ReflectionUtil::RegisterStaticCast<int64_t, int16_t>();
		ReflectionUtil::RegisterStaticCast<int64_t, int32_t>();

		ReflectionUtil::RegisterStaticCast<int64_t, uint8_t>();
		ReflectionUtil::RegisterStaticCast<int64_t, uint16_t>();
		ReflectionUtil::RegisterStaticCast<int64_t, uint32_t>();
		ReflectionUtil::RegisterStaticCast<int64_t, uint64_t>();

		ReflectionUtil::RegisterStaticCast<int64_t, float>();
		ReflectionUtil::RegisterStaticCast<int64_t, double>();

		return true;
	}

	template<>
	inline bool ClassDB_CT::HasClassInfo<float>() { return true; }

	template<>
	inline bool ClassDB_CT::InitClassInfo<float>()
	{
		ReflectionUtil::RegisterClassName<float>("float");

		ReflectionUtil::RegisterStaticCast<float, int8_t>();
		ReflectionUtil::RegisterStaticCast<float, int16_t>();
		ReflectionUtil::RegisterStaticCast<float, int32_t>();
		ReflectionUtil::RegisterStaticCast<float, int64_t>();

		ReflectionUtil::RegisterStaticCast<float, uint8_t>();
		ReflectionUtil::RegisterStaticCast<float, uint16_t>();
		ReflectionUtil::RegisterStaticCast<float, uint32_t>();
		ReflectionUtil::RegisterStaticCast<float, uint64_t>();

		ReflectionUtil::RegisterStaticCast<float, double>();

		return true;
	}

	template<>
	inline bool ClassDB_CT::HasClassInfo<double>() { return true; }

	template<>
	inline bool ClassDB_CT::InitClassInfo<double>()
	{
		ReflectionUtil::RegisterClassName<double>("double");

		ReflectionUtil::RegisterStaticCast<double, int8_t>();
		ReflectionUtil::RegisterStaticCast<double, int16_t>();
		ReflectionUtil::RegisterStaticCast<double, int32_t>();
		ReflectionUtil::RegisterStaticCast<double, int64_t>();

		ReflectionUtil::RegisterStaticCast<double, uint8_t>();
		ReflectionUtil::RegisterStaticCast<double, uint16_t>();
		ReflectionUtil::RegisterStaticCast<double, uint32_t>();
		ReflectionUtil::RegisterStaticCast<double, uint64_t>();

		ReflectionUtil::RegisterStaticCast<double, double>();

		return true;
	}

	template<>
	inline bool ClassDB_CT::HasClassInfo<std::string>() { return true; }

	template<>
	inline bool ClassDB_CT::InitClassInfo<std::string>()
	{
		ReflectionUtil::RegisterClassName<std::string>("string");

		return true;
	}
}
