#pragma once
#include <stdint.h>

namespace SchmixEngine
{
	class Util
	{
	public:
		Util() = delete;

		Util(const Util&) = delete;
		Util& operator=(const Util&) = delete;

		static size_t AlignOffset(size_t CurrentOffset, size_t Alignment);
	};
}