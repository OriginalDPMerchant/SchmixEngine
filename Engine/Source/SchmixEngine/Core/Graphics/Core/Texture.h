#pragma once
#include <string>

namespace SchmixEngine
{
	class TextureType
	{
	public:
		TextureType() = delete;
		TextureType(const TextureType&) = delete;
		TextureType& operator=(const TextureType&) = delete;

		enum Enum : uint8_t
		{
			Unknown,
			Texture1D,
			Texture1D_Array,
			Texture2D,
			Texture2D_Array,
			CubeMap,
			CubeMap_Array,
			Texture3D,

			First = Unknown,
			Last = Texture3D
		};


		static std::string ToString(TextureType::Enum Value);
		static TextureType::Enum FromString(const std::string& String);
	};

	class TextureFormat
	{
	public:
		TextureFormat() = delete;
		TextureFormat(const TextureFormat&) = delete;
		TextureFormat& operator=(const TextureFormat&) = delete;

		enum Enum : uint8_t
		{
			Unknown,
			R8,
			R8_G8,
			R8_G8_B8,
			R8_G8_B8_A8,
			R8_G8_B8_SRGB,
			R8_G8_B8_A8_SRGB,
			R16,
			R16_G16,
			R16_G16_B16,
			R16_G16_B16_A16,
			R16_FLOAT,
			R16_G16_FLOAT,
			R16_G16_B16_FLOAT,
			R16_G16_B16_A16_FLOAT,
			R32_FLOAT,
			R32_G32_FLOAT,
			R32_G32_B32_FLOAT,
			R32_G32_B32_A32_FLOAT,

			First = Unknown,
			Last = R32_G32_B32_A32_FLOAT
		};

		static uint32_t GetNumChannels(TextureFormat::Enum Value);
		static uint32_t GetBitsPerChannel(TextureFormat::Enum Value);
		static uint32_t GetBitsPerPixel(TextureFormat::Enum Value);

		static std::string ToString(TextureFormat::Enum Value);
		static TextureFormat::Enum FromString(const std::string& String);
	};

	class TextureFilteringMode
	{
	public:
		TextureFilteringMode() = delete;
		TextureFilteringMode(const TextureFilteringMode&) = delete;
		TextureFilteringMode& operator=(const TextureFilteringMode&) = delete;

		enum Enum : uint8_t
		{
			Unknown,
			Nearest,
			Linear,

			First = Unknown,
			Last = Linear
		};

		static std::string ToString(TextureFilteringMode::Enum Value);
		static TextureFilteringMode::Enum FromString(const std::string& String);
	};

	class TextureWrappingMode
	{
	public:
		TextureWrappingMode() = delete;
		TextureWrappingMode(const TextureWrappingMode&) = delete;
		TextureWrappingMode& operator=(const TextureWrappingMode&) = delete;

		enum Enum : uint8_t
		{
			Unknown,
			Repeat,
			MirroredRepeat,
			ClampToEdge,
			ClampToBorder,

			First = Unknown,
			Last = ClampToBorder
		};

		static std::string ToString(TextureWrappingMode::Enum Value);
		static TextureWrappingMode::Enum FromString(const std::string& String);
	};

	struct TextureInfo
	{
		TextureType::Enum Type;
		TextureFormat::Enum Format;
		TextureFilteringMode::Enum FilterMode;
		TextureWrappingMode::Enum WrappingMode;
		uint32_t MipMapLevels;
		uint32_t Width; 
		uint32_t Height; 
		uint32_t Depth;
	};
}