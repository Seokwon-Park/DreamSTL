// Tests/Source/Main.cpp
#include <cstdio>
#include <cstdlib>
#include "DreamSTL/Array.h"

#define CHECK(expr)                                                     \
	do {                                                                \
		if (!(expr)) {                                                  \
			std::printf("FAILED: %s (%s:%d)\n", #expr, __FILE__, __LINE__); \
			std::abort();                                               \
		}                                                               \
	} while (0)

int main()
{
	DreamSTL::Array<int> arr;
	arr.Add(1);
	arr.Add(2);
	CHECK(arr.Size() == 2);
	CHECK(arr[1] == 2);

	std::printf("All tests passed\n");
}