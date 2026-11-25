#pragma once

#include <cstddef>
#include <new>

namespace DreamSTL
{
	struct DefaultAllocator
	{
		using SizeType = unsigned int;

		[[nodiscard]] void* Allocate(size_t _bytes, size_t _alignment)
		{
			if (_alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
			{
				return ::operator new(_bytes, std::align_val_t{ _alignment });
			}
			return ::operator new(_bytes);
		}
		void Deallocate(void* _ptr, size_t _bytes, size_t _alignment) noexcept
		{
			if (_alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
				::operator delete(_ptr, _bytes, std::align_val_t{ _alignment });
			else
				::operator delete(_ptr, _bytes);
		}

		bool operator==(const DefaultAllocator&) const = default;
	};
}