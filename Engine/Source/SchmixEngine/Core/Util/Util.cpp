#include <SchmixEngine/Core/Util/Util.h>

namespace SchmixEngine
{
	size_t Util::AlignOffset(size_t CurrentOffset, size_t Alignment)
	{
		return CurrentOffset % Alignment != 0 ? CurrentOffset + (Alignment - (CurrentOffset % Alignment)) : CurrentOffset;
	}
}
