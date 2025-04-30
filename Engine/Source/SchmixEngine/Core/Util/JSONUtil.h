#pragma once
#include <SchmixEngine/Core/Debugging/Debug.h>
#include <SchmixEngine/ThirdParty/rapidjson/document.h>
#include <SchmixEngine/ThirdParty/GLM/gtc/type_ptr.hpp>
#include <string>
#include <type_traits>

namespace SchmixEngine
{
	class JSONUtil
	{
	public:
		JSONUtil() = delete;

		JSONUtil(const JSONUtil&) = delete;
		JSONUtil& operator=(const JSONUtil&) = delete;

		template<typename T>
		static T ReadJSON_Value(rapidjson::Value& ParentObject, const std::string& ParentName, const std::string& MemberName, bool* pbWasSuccessful);

		template<typename T, glm::length_t L>
		static glm::vec<L, T> ReadJSON_Vector(rapidjson::Value& ParentObject, const std::string& ParentName, const std::string& MemberName, bool* pbWasSuccessful);

		template<typename T, typename S = T::Enum>
		static S ReadJSON_Enum(const std::string& EnumName, rapidjson::Value& ParentObject, const std::string& ParentName, const std::string& MemberName, bool* pbWasSuccessful);

	private:
		template<typename T>
		static std::string TypeToString();

		template<typename T>
		static bool RapidJSONValue_Is(const rapidjson::Value& Value);

		template<typename T>
		static T RapidJSONValue_Get(const rapidjson::Value& Value);

		static std::string GetRapidJSONValueTypeString(const rapidjson::Value& Value);
	};
}

#include <SchmixEngine/Core/Util/JSONUtil.inl>