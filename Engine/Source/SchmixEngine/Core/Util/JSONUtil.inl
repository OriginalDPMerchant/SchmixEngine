#include <SchmixEngine/Core/Util/JSONUtil.h>

namespace SchmixEngine
{
	template<typename T>
	inline T JSONUtil::ReadJSON_Value(rapidjson::Value& ParentObject, const std::string& ParentName, const std::string& MemberName, bool* pbWasSuccessful)
	{
		if (ParentObject.HasMember(MemberName.c_str()))
		{
			const rapidjson::Value& Value = ParentObject[MemberName.c_str()];

			if (RapidJSONValue_Is<T>(Value))
			{
				if (pbWasSuccessful)
					*pbWasSuccessful = true;

				return RapidJSONValue_Get<T>(Value);
			}
			else
			{
				std::string Msg = "Expeced Member of \"" + ParentName + "\", Named \"" + MemberName + "\" To be of Type \"" +
					JSONUtil::TypeToString<T>() + "\" But the Actual Type of The Member is \"" + JSONUtil::GetRapidJSONValueTypeString(Value) + "\"";

				SMX_LOG_WSRC(Msg, "SchmixEngine::JSONUtil::ReadJSON_Value", LogLevel::Error);
			}
		}
		else
		{
			std::string Msg = "JSON Object \"" + ParentName + "\" Has No Member Named \"" + MemberName + "\"";
			SMX_LOG_WSRC(Msg, "SchmixEngine::JSONUtil::ReadJSON_Value", LogLevel::Error);
		}

		if (pbWasSuccessful)
			*pbWasSuccessful = false;

		return T();
	}

	template<typename T, glm::length_t L>
	inline glm::vec<L, T> JSONUtil::ReadJSON_Vector(rapidjson::Value& ParentObject, const std::string& ParentName, const std::string& MemberName, bool* pbWasSuccessful)
	{
		if (ParentObject.HasMember(MemberName.c_str()))
		{
			rapidjson::Value& Member = ParentObject[MemberName.c_str()];

			if (Member.IsArray())
			{
				if (Member.Size() == L)
				{
					if (pbWasSuccessful)
						*pbWasSuccessful = true;

					glm::vec<L, T> Vector{};

					for (glm::length_t i = 0; i < L; i++)
					{
						if (RapidJSONValue_Is<T>(Member[i]))
						{
							Vector[i] = RapidJSONValue_Get<T>(Member[i]);
						}
						else
						{
							if(pbWasSuccessful)
								*pbWasSuccessful = false;

							std::string Msg = "Expeced Member of \"" + ParentName + "\", Named \"" + MemberName + "[" + std::to_string(i) + "]\" To be of Type \"" +
								JSONUtil::TypeToString<T>() + "\" But the Actual Type of The Member is \"" + JSONUtil::GetRapidJSONValueTypeString(Member[i]) + "\"";

							SMX_LOG_WSRC(Msg, "SchmixEngine::JSONUtil::ReadJSON_Vector", LogLevel::Error);
						}
					}

					return Vector;
				}
				else
				{
					std::string Msg = "Expeced Member of \"" + ParentName + "\", Named \"" + MemberName + "\" To be an Array of Size \"" +
						std::to_string(L) + "\" But the Actual Size of The Array is \"" + std::to_string(Member.Size()) + "\"";

					SMX_LOG_WSRC(Msg, "SchmixEngine::JSONUtil::ReadJSON_Vector", LogLevel::Error);
				}
			}
			else
			{
				std::string Msg = "Expeced Member of \"" + ParentName + "\", Named \"" + MemberName + "\" To be an Array"
					+ " But the Actual Type of The Member is \"" + JSONUtil::GetRapidJSONValueTypeString(Member) + "\"";

				SMX_LOG_WSRC(Msg, "SchmixEngine::JSONUtil::ReadJSON_Vector", LogLevel::Error);
			}
		}
		else
		{
			std::string Msg = "JSON Object \"" + ParentName + "\" Has No Member Named \"" + MemberName + "\"";
			SMX_LOG_WSRC(Msg, "SchmixEngine::JSONUtil::ReadJSON_Vector", LogLevel::Error);
		}

		if (pbWasSuccessful)
			*pbWasSuccessful = false;

		return glm::vec<L, T>();
	}

	template<typename T, typename S>
	inline S JSONUtil::ReadJSON_Enum(const std::string& EnumName, rapidjson::Value& ParentObject, const std::string& ParentName, const std::string& MemberName, bool* pbWasSuccessful)
	{
		bool bSuccess = true;
		S Enum = T::Unknown;

		std::string EnumAsString = ReadJSON_Value<std::string>(ParentObject, ParentName, MemberName, &bSuccess);

		if (bSuccess)
		{
			Enum = T::FromString(EnumAsString);

			if (Enum == T::Unknown)
			{
				std::string Msg = "\"" + EnumAsString + "\" is an Invalid Value for Enum of Type \"" + EnumName + "\"";
				SMX_LOG_WSRC(Msg, "SchmixEngine::JSONUtil::ReadJSON_Value", LogLevel::Error);

				bSuccess = false;
			}
		}

		if (pbWasSuccessful)
			*pbWasSuccessful = bSuccess;

		return Enum;
	}

	template<typename T>
	inline std::string JSONUtil::TypeToString()
	{
		static_assert
			(
				std::is_same_v<T, bool>
				|| std::is_same_v<T, uint32_t>
				|| std::is_same_v<T, int32_t>
				|| std::is_same_v<T, uint64_t>
				|| std::is_same_v<T, int64_t>
				|| std::is_same_v<T, float>
				|| std::is_same_v<T, double>
				|| std::is_same_v<T, std::string>,
				"Template Argument for JSONUtil::TypeToString() must be one of: bool, uint32_t, int32_t, uint64_t, int64_t, float, double or std::string"
				);

		if constexpr (std::is_same_v<T, bool>)
			return "Boolean";
		else if constexpr (std::is_same_v<T, uint32_t>)
			return "32-Bit Unsigned Integer";
		else if constexpr (std::is_same_v<T, int32_t>)
			return "32-Bit Signed Integer";
		else if constexpr (std::is_same_v<T, uint64_t>)
			return "64-Bit Unsigned Integer";
		else if constexpr (std::is_same_v<T, int64_t>)
			return "64-Bit Signed Integer";
		else if constexpr (std::is_same_v<T, float>)
			return "Single Percision Floating Point Number";
		else if constexpr (std::is_same_v<T, double>)
			return "Double Percision Floating Point Number";
		else if constexpr (std::is_same_v<T, std::string>)
			return "String";
	}

	template<typename T>
	inline bool JSONUtil::RapidJSONValue_Is(const rapidjson::Value& Value)
	{
		static_assert
			(
				std::is_same_v<T, bool>
				|| std::is_same_v<T, uint32_t>
				|| std::is_same_v<T, int32_t>
				|| std::is_same_v<T, uint64_t>
				|| std::is_same_v<T, int64_t>
				|| std::is_same_v<T, float>
				|| std::is_same_v<T, double>
				|| std::is_same_v<T, std::string>,
				"Template Argument for JSONUtil::RapidJSONValue_Is() must be one of: bool, uint32_t, int32_t, uint64_t, int64_t, float, double or std::string"
				);

		if constexpr (std::is_same_v<T, bool>)
			return Value.IsBool();
		else if constexpr (std::is_same_v<T, uint32_t>)
			return Value.IsUint();
		else if constexpr (std::is_same_v<T, int32_t>)
			return Value.IsInt();
		else if constexpr (std::is_same_v<T, uint64_t>)
			return Value.IsUint64() || Value.UIsInt64();
		else if constexpr (std::is_same_v<T, int64_t>)
			return Value.IsInt64() || Value.IsInt();
		else if constexpr (std::is_same_v<T, float>)
			return Value.IsFloat() || Value.IsUint() || Value.IsInt() || Value.IsUint64() || Value.IsInt64();
		else if constexpr (std::is_same_v<T, double>)
			return Value.IsDouble() || Value.IsFloat() || Value.IsUint() || Value.IsInt() || Value.IsUint64() || Value.IsInt64();
		else if constexpr (std::is_same_v<T, std::string>)
			return Value.IsString();
		else
			return false;
	}

	template<typename T>
	inline T JSONUtil::RapidJSONValue_Get(const rapidjson::Value& Value)
	{
		static_assert
			(
				std::is_same_v<T, bool>
				|| std::is_same_v<T, uint32_t>
				|| std::is_same_v<T, int32_t>
				|| std::is_same_v<T, uint64_t>
				|| std::is_same_v<T, int64_t>
				|| std::is_same_v<T, float>
				|| std::is_same_v<T, double>
				|| std::is_same_v<T, std::string>,
				"Template Argument for JSONUtil::RapidJSONValue_Get() must be one of: bool, uint32_t, int32_t, uint64_t, int64_t, float, double or std::string"
				);

		if constexpr (std::is_same_v<T, bool>)
			return Value.GetBool();
		else if constexpr (std::is_same_v<T, uint32_t>)
			return Value.GetUint();
		else if constexpr (std::is_same_v<T, int32_t>)
			return Value.GetInt();
		else if constexpr (std::is_same_v<T, uint64_t>)
			return Value.GetUint64();
		else if constexpr (std::is_same_v<T, int64_t>)
			return Value.GetInt64();
		else if constexpr (std::is_same_v<T, float>)
			return Value.GetFloat();
		else if constexpr (std::is_same_v<T, double>)
			return Value.GetDouble();
		else if constexpr (std::is_same_v<T, std::string>)
			return Value.GetString();
	}


	inline std::string JSONUtil::GetRapidJSONValueTypeString(const rapidjson::Value& Value)
	{
		if (Value.IsBool())
			return JSONUtil::TypeToString<bool>();
		else if (Value.IsInt())
			return JSONUtil::TypeToString<int32_t>();
		else if (Value.IsUint())
			return JSONUtil::TypeToString<uint32_t>();
		else if (Value.IsInt64())
			return JSONUtil::TypeToString<int64_t>();
		else if (Value.IsUint64())
			return JSONUtil::TypeToString<uint64_t>();
		else if (Value.IsFloat())
			return JSONUtil::TypeToString<float>();
		else if (Value.IsDouble())
			return JSONUtil::TypeToString<double>();
		else if (Value.IsString())
			return JSONUtil::TypeToString<std::string>();
		else
			return std::string();
	}
}