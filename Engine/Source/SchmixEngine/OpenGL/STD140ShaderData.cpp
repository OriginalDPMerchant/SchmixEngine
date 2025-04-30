#include <SchmixEngine/OpenGL/STD140ShaderData.h>

namespace SchmixEngine
{
	STD140ShaderData::STD140ShaderData(std::initializer_list<std::pair<std::string, ShaderDataType::Enum>> Layout)
	{
		STD140ShaderData::SetShaderDataLayout(std::vector<std::pair<std::string, ShaderDataType::Enum>>(Layout));
	}

	STD140ShaderData::STD140ShaderData(const std::vector<std::pair<std::string, ShaderDataType::Enum>>& Layout)
	{
		STD140ShaderData::SetShaderDataLayout(Layout);
	}

	size_t STD140ShaderData::GetShaderDataSize()
	{
		return m_Data.size();
	}

	void* STD140ShaderData::GetShaderDataPtr()
	{
		return m_Data.data();
	}

	void STD140ShaderData::SetShaderDataLayout(const std::vector<std::pair<std::string, ShaderDataType::Enum>>& Layout)
	{
		m_BufferLayoutElements.clear();
		size_t Stride = 0;

		for (size_t i = 0; i < Layout.size(); i++)
		{
			if (m_BufferLayoutElements.find(Layout[i].first) == m_BufferLayoutElements.end())
			{
				const uint32_t Alignment = GetShaderDataTypeAlignment(Layout[i].second);

				//Compute Aligned Offset
				const uint32_t AlignedOffset = Util::AlignOffset(Stride, Alignment);//Stride % Alignment == 0 ? Stride : Stride + (Alignment - (Stride % Alignment));

				BufferLayoutElement Element;
				Element.Type = Layout[i].second;
				Element.Size = ShaderDataType::GetSize(Layout[i].second);

				//Set The Offset of the Element to be the aligned offset
				Element.Offset = AlignedOffset;

				//Add the Size of the Element in the Buffer to the Offset, 
				//for Matrix2x2 the size it will take up in the buffer is the size of 2 Float4's
				//for Matrix3x3 the size it will take up in the buffer is the size of 3 Float4's

				switch (Element.Type)
				{
				case ShaderDataType::Mat2x2:
					Stride = AlignedOffset + (ShaderDataType::GetSize(ShaderDataType::Float4) * 2);
					break;
				case ShaderDataType::Mat3x3:
					Stride = AlignedOffset + (ShaderDataType::GetSize(ShaderDataType::Float4) * 3);
					break;
				default:
					Stride = AlignedOffset + Element.Size;
				}

				//Add this to our ShaderDataLayout hashmap
				m_BufferLayoutElements.insert(std::pair<std::string, BufferLayoutElement>(Layout[i].first, Element));
			}
		}

		m_Data.resize(Stride, 0);
	}

	std::vector<std::pair<std::string, ShaderDataType::Enum>> STD140ShaderData::GetShaderDataLayout()
	{
		return std::vector<std::pair<std::string, ShaderDataType::Enum>>();
	}

	void STD140ShaderData::SetBool(const std::string& Name, bool Data)
	{
		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Bool)
		{
			uint32_t IntData = static_cast<uint32_t>(Data);

			memcpy(m_Data.data() + Element.Offset, &IntData, Element.Size);
		}
	}

	void STD140ShaderData::SetBool2(const std::string& Name, glm::bvec2 Data)
	{
		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Bool2)
		{
			glm::ivec2 IntData = Data;

			memcpy(m_Data.data() + Element.Offset, glm::value_ptr(IntData), Element.Size);
		}
	}

	void STD140ShaderData::SetBool3(const std::string& Name, glm::bvec3 Data)
	{
		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Bool3)
		{
			glm::ivec3 IntData = Data;

			memcpy(m_Data.data() + Element.Offset, glm::value_ptr(IntData), Element.Size);
		}
	}

	void STD140ShaderData::SetBool4(const std::string& Name, glm::bvec4 Data)
	{
		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Bool4)
		{
			glm::ivec4 IntData = Data;

			memcpy(m_Data.data() + Element.Offset, glm::value_ptr(IntData), Element.Size);
		}
	}

	bool STD140ShaderData::GetBool(const std::string& Name)
	{
		uint32_t Data = 0;

		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Bool)
		{
			memcpy(&Data, m_Data.data() + Element.Offset, Element.Size);
		}

		return static_cast<bool>(Data);
	}

	glm::bvec2 STD140ShaderData::GetBool2(const std::string& Name)
	{
		glm::ivec2 Data = glm::ivec2(0);

		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Bool2)
		{
			memcpy(glm::value_ptr(Data), m_Data.data() + Element.Offset, Element.Size);
		}

		return static_cast<glm::bvec2>(Data);
	}

	glm::bvec3 STD140ShaderData::GetBool3(const std::string& Name)
	{
		glm::ivec3 Data = glm::ivec3(0);

		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Bool3)
		{
			memcpy(glm::value_ptr(Data), m_Data.data() + Element.Offset, Element.Size);
		}

		return static_cast<glm::bvec3>(Data);
	}

	glm::bvec4 STD140ShaderData::GetBool4(const std::string& Name)
	{
		glm::ivec4 Data = glm::ivec4(0);

		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Bool4)
		{
			memcpy(glm::value_ptr(Data), m_Data.data() + Element.Offset, Element.Size);
		}

		return static_cast<glm::bvec4>(Data);
	}

	void STD140ShaderData::SetInt(const std::string& Name, int32_t Data)
	{
		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Int)
		{
			memcpy(m_Data.data() + Element.Offset, &Data, Element.Size);
		}
	}

	void STD140ShaderData::SetInt2(const std::string& Name, glm::ivec2 Data)
	{
		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Int2)
		{
			memcpy(m_Data.data() + Element.Offset, glm::value_ptr(Data), Element.Size);
		}
	}

	void STD140ShaderData::SetInt3(const std::string& Name, glm::ivec3 Data)
	{
		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Int3)
		{
			memcpy(m_Data.data() + Element.Offset, glm::value_ptr(Data), Element.Size);
		}
	}

	void STD140ShaderData::SetInt4(const std::string& Name, glm::ivec4 Data)
	{
		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Int4)
		{
			memcpy(m_Data.data() + Element.Offset, glm::value_ptr(Data), Element.Size);
		}
	}

	int32_t STD140ShaderData::GetInt(const std::string& Name)
	{
		int32_t Data = 0;

		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Int)
		{
			memcpy(&Data, m_Data.data() + Element.Offset, Element.Size);
		}

		return Data;
	}

	glm::ivec2 STD140ShaderData::GetInt2(const std::string& Name)
	{
		glm::ivec2 Data = glm::ivec2(0);

		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Int2)
		{
			memcpy(glm::value_ptr(Data), m_Data.data() + Element.Offset, Element.Size);
		}

		return Data;
	}

	glm::ivec3 STD140ShaderData::GetInt3(const std::string& Name)
	{
		glm::ivec3 Data = glm::ivec3(0);

		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Int3)
		{
			memcpy(glm::value_ptr(Data), m_Data.data() + Element.Offset, Element.Size);
		}

		return Data;
	}

	glm::ivec4 STD140ShaderData::GetInt4(const std::string& Name)
	{
		glm::ivec4 Data = glm::ivec4(0);

		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Int4)
		{
			memcpy(glm::value_ptr(Data), m_Data.data() + Element.Offset, Element.Size);
		}

		return Data;
	}

	void STD140ShaderData::SetUInt(const std::string& Name, uint32_t Data)
	{
		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::UInt)
		{
			memcpy(m_Data.data() + Element.Offset, &Data, Element.Size);
		}
	}

	void STD140ShaderData::SetUInt2(const std::string& Name, glm::uvec2 Data)
	{
		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::UInt2)
		{
			memcpy(m_Data.data() + Element.Offset, glm::value_ptr(Data), Element.Size);
		}
	}

	void STD140ShaderData::SetUInt3(const std::string& Name, glm::uvec3 Data)
	{
		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::UInt3)
		{
			memcpy(m_Data.data() + Element.Offset, glm::value_ptr(Data), Element.Size);
		}
	}

	void STD140ShaderData::SetUInt4(const std::string& Name, glm::uvec4 Data)
	{
		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::UInt4)
		{
			memcpy(m_Data.data() + Element.Offset, glm::value_ptr(Data), Element.Size);
		}
	}

	uint32_t STD140ShaderData::GetUInt(const std::string& Name)
	{
		uint32_t Data = 0;

		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::UInt)
		{
			memcpy(&Data, m_Data.data() + Element.Offset, Element.Size);
		}

		return Data;
	}

	glm::uvec2 STD140ShaderData::GetUInt2(const std::string& Name)
	{
		glm::uvec2 Data = glm::uvec2(0);

		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::UInt2)
		{
			memcpy(glm::value_ptr(Data), m_Data.data() + Element.Offset, Element.Size);
		}

		return Data;
	}

	glm::uvec3 STD140ShaderData::GetUInt3(const std::string& Name)
	{
		glm::uvec3 Data = glm::uvec3(0);

		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::UInt3)
		{
			memcpy(glm::value_ptr(Data), m_Data.data() + Element.Offset, Element.Size);
		}

		return Data;
	}

	glm::uvec4 STD140ShaderData::GetUInt4(const std::string& Name)
	{
		glm::uvec4 Data = glm::uvec4(0);

		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::UInt4)
		{
			memcpy(glm::value_ptr(Data), m_Data.data() + Element.Offset, Element.Size);
		}

		return Data;
	}

	void STD140ShaderData::SetFloat(const std::string& Name, float Data)
	{
		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Float)
		{
			memcpy(m_Data.data() + Element.Offset, &Data, Element.Size);
		}
	}

	void STD140ShaderData::SetFloat2(const std::string& Name, glm::vec2 Data)
	{
		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Float2)
		{
			memcpy(m_Data.data() + Element.Offset, glm::value_ptr(Data), Element.Size);
		}
	}

	void STD140ShaderData::SetFloat3(const std::string& Name, glm::vec3 Data)
	{
		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Float3)
		{
			memcpy(m_Data.data() + Element.Offset, glm::value_ptr(Data), Element.Size);
		}
	}

	void STD140ShaderData::SetFloat4(const std::string& Name, glm::vec4 Data)
	{
		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Float4)
		{
			memcpy(m_Data.data() + Element.Offset, glm::value_ptr(Data), Element.Size);
		}
	}

	float STD140ShaderData::GetFloat(const std::string& Name)
	{
		float Data = 0.0f;

		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Float)
		{
			memcpy(&Data, m_Data.data() + Element.Offset, Element.Size);
		}

		return Data;
	}

	glm::vec2 STD140ShaderData::GetFloat2(const std::string& Name)
	{
		glm::vec2 Data = glm::vec2(0.0f);

		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Float2)
		{
			memcpy(glm::value_ptr(Data), m_Data.data() + Element.Offset, Element.Size);
		}

		return Data;
	}

	glm::vec3 STD140ShaderData::GetFloat3(const std::string& Name)
	{
		glm::vec3 Data = glm::vec3(0.0f);

		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Float3)
		{
			memcpy(glm::value_ptr(Data), m_Data.data() + Element.Offset, Element.Size);
		}

		return Data;
	}

	glm::vec4 STD140ShaderData::GetFloat4(const std::string& Name)
	{
		glm::vec4 Data = glm::vec4(0.0f);

		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Float4)
		{
			memcpy(glm::value_ptr(Data), m_Data.data() + Element.Offset, Element.Size);
		}

		return Data;
	}

	void STD140ShaderData::SetMat2x2(const std::string& Name, glm::mat2 Data)
	{
		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Mat2x2)
		{
			const uint32_t SizeOfVec2 = ShaderDataType::GetSize(ShaderDataType::Float2);
			uint32_t Offset = Element.Offset;

			glm::vec2 Row;

			for (int i = 0; i < 2; i++)
			{
				Row = Data[i];

				memcpy(m_Data.data() + Offset, glm::value_ptr(Row), SizeOfVec2);

				Offset += ShaderDataType::GetSize(ShaderDataType::Float4);
			}
		}
	}

	void STD140ShaderData::SetMat3x3(const std::string& Name, glm::mat3 Data)
	{
		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Mat3x3)
		{
			const uint32_t SizeOfVec3 = ShaderDataType::GetSize(ShaderDataType::Float3);
			uint32_t Offset = Element.Offset;

			glm::vec3 Row;

			for (int i = 0; i < 3; i++)
			{
				Row = Data[i];

				memcpy(m_Data.data() + Offset, glm::value_ptr(Row), SizeOfVec3);

				Offset += ShaderDataType::GetSize(ShaderDataType::Float4);
			}
		}
	}

	void STD140ShaderData::SetMat4x4(const std::string& Name, glm::mat4 Data)
	{
		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Mat4x4)
		{
			memcpy(m_Data.data() + Element.Offset, glm::value_ptr(Data), Element.Size);
		}
	}

	glm::mat2 STD140ShaderData::GetMat2x2(const std::string& Name)
	{
		glm::mat2 Data = glm::mat2(0.0f);

		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Mat2x2)
		{
			const uint32_t SizeOfVec2 = ShaderDataType::GetSize(ShaderDataType::Float2);
			uint32_t Offset = Element.Offset;

			glm::vec2 Row;

			for (int i = 0; i < 2; i++)
			{
				memcpy(glm::value_ptr(Row), m_Data.data() + Offset, SizeOfVec2);

				Data[i] = Row;

				Offset += ShaderDataType::GetSize(ShaderDataType::Float4);
			}
		}

		return Data;
	}

	glm::mat3 STD140ShaderData::GetMat3x3(const std::string& Name)
	{
		glm::mat3 Data = glm::mat3(0.0f);

		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Mat3x3)
		{
			const uint32_t SizeOfVec3 = ShaderDataType::GetSize(ShaderDataType::Float3);
			uint32_t Offset = Element.Offset;

			glm::vec3 Row;

			for (int i = 0; i < 3; i++)
			{
				memcpy(glm::value_ptr(Row), m_Data.data() + Offset, SizeOfVec3);

				Data[i] = Row;

				Offset += ShaderDataType::GetSize(ShaderDataType::Float4);
			}
		}

		return Data;
	}

	glm::mat4 STD140ShaderData::GetMat4x4(const std::string& Name)
	{
		glm::mat4 Data = glm::mat4(0.0f);

		BufferLayoutElement Element = GetLayoutElement(Name);

		if (Element.Type == ShaderDataType::Mat4x4)
		{
			memcpy(glm::value_ptr(Data), m_Data.data() + Element.Offset, Element.Size);
		}

		return Data;
	}

	size_t STD140ShaderData::GetShaderDataTypeAlignment(ShaderDataType::Enum Type)
	{
		switch (Type)
		{
		case ShaderDataType::Bool:
			return 4;
		case ShaderDataType::Bool2:
			return 4 * 2;
		case ShaderDataType::Bool3:
			return 4 * 4;
		case ShaderDataType::Bool4:
			return 4 * 4;
		case ShaderDataType::Int:
			return 4;
		case ShaderDataType::Int2:
			return 4 * 2;
		case ShaderDataType::Int3:
			return 4 * 4;
		case ShaderDataType::Int4:
			return 4 * 4;
		case ShaderDataType::UInt:
			return 4;
		case ShaderDataType::UInt2:
			return 4 * 2;
		case ShaderDataType::UInt3:
			return 4 * 4;
		case ShaderDataType::UInt4:
			return 4 * 4;
		case ShaderDataType::Float:
			return 4;
		case ShaderDataType::Float2:
			return 4 * 2;
		case ShaderDataType::Float3:
			return 4 * 4;
		case ShaderDataType::Float4:
			return 4 * 4;
		case ShaderDataType::Mat2x2:
			return 4 * 2;
		case ShaderDataType::Mat3x3:
			return 4 * 4;
		case ShaderDataType::Mat4x4:
			return 4 * 4;
		default:
			return 0;
		}
	}

	BufferLayoutElement STD140ShaderData::GetLayoutElement(const std::string& Name)
	{
		auto Iterator = m_BufferLayoutElements.find(Name);

		if (Iterator != m_BufferLayoutElements.end())
			return Iterator->second;
		else
			return { ShaderDataType::Unknown };
	}
}