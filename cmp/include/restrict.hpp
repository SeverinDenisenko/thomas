#pragma once

/*
 * Since we are writing code in C++, restrict keyword is not allowed. So we are using this.
 * This may be a problem if we are using exotic compilers, but majors (gcc, clang, msvc) support this.
 */

#ifdef __cplusplus
#if defined(__clang__)
#define restrict __restrict__
#elif defined(__GNUC__) || defined(__GNUG__)
#define restrict __restrict__
#elif defined(_MSC_VER)
#define restrict __restrict
#else
#error Get rid of this antient potato!
#endif
#endif
