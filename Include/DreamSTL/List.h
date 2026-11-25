#pragma once

#include "Core.h"
#include "DefaultAllocator.h"

namespace DreamSTL
{
	template <typename T, typename Allocator = DefaultAllocator>
	class List
	{
	public:
		using ElementType = T;
		using AllocatorType = Allocator;
	private:
	};
}