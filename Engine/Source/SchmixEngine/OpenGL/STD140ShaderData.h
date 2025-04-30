#pragma once
#include <SchmixEngine/Core/Graphics/Core/ShaderData.h>
#include <SchmixEngine/ThirdParty/GLM/gtc/type_ptr.hpp>
#include <SchmixEngine/Core/Util/Util.h>
#include <unordered_map>

namespace SchmixEngine
{
	class STD140ShaderData : public ShaderData
	{
	public:
		STD140ShaderData() = default;
		STD140ShaderData(std::initializer_list<std::pair<std::string, ShaderDataType::Enum>> Layout);
		STD140ShaderData(const std::vector<std::pair<std::string, ShaderDataType::Enum>>& Layout);

		virtual ~STD140ShaderData() = default;

		virtual size_t GetShaderDataSize();
		virtual void* GetShaderDataPtr();

		virtual void SetShaderDataLayout(const std::vector<std::pair<std::string, ShaderDataType::Enum>>& Layout);
		virtual std::vector<std::pair<std::string, ShaderDataType::Enum>> GetShaderDataLayout();

		virtual void SetBool(const std::string& Name, bool Value);
		virtual bool GetBool(const std::string& Name);

		virtual void SetBool2(const std::string& Name, glm::bvec2 Value);
		virtual glm::bvec2 GetBool2(const std::string& Name);

		virtual void SetBool3(const std::string& Name, glm::bvec3 Value);
		virtual glm::bvec3 GetBool3(const std::string& Name);

		virtual void SetBool4(const std::string& Name, glm::bvec4 Value);
		virtual glm::bvec4 GetBool4(const std::string& Name);

		virtual void SetInt(const std::string& Name, int32_t Value);
		virtual int32_t GetInt(const std::string& Name);

		virtual void SetInt2(const std::string& Name, glm::ivec2 Value);
		virtual glm::ivec2 GetInt2(const std::string& Name);

		virtual void SetInt3(const std::string& Name, glm::ivec3 Value);
		virtual glm::ivec3 GetInt3(const std::string& Name);

		virtual void SetInt4(const std::string& Name, glm::ivec4 Value);
		virtual glm::ivec4 GetInt4(const std::string& Name);

		virtual void SetUInt(const std::string& Name, uint32_t Value);
		virtual uint32_t GetUInt(const std::string& Name);

		virtual void SetUInt2(const std::string& Name, glm::uvec2 Value);
		virtual glm::uvec2 GetUInt2(const std::string& Name);

		virtual void SetUInt3(const std::string& Name, glm::uvec3 Value);
		virtual glm::uvec3 GetUInt3(const std::string& Name);

		virtual void SetUInt4(const std::string& Name, glm::uvec4 Value);
		virtual glm::uvec4 GetUInt4(const std::string& Name);

		virtual void SetFloat(const std::string& Name, float Value);
		virtual float GetFloat(const std::string& Name);

		virtual void SetFloat2(const std::string& Name, glm::fvec2 Value);
		virtual glm::fvec2 GetFloat2(const std::string& Name);

		virtual void SetFloat3(const std::string& Name, glm::fvec3 Value);
		virtual glm::fvec3 GetFloat3(const std::string& Name);

		virtual void SetFloat4(const std::string& Name, glm::fvec4 Value);
		virtual glm::fvec4 GetFloat4(const std::string& Name);

		virtual void SetMat2x2(const std::string& Name, glm::fmat2 Value);
		virtual glm::fmat2 GetMat2x2(const std::string& Name);

		virtual void SetMat3x3(const std::string& Name, glm::fmat3 Value);
		virtual glm::fmat3 GetMat3x3(const std::string& Name);

		virtual void SetMat4x4(const std::string& Name, glm::fmat4 Value);
		virtual glm::fmat4 GetMat4x4(const std::string& Name);

	protected:
		size_t GetShaderDataTypeAlignment(ShaderDataType::Enum Type);

		BufferLayoutElement GetLayoutElement(const std::string& Name);

		std::unordered_map<std::string, BufferLayoutElement> m_BufferLayoutElements;
		std::vector<uint8_t> m_Data;
	};
}