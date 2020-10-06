#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#ifdef NDEBUG
#   define ASSERT(x) ((void)0)
#else
#   include <cassert>
#   define ASSERT(x) assert(x)
#endif

#endif
