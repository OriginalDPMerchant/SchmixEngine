#include <SchmixEngine/Core/Graphics/Core/GraphicsPipeline.h>

namespace SchmixEngine
{
	std::string ShaderStage::ToString(ShaderStage::Enum Stage)
	{
		switch (Stage)
		{
		case ShaderStage::VertexShaderStage:
			return "VertexShader";
		case ShaderStage::FragmentShaderStage:
			return "FragmentShader";
		default:
			return "Unknown";
		}
	}

	ShaderStage::Enum ShaderStage::FromString(const std::string& String)
	{
		ShaderStage::Enum Stage = ShaderStage::Unknown;

		for (uint8_t i = static_cast<uint8_t>(ShaderStage::First); i <= static_cast<uint8_t>(ShaderStage::Last); i++)
		{
			if (String == ToString(static_cast<ShaderStage::Enum>(i)))
			{
				Stage = static_cast<ShaderStage::Enum>(i);
				break;
			}
		}

		return Stage;
	}

	std::string VertexInputRate::ToString(VertexInputRate::Enum Rate)
	{
		switch (Rate)
		{
		case VertexInputRate::PerInstance:
			return "PerInstance";
		case VertexInputRate::PerVertex:
			return "PerVertex";
		default:
			return "Unknown";
		}
	}

	VertexInputRate::Enum VertexInputRate::FromString(const std::string& String)
	{
		VertexInputRate::Enum Rate = VertexInputRate::Unknown;

		for (uint8_t i = static_cast<uint8_t>(VertexInputRate::First); i <= static_cast<uint8_t>(VertexInputRate::Last); i++)
		{
			if (String == ToString(static_cast<VertexInputRate::Enum>(i)))
			{
				Rate = static_cast<VertexInputRate::Enum>(i);
				break;
			}
		}

		return Rate;
	}

	std::string PrimitiveAssemblyType::ToString(PrimitiveAssemblyType::Enum Type)
	{
		switch (Type)
		{
		case PrimitiveAssemblyType::Triangles:
			return "Triangles";
		case PrimitiveAssemblyType::TriangleFan:
			return "TriangleFan";
		case PrimitiveAssemblyType::TriangleStrip:
			return "TriangleFanStrip";
		case PrimitiveAssemblyType::Line:
			return "Line";
		case PrimitiveAssemblyType::LineStrip:
			return "LineStrip";
		case PrimitiveAssemblyType::LineLoop:
			return "LineLoop";
		default:
			return "Unknown";
		}
	}

	PrimitiveAssemblyType::Enum PrimitiveAssemblyType::FromString(const std::string& String)
	{
		PrimitiveAssemblyType::Enum Out = PrimitiveAssemblyType::Unknown;

		for (uint8_t i = static_cast<uint8_t>(PrimitiveAssemblyType::First); i <= static_cast<uint8_t>(PrimitiveAssemblyType::Last); i++)
		{
			if (String == ToString(static_cast<PrimitiveAssemblyType::Enum>(i)))
			{
				Out = static_cast<PrimitiveAssemblyType::Enum>(i);
				break;
			}
		}

		return Out;
	}

	std::string BlendFactor::ToString(BlendFactor::Enum Type)
	{
		switch (Type)
		{
		case BlendFactor::Zero:
			return "Zero";
		case BlendFactor::One:
			return "One";
		case BlendFactor::SrcColour:
			return "SrcColour";
		case BlendFactor::OneMinusSrcColour:
			return "OneMinusSrcColour";
		case BlendFactor::DstColour:
			return "DstColour";
		case BlendFactor::OneMinusDstColour:
			return "OneMinusDstColour";
		case BlendFactor::SrcAlpha:
			return "SrcAlpha";
		case BlendFactor::OneMinusSrcAlpha:
			return "OneMinusSrcAlpha";
		case BlendFactor::DstAlpha:
			return "DstAlpha";
		case BlendFactor::OneMinusDstAlpha:
			return "OneMinusDstAlpha";
		case BlendFactor::ConstantColour:
			return "ConstantColour";
		case BlendFactor::OneMinusConstantColour:
			return "OneMinusConstantColour";
		case BlendFactor::ConstantAlpha:
			return "ConstantAlpha";
		case BlendFactor::OneMinusConstantAlpha:
			return "OneMinusConstantAlpha";
		case BlendFactor::SrcAlphaSaturate:
			return "SrcAlphaSaturate";
		case BlendFactor::ReciprocalSrcColour:
			return "ReciprocalSrcColour";
		case BlendFactor::OneMinusReciprocalSrcColour:
			return "OneMinusReciprocalSrcColour";
		case BlendFactor::ReciprocalSrcAlpha:
			return "ReciprocalSrcAlpha";
		case BlendFactor::OneMinusReciprocalSrcAlpha:
			return "OneMinusReciprocalSrcAlpha";
		default:
			return "Unknown";
		}
	}

	BlendFactor::Enum BlendFactor::FromString(const std::string& String)
	{
		BlendFactor::Enum Out = BlendFactor::Unknown;

		for (uint8_t i = static_cast<uint8_t>(BlendFactor::First); i <= static_cast<uint8_t>(BlendFactor::Last); i++)
		{
			if (String == ToString(static_cast<BlendFactor::Enum>(i)))
			{
				Out = static_cast<BlendFactor::Enum>(i);
				break;
			}
		}

		return Out;
	}

	std::string BlendOperation::ToString(BlendOperation::Enum Type)
	{
		switch (Type)
		{
		case BlendOperation::Add:
			return "Add";
		case BlendOperation::Subtract:
			return "Subtract";
		case BlendOperation::ReverseSubtract:
			return "ReverseSubtract";
		case BlendOperation::Min:
			return "Min";
		case BlendOperation::Max:
			return "Max";
		default:
			return "Unknown";
		}
	}

	BlendOperation::Enum BlendOperation::FromString(const std::string& String)
	{
		BlendOperation::Enum Out = BlendOperation::Unknown;

		for (uint8_t i = static_cast<uint8_t>(BlendOperation::First); i <= static_cast<uint8_t>(BlendOperation::Last); i++)
		{
			if (String == ToString(static_cast<BlendOperation::Enum>(i)))
			{
				Out = static_cast<BlendOperation::Enum>(i);
				break;
			}
		}

		return Out;
	}
}