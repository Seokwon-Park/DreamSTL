#pragma once

#include <cstddef>
#include <new>

namespace DreamSTL
{
	struct DefaultAllocator
	{
		using SizeType = unsigned int;

		[[nodiscard]] void* Allocate(SizeType _bytes, size_t _alignment)
		{
			return ::operator new(_bytes, std::align_val_t{ _alignment });
		}
		void Deallocate(void* _ptr, SizeType _bytes, size_t _alignment) noexcept
		{
			::operator delete(_ptr, _bytes, std::align_val_t{ _alignment });
		}

		bool operator==(const DefaultAllocator&) const = default;
	};
}