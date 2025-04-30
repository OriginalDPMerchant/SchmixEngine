#pragma once
#include <SchmixEngine/Core/Util/Util.h>
#include <vector>

namespace SchmixEngine
{
	struct BufferMemoryBlock
	{
		size_t Offset;
		size_t Size;
		bool bIsDirty;
	};

	struct BufferMemoryBlockPtr
	{
		size_t Offset;
		bool bIsValid;
	};

	class BufferMemoryManager
	{
	public:
		BufferMemoryManager();

		void Init(size_t BufferSize);

		BufferMemoryBlockPtr Malloc(size_t Size, size_t Allignment);

		bool Free(size_t Offset);

		void MergeAdjacentFreeMemory();

	private:
		std::vector<BufferMemoryBlock> m_BufferMemoryBlocks;
	};
}