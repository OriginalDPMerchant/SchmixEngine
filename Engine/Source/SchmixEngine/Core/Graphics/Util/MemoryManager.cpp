#include "MemoryManager.h"

namespace SchmixEngine
{
	BufferMemoryManager::BufferMemoryManager()
	{
	}

	void BufferMemoryManager::Init(size_t BufferSize)
	{
		m_BufferMemoryBlocks.clear();
		m_BufferMemoryBlocks.push_back({ 0, BufferSize, true });
	}

	BufferMemoryBlockPtr BufferMemoryManager::Malloc(size_t Size, size_t Alignment)
	{
		for (size_t i = 0; i < m_BufferMemoryBlocks.size(); i++)
		{
			size_t CurrentBlockIndex = m_BufferMemoryBlocks.size() - i - 1;
			BufferMemoryBlock& CurrentBlock = m_BufferMemoryBlocks[CurrentBlockIndex];

			if (CurrentBlock.bIsDirty)
			{
				size_t AlignedOffset = Util::AlignOffset(CurrentBlock.Offset, Alignment);

				size_t Padding = AlignedOffset - CurrentBlock.Offset;

				if (CurrentBlock.Size >= Padding + Size)
				{
					auto Iterator = m_BufferMemoryBlocks.begin() + CurrentBlockIndex;
					size_t ExtraSpace = CurrentBlock.Size - Padding - Size;

					CurrentBlock.bIsDirty = Padding > 0;
					CurrentBlock.Size = Padding > 0 ? Padding : Size;

					if (Padding > 0)
						Iterator = m_BufferMemoryBlocks.insert(++Iterator, { AlignedOffset, Size, false });

					if (ExtraSpace > 0)
						Iterator = m_BufferMemoryBlocks.insert(++Iterator, { AlignedOffset + Size, ExtraSpace, true });

					return { AlignedOffset, true };
				}
			}
		}

		return { 0, false };
	}

	bool BufferMemoryManager::Free(size_t Offset)
	{
		size_t Left = 0;
		size_t Right = m_BufferMemoryBlocks.size() - 1;

		while (Left <= Right)
		{
			size_t Middle = Left + (Right - Left) / 2;
			size_t CurrentBlockOffset = m_BufferMemoryBlocks[Middle].Offset;

			if (Offset == CurrentBlockOffset)
			{
				m_BufferMemoryBlocks[Middle].bIsDirty = true;
				return true;
			}
			else if (Offset > CurrentBlockOffset)
			{
				Left = Middle + 1;
			}
			else
			{
				Right = Middle - 1;
			}
		}

		return false;
	}

	void BufferMemoryManager::MergeAdjacentFreeMemory()
	{
		auto Iterator = m_BufferMemoryBlocks.begin();
		auto Next = Iterator + 1;

		while (Next != m_BufferMemoryBlocks.end())
		{
			if (Iterator->bIsDirty && Next->bIsDirty)
			{
				Next->Offset = Iterator->Offset;
				Next->Size += Iterator->Size;
				Next = m_BufferMemoryBlocks.erase(Iterator);
			}

			Iterator = Next;
			Next = Iterator + 1;
		}
	}
}