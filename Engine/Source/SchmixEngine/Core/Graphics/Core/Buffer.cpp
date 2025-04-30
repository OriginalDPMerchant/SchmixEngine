#include <SchmixEngine/Core/Graphics/Core/Buffer.h>

namespace SchmixEngine
{
	PackedBufferLayout::PackedBufferLayout()
		: m_Stride(0)
	{
	}

	PackedBufferLayout::PackedBufferLayout(std::initializer_list<ShaderDataType::Enum> Layout)
		: m_Stride(0)
	{
		std::vector<ShaderDataType::Enum> ShaderDataTypes(Layout);

		SetBufferLayout(ShaderDataTypes);
	}

	PackedBufferLayout::PackedBufferLayout(std::vector<ShaderDataType::Enum>& Layout)
		: m_Stride(0)
	{
		SetBufferLayout(Layout);
	}

	void PackedBufferLayout::SetBufferLayout(std::vector<ShaderDataType::Enum>& Layout)
	{
		m_Stride = 0;
		m_BufferLayoutElements.clear();

		for (ShaderDataType::Enum DataType : Layout)
		{
			BufferLayoutElement Element;
			Element.Type = DataType;
			Element.Size = ShaderDataType::GetSize(DataType);
			Element.Offset = m_Stride;
			Element.Count = ShaderDataType::GetCount(DataType);

			m_Stride += Element.Size;

			m_BufferLayoutElements.push_back(Element);
		}
	}

	const std::vector<BufferLayoutElement>& PackedBufferLayout::GetBufferLayoutElements()
	{
		return m_BufferLayoutElements;
	}

	size_t PackedBufferLayout::GetStride()
	{
		return m_Stride;
	}

	size_t ShaderDataType::GetSize(ShaderDataType::Enum DataType)
	{
		switch (DataType)
		{
		case ShaderDataType::Bool:
			return 4;
		case ShaderDataType::Bool2:
			return 4 * 2;
		case ShaderDataType::Bool3:
			return 4 * 3;
		case ShaderDataType::Bool4:
			return 4 * 4;
		case ShaderDataType::Int:
			return 4;
		case ShaderDataType::Int2:
			return 4 * 2;
		case ShaderDataType::Int3:
			return 4 * 3;
		case ShaderDataType::Int4:
			return 4 * 4;
		case ShaderDataType::UInt:
			return 4;
		case ShaderDataType::UInt2:
			return 4 * 2;
		case ShaderDataType::UInt3:
			return 4 * 3;
		case ShaderDataType::UInt4:
			return 4 * 4;
		case ShaderDataType::Float:
			return 4;
		case ShaderDataType::Float2:
			return 4 * 2;
		case ShaderDataType::Float3:
			return 4 * 3;
		case ShaderDataType::Float4:
			return 4 * 4;
		case ShaderDataType::Mat2x2:
			return 4 * 2 * 2;
		case ShaderDataType::Mat3x3:
			return 4 * 3 * 3;
		case ShaderDataType::Mat4x4:
			return 4 * 4 * 4;
		default:
			return 0;
		}
	}

	size_t ShaderDataType::GetCount(ShaderDataType::Enum DataType)
	{
		switch (DataType)
		{
		case ShaderDataType::Bool:
			return 1;
		case ShaderDataType::Bool2:
			return 2;
		case ShaderDataType::Bool3:
			return 3;
		case ShaderDataType::Bool4:
			return 4;
		case ShaderDataType::Int:
			return 1;
		case ShaderDataType::Int2:
			return 2;
		case ShaderDataType::Int3:
			return 3;
		case ShaderDataType::Int4:
			return 4;
		case ShaderDataType::UInt:
			return 1;
		case ShaderDataType::UInt2:
			return 2;
		case ShaderDataType::UInt3:
			return 3;
		case ShaderDataType::UInt4:
			return 4;
		case ShaderDataType::Float:
			return 1;
		case ShaderDataType::Float2:
			return 2;
		case ShaderDataType::Float3:
			return 3;
		case ShaderDataType::Float4:
			return 4;
		case ShaderDataType::Mat2x2:
			return 2;
		case ShaderDataType::Mat3x3:
			return 3;
		case ShaderDataType::Mat4x4:
			return 4;
		default:
			return 0;
		}
	}

	std::string ShaderDataType::ToString(ShaderDataType::Enum DataType)
	{
		switch (DataType)
		{
		case ShaderDataType::Bool:
			return "Bool";
		case ShaderDataType::Bool2:
			return "Bool2";
		case ShaderDataType::Bool3:
			return "Bool3";
		case ShaderDataType::Bool4:
			return "Bool4";
		case ShaderDataType::Int:
			return "Int";
		case ShaderDataType::Int2:
			return "Int2";
		case ShaderDataType::Int3:
			return "Int3";
		case ShaderDataType::Int4:
			return "Int4";
		case ShaderDataType::UInt:
			return "UInt";
		case ShaderDataType::UInt2:
			return "UInt2";
		case ShaderDataType::UInt3:
			return "UInt3";
		case ShaderDataType::UInt4:
			return "UInt4";
		case ShaderDataType::Float:
			return "Float";
		case ShaderDataType::Float2:
			return "Float2";
		case ShaderDataType::Float3:
			return "Float3";
		case ShaderDataType::Float4:
			return "Float4";
		case ShaderDataType::Mat2x2:
			return "Mat2x2";
		case ShaderDataType::Mat3x3:
			return "Mat3x3";
		case ShaderDataType::Mat4x4:
			return "Mat4x4";
		default:
			return "Unknown";
		}
	}

	ShaderDataType::Enum ShaderDataType::FromString(const std::string& String)
	{
		ShaderDataType::Enum Out = ShaderDataType::Unknown;

		for (uint8_t i = static_cast<uint8_t>(ShaderDataType::First); i <= static_cast<uint8_t>(ShaderDataType::Last); i++)
		{
			if (String == ToString(static_cast<ShaderDataType::Enum>(i)))
			{
				Out = static_cast<ShaderDataType::Enum>(i);
				break;
			}
		}

		return Out;
	}
}