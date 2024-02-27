#pragma once

/*
 * Most programmers say that without benchmarks force inlining functions is bad.
 * It might be, but we have some trivial hot paths, and we dont want to trust compiler with that.
 */

#if defined(__clang__)
#define forceinline __attribute__((always_inline))
#elif defined(__GNUC__) || defined(__GNUG__)
#define forceinline __inline__
#elif defined(_MSC_VER)
#define forceinline __forceinline
#else
#error Get rid of this antient potato!
#endif
