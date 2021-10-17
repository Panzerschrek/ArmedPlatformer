#pragma once
#include <cassert>

// Simple assert wrapper.
// If you wish disable asserts, or do something else redefine this macro.
#ifdef DEBUG
#define ARMED_ASSERT(x) \
	{ assert(x); }
#else
#define ARMED_ASSERT(x) {}
#endif

#define ARMED_UNUSED(x) (void)x
