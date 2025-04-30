#pragma once
#include <SchmixEngine/Core/Graphics/Core/Buffer.h>
#include <SchmixEngine/Core/Graphics/Core/ShaderData.h>
#include <SchmixEngine/ThirdParty/GLM/glm.hpp>
#include <string>

namespace SchmixEngine
{
	class ShaderStage
	{
	public:
		ShaderStage() = delete;
		ShaderStage(const ShaderStage&) = delete;
		ShaderStage& operator=(const ShaderStage&) = delete;

		enum Enum : uint8_t
		{
			Unknown,
			VertexShaderStage,
			//TesselationControlShaderStage,
			//TesselationEvaluationShaderStage,
			//GeometryShaderStage,
			FragmentShaderStage,
			//ComputeShaderStage

			First = Unknown,
			Last = FragmentShaderStage
		};
		
		static std::string ToString(ShaderStage::Enum Stage);
		static ShaderStage::Enum FromString(const std::string& String);
	};

	class VertexInputRate
	{
	public:
		VertexInputRate() = delete;
		VertexInputRate(const VertexInputRate&) = delete;
		VertexInputRate& operator=(const VertexInputRate&) = delete;

		enum Enum : uint8_t
		{
			Unknown,
			PerVertex,
			PerInstance,

			First = Unknown,
			Last = PerInstance
		};

		static std::string ToString(VertexInputRate::Enum Rate);
		static VertexInputRate::Enum FromString(const std::string& String);
	};

	class PrimitiveAssemblyType
	{
	public:
		PrimitiveAssemblyType() = delete;
		PrimitiveAssemblyType(const PrimitiveAssemblyType&) = delete;
		PrimitiveAssemblyType& operator=(const PrimitiveAssemblyType&) = delete;

		enum Enum : uint8_t
		{
			Unknown,
			Triangles,
			TriangleFan,
			TriangleStrip,
			Line,
			LineStrip,
			LineLoop,

			First = Unknown,
			Last = LineLoop
		};

		static std::string ToString(PrimitiveAssemblyType::Enum Type);
		static PrimitiveAssemblyType::Enum FromString(const std::string& String);
	};

	class BlendFactor
	{
	public:
		BlendFactor() = delete;
		BlendFactor(const BlendFactor&) = delete;
		BlendFactor& operator=(const BlendFactor&) = delete;

		enum Enum : uint8_t
		{
			Unknown,
			Zero,
			One,
			SrcColour,
			OneMinusSrcColour,
			DstColour,
			OneMinusDstColour,
			SrcAlpha,
			OneMinusSrcAlpha,
			DstAlpha,
			OneMinusDstAlpha,
			ConstantColour,
			OneMinusConstantColour,
			ConstantAlpha,
			OneMinusConstantAlpha,
			SrcAlphaSaturate,
			ReciprocalSrcColour,
			OneMinusReciprocalSrcColour,
			ReciprocalSrcAlpha,
			OneMinusReciprocalSrcAlpha,

			First = Unknown,
			Last = OneMinusReciprocalSrcAlpha
		};

		static std::string ToString(BlendFactor::Enum Type);
		static BlendFactor::Enum FromString(const std::string& String);
	};

	class BlendOperation
	{
	public:
		BlendOperation() = delete;
		BlendOperation(const BlendOperation&) = delete;
		BlendOperation& operator=(const BlendOperation&) = delete;

		enum Enum : uint8_t
		{
			Unknown,
			Add,
			Subtract,
			ReverseSubtract,
			Min,
			Max,

			First = Unknown,
			Last = Max
		};

		static std::string ToString(BlendOperation::Enum Type);
		static BlendOperation::Enum FromString(const std::string& String);
	};

	struct ShaderStageCreateInfo
	{
		ShaderStage::Enum ShaderStageType;
		const char* pShaderCode;
	};

	struct ShaderStageCreateInfo_2
	{
		ShaderStage::Enum ShaderStageType;
		std::string ShaderCode;
	};

	struct VertexInputInfo
	{
		uint32_t VertexBufferBindingIndex;
		VertexInputRate::Enum VertexInputRate;
		PackedBufferLayout* pVertexBufferLayout;
	};

	struct VertexInputInfo_2
	{
		uint32_t VertexBufferBindingIndex;
		VertexInputRate::Enum VertexInputRate;
		PackedBufferLayout VertexBufferLayout;
	};

	struct PrimitiveAssemblyInfo
	{
		PrimitiveAssemblyType::Enum PrimitiveType;
		bool bPrimativeRestartEnabled;
	};

	struct ColourBlendingInfo
	{
		bool bBlendingEnabled;
		BlendFactor::Enum SrcColourBlendFactor;
		BlendFactor::Enum DstColourBlendFactor;
		BlendOperation::Enum ColourBlendOperation;
		BlendFactor::Enum SrcAlphaBlendFactor;
		BlendFactor::Enum DstAlphaBlendFactor;
		BlendOperation::Enum AlphaBlendOperation;
		glm::vec4 ConstantColour;
	};

	struct TextureSlot
	{
		std::string TextureSlotName;
		int64_t TextureSlotLocation;
	};

	struct GraphicsPipelineCreateInfo
	{
		uint32_t NumShaderStages;
		ShaderStageCreateInfo* pShaderStages;
		uint32_t NumVertexInputInfos;
		VertexInputInfo* pVertexInputInfos;
		PrimitiveAssemblyInfo PrimitiveAssemblyInfo;
		ColourBlendingInfo ColourBlendingInfo;
		uint32_t NumTextureSlots;
		TextureSlot* pTextureSlots;
		uint32_t NumShaderParameters;
		ShaderParameter* pShaderParameters;
	};

	struct GraphicsPipelineCreateInfo_2
	{
		std::vector<ShaderStageCreateInfo_2> ShaderStages;
		std::vector<VertexInputInfo_2> VertexInputInfos;
		PrimitiveAssemblyInfo PrimitiveAssemblyInfo;
		ColourBlendingInfo ColourBlendingInfo;
		std::vector<TextureSlot> TextureSlots;
		std::vector<ShaderParameter> ShaderParameters;
	};

	struct GraphicsPipelineState
	{
		PrimitiveAssemblyInfo PrimitiveAssemblyInfo;
		ColourBlendingInfo ColourBlendingInfo;
	};
}