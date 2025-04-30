#pragma once
#include <SchmixEngine/Core/Graphics/Core/Buffer.h>
#include <SchmixEngine/ThirdParty/GLM/glm.hpp>
#include <string>

namespace SchmixEngine
{
	typedef std::pair<std::string, ShaderDataType::Enum> ShaderParameter;

	class ShaderData
	{
	public:
		ShaderData() = default;
		virtual ~ShaderData() = default;

		virtual size_t GetShaderDataSize() = 0;
		virtual void* GetShaderDataPtr() = 0;

		virtual void SetShaderDataLayout(const std::vector<ShaderParameter>& Layout) = 0;
		virtual std::vector<ShaderParameter> GetShaderDataLayout() = 0;

		virtual void SetBool(const std::string& Name, bool Value) = 0;
		virtual bool GetBool(const std::string& Name) = 0;

		virtual void SetBool2(const std::string& Name, glm::bvec2 Value) = 0;
		virtual glm::bvec2 GetBool2(const std::string& Name) = 0;

		virtual void SetBool3(const std::string& Name, glm::bvec3 Value) = 0;
		virtual glm::bvec3 GetBool3(const std::string& Name) = 0;

		virtual void SetBool4(const std::string& Name, glm::bvec4 Value) = 0;
		virtual glm::bvec4 GetBool4(const std::string& Name) = 0;

		virtual void SetInt(const std::string& Name, int32_t Value) = 0;
		virtual int32_t GetInt(const std::string& Name) = 0;

		virtual void SetInt2(const std::string& Name, glm::ivec2 Value) = 0;
		virtual glm::ivec2 GetInt2(const std::string& Name) = 0;

		virtual void SetInt3(const std::string& Name, glm::ivec3 Value) = 0;
		virtual glm::ivec3 GetInt3(const std::string& Name) = 0;

		virtual void SetInt4(const std::string& Name, glm::ivec4 Value) = 0;
		virtual glm::ivec4 GetInt4(const std::string& Name) = 0;

		virtual void SetUInt(const std::string& Name, uint32_t Value) = 0;
		virtual uint32_t GetUInt(const std::string& Name) = 0;

		virtual void SetUInt2(const std::string& Name, glm::uvec2 Value) = 0;
		virtual glm::uvec2 GetUInt2(const std::string& Name) = 0;

		virtual void SetUInt3(const std::string& Name, glm::uvec3 Value) = 0;
		virtual glm::uvec3 GetUInt3(const std::string& Name) = 0;

		virtual void SetUInt4(const std::string& Name, glm::uvec4 Value) = 0;
		virtual glm::uvec4 GetUInt4(const std::string& Name) = 0;

		virtual void SetFloat(const std::string& Name, float Value) = 0;
		virtual float GetFloat(const std::string& Name) = 0;

		virtual void SetFloat2(const std::string& Name, glm::fvec2 Value) = 0;
		virtual glm::fvec2 GetFloat2(const std::string& Name) = 0;

		virtual void SetFloat3(const std::string& Name, glm::fvec3 Value) = 0;
		virtual glm::fvec3 GetFloat3(const std::string& Name) = 0;

		virtual void SetFloat4(const std::string& Name, glm::fvec4 Value) = 0;
		virtual glm::fvec4 GetFloat4(const std::string& Name) = 0;

		virtual void SetMat2x2(const std::string& Name, glm::fmat2 Value) = 0;
		virtual glm::fmat2 GetMat2x2(const std::string& Name) = 0;

		virtual void SetMat3x3(const std::string& Name, glm::fmat3 Value) = 0;
		virtual glm::fmat3 GetMat3x3(const std::string& Name) = 0;

		virtual void SetMat4x4(const std::string& Name, glm::fmat4 Value) = 0;
		virtual glm::fmat4 GetMat4x4(const std::string& Name) = 0;
	};
}