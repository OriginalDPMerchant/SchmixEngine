#include <SchmixEngine/Core/Graphics/Core/Texture.h>

namespace SchmixEngine
{
	std::string TextureType::ToString(TextureType::Enum Value)
	{
		switch (Value)
		{
		case TextureType::Texture1D:
			return "Texture1D";
		case TextureType::Texture1D_Array:
			return "Texture1D_Array";
		case TextureType::Texture2D:
			return "Texture2D";
		case TextureType::Texture2D_Array:
			return "Texture2D_Array";
		case TextureType::CubeMap:
			return "CubeMap";
		case TextureType::CubeMap_Array:
			return "CubeMapArray";
		case TextureType::Texture3D:
			return "Texture3D";
		default:
			return "Unknown";
		}
	}

	TextureType::Enum TextureType::FromString(const std::string& String)
	{
		TextureType::Enum Out = TextureType::Unknown;

		for (uint8_t i = static_cast<uint8_t>(TextureType::First); i <= static_cast<uint8_t>(TextureType::Last); i++)
		{
			if (String == ToString(static_cast<TextureType::Enum>(i)))
			{
				Out = static_cast<TextureType::Enum>(i);
				break;
			}
		}

		return Out;
	}

	uint32_t TextureFormat::GetNumChannels(TextureFormat::Enum Value)
	{
		switch (Value)
		{
		case TextureFormat::R8:
			return 1;
		case TextureFormat::R8_G8:
			return 2;
		case TextureFormat::R8_G8_B8:
			return 3;
		case TextureFormat::R8_G8_B8_A8:
			return 4;
		case TextureFormat::R8_G8_B8_SRGB:
			return 3;
		case TextureFormat::R8_G8_B8_A8_SRGB:
			return 4;
		case TextureFormat::R16:
			return 1;
		case TextureFormat::R16_G16:
			return 2;
		case TextureFormat::R16_G16_B16:
			return 3;
		case TextureFormat::R16_G16_B16_A16:
			return 4;
		case TextureFormat::R16_FLOAT:
			return 1;
		case TextureFormat::R16_G16_FLOAT:
			return 2;
		case TextureFormat::R16_G16_B16_FLOAT:
			return 3;
		case TextureFormat::R16_G16_B16_A16_FLOAT:
			return 4;
		case TextureFormat::R32_FLOAT:
			return 1;
		case TextureFormat::R32_G32_FLOAT:
			return 2;
		case TextureFormat::R32_G32_B32_FLOAT:
			return 3;
		case TextureFormat::R32_G32_B32_A32_FLOAT:
			return 4;
		default:
			return 0;
		}
	}

	uint32_t TextureFormat::GetBitsPerChannel(TextureFormat::Enum Value)
	{
		switch (Value)
		{
		case TextureFormat::R8:
			return 8;
		case TextureFormat::R8_G8:
			return 8;
		case TextureFormat::R8_G8_B8:
			return 8;
		case TextureFormat::R8_G8_B8_A8:
			return 8;
		case TextureFormat::R8_G8_B8_SRGB:
			return 8;
		case TextureFormat::R8_G8_B8_A8_SRGB:
			return 8;
		case TextureFormat::R16:
			return 16;
		case TextureFormat::R16_G16:
			return 16;
		case TextureFormat::R16_G16_B16:
			return 16;
		case TextureFormat::R16_G16_B16_A16:
			return 16;
		case TextureFormat::R16_FLOAT:
			return 16;
		case TextureFormat::R16_G16_FLOAT:
			return 16;
		case TextureFormat::R16_G16_B16_FLOAT:
			return 16;
		case TextureFormat::R16_G16_B16_A16_FLOAT:
			return 16;
		case TextureFormat::R32_FLOAT:
			return 32;
		case TextureFormat::R32_G32_FLOAT:
			return 32;
		case TextureFormat::R32_G32_B32_FLOAT:
			return 32;
		case TextureFormat::R32_G32_B32_A32_FLOAT:
			return 32;
		default:
			return 0;
		}
	}

	uint32_t TextureFormat::GetBitsPerPixel(TextureFormat::Enum Value)
	{
		return TextureFormat::GetNumChannels(Value) * TextureFormat::GetBitsPerChannel(Value);
	}

	std::string TextureFormat::ToString(TextureFormat::Enum Value)
	{
		switch (Value)
		{
		case TextureFormat::R8:
			return "R8";
		case TextureFormat::R8_G8:
			return "R8_G8";
		case TextureFormat::R8_G8_B8:
			return "R8_G8_B8";
		case TextureFormat::R8_G8_B8_A8:
			return "R8_G8_B8_A8";
		case TextureFormat::R8_G8_B8_SRGB:
			return "R8_G8_B8_SRGB";
		case TextureFormat::R8_G8_B8_A8_SRGB:
			return "R8_G8_B8_A8_SRGB";
		case TextureFormat::R16:
			return "R16";
		case TextureFormat::R16_G16:
			return "R16_G16";
		case TextureFormat::R16_G16_B16:
			return "R16_G16_B16";
		case TextureFormat::R16_G16_B16_A16:
			return "R16_G16_B16_A16";
		case TextureFormat::R16_FLOAT:
			return "R16_FLOAT";
		case TextureFormat::R16_G16_FLOAT:
			return "R16_G16_FLOAT";
		case TextureFormat::R16_G16_B16_FLOAT:
			return "R16_G16_B16_FLOAT";
		case TextureFormat::R16_G16_B16_A16_FLOAT:
			return "R16_G16_B16_A16_FLOAT";
		case TextureFormat::R32_FLOAT:
			return "R32_FLOAT";
		case TextureFormat::R32_G32_FLOAT:
			return "R32_G32_FLOAT";
		case TextureFormat::R32_G32_B32_FLOAT:
			return "R32_G32_B32_FLOAT";
		case TextureFormat::R32_G32_B32_A32_FLOAT:
			return "R32_G32_B32_A32_FLOAT";
		default:
			return "Unknown";
		}
	}

	TextureFormat::Enum TextureFormat::FromString(const std::string& String)
	{
		TextureFormat::Enum Out = TextureFormat::Unknown;

		for (uint8_t i = static_cast<uint8_t>(TextureFormat::First); i <= static_cast<uint8_t>(TextureFormat::Last); i++)
		{
			if (String == ToString(static_cast<TextureFormat::Enum>(i)))
			{
				Out = static_cast<TextureFormat::Enum>(i);
				break;
			}
		}

		return Out;
	}

	std::string TextureFilteringMode::ToString(TextureFilteringMode::Enum Value)
	{
		switch (Value)
		{
		case TextureFilteringMode::Nearest:
			return "Nearest";
		case TextureFilteringMode::Linear:
			return "Linear";
		default:
			return "Unknown";
		}
	}

	TextureFilteringMode::Enum TextureFilteringMode::FromString(const std::string& String)
	{
		TextureFilteringMode::Enum Out = TextureFilteringMode::Unknown;

		for (uint8_t i = static_cast<uint8_t>(TextureFilteringMode::First); i <= static_cast<uint8_t>(TextureFilteringMode::Last); i++)
		{
			if (String == ToString(static_cast<TextureFilteringMode::Enum>(i)))
			{
				Out = static_cast<TextureFilteringMode::Enum>(i);
				break;
			}
		}

		return Out;
	}

	std::string TextureWrappingMode::ToString(TextureWrappingMode::Enum Value)
	{
		switch (Value)
		{
		case TextureWrappingMode::Repeat:
			return "Repeat"; 
		case TextureWrappingMode::MirroredRepeat:
			return "Repeat";
		case TextureWrappingMode::ClampToEdge:
			return "ClampToEdge";
		case TextureWrappingMode::ClampToBorder:
			return "ClampToEdge";
		default:
			return "Unknown";
		}
	}

	TextureWrappingMode::Enum TextureWrappingMode::FromString(const std::string& String)
	{
		TextureWrappingMode::Enum Out = TextureWrappingMode::Unknown;

		for (uint8_t i = static_cast<uint8_t>(TextureWrappingMode::First); i <= static_cast<uint8_t>(TextureWrappingMode::Last); i++)
		{
			if (String == ToString(static_cast<TextureWrappingMode::Enum>(i)))
			{
				Out = static_cast<TextureWrappingMode::Enum>(i);
				break;
			}
		}

		return Out;
	}
}