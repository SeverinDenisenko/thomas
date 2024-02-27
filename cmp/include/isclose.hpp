#pragma once

#include "mod.hpp"

namespace cmp
{

template <typename real>
constexpr real eps();

template <>
constexpr float eps<float>()
{
    return 0x1p-23f; // 1.192e-07
};

template <>
constexpr double eps<double>()
{
    return 0x1p-52; // 2.220e-16
};

template <>
constexpr long double eps<long double>()
{
    return 0x1p-63l; // 1.084e-19
};

static_assert(eps<float>() > 0.0f);
static_assert(eps<double>() > 0.0);
static_assert(eps<long double>() > 0.0l);

/*
 * Selecting rigth tolerance for general case is imposible.
 * This value was selected based on experements.
 */

template <typename real>
inline bool isclose(real a, real b, real sigma = 10.0)
{
    return mod(a - b) < eps<real>() * sigma;
}
} // namespace cmp