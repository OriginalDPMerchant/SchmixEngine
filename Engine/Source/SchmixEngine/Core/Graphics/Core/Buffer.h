#pragma once
#include <vector>
#include <string>

namespace SchmixEngine
{
	enum class BufferType : uint8_t
	{
		Unknown,
		VertexBuffer,
		ElementBuffer,
		UniformBuffer,
		ShaderStorageBuffer
	};

	enum class ElementBufferFormat : uint8_t
	{
		Unknown,
		UInt16,
		UInt32
	};

	class ShaderDataType final
	{
	public:
		ShaderDataType() = delete;
		ShaderDataType(const ShaderDataType&) = delete;
		ShaderDataType& operator=(const ShaderDataType&) = delete;

		enum Enum : uint8_t
		{
			Unknown,
			Bool, Bool2, Bool3, Bool4,
			Int, Int2, Int3, Int4,
			UInt, UInt2, UInt3, UInt4,
			Float, Float2, Float3, Float4,
			Mat2x2, Mat3x3, Mat4x4,

			First = Unknown,
			Last = Mat4x4
		};

		static size_t GetSize(ShaderDataType::Enum DataType);

		static size_t GetCount(ShaderDataType::Enum DataType);

		static std::string ToString(ShaderDataType::Enum Type);
		static ShaderDataType::Enum FromString(const std::string& String);
	};

	struct BufferLayoutElement
	{
		ShaderDataType::Enum Type;
		size_t Size;
		size_t Offset;
		uint32_t Count;
	};

	class PackedBufferLayout final
	{
	public:
		PackedBufferLayout();
		PackedBufferLayout(std::initializer_list<ShaderDataType::Enum> Layout);
		PackedBufferLayout(std::vector<ShaderDataType::Enum>& Layout);

		void SetBufferLayout(std::vector<ShaderDataType::Enum>& Layout);
		const std::vector<BufferLayoutElement>& GetBufferLayoutElements();

		size_t GetStride();

	private:
		std::vector<BufferLayoutElement> m_BufferLayoutElements;

		size_t m_Stride;
	};
}