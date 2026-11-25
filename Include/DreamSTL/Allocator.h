#pragma once
#include <cstddef>
#include <new>
#include "DefaultAllocator.h"

template <class T>
struct Allocator
{

public:
	Allocator();
	~Allocator() = default;

	void* Allocate(size_t _size);
	void Deallocate();
	void GetMaxSize();
	void Construct();
	void Destroy();

	bool operator==(const Allocator& _other);
}