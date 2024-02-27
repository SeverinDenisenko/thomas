#pragma once

#include <cmath>

/*
 * Is C/C++ absolute value functions is absolute mess.
 * We can't trust the compiler implementation with that.
 */
namespace cmp
{

template <typename real>
inline real mod(real);

template <>
inline float mod<float>(float a)
{
    return fabsf(a);
};

template <>
inline double mod<double>(double a)
{
    return fabs(a);
};

template <>
inline long double mod<long double>(long double a)
{
    return fabsl(a);
};
} // namespace cmp