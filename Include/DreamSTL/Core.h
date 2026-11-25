#pragma once
#include <cassert>

#ifndef DS_ASSERT
	#define DS_ASSERT(expr) assert(expr)
#endif

// Core.h
#if defined(_MSC_VER)
#define DS_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
#define DS_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif
