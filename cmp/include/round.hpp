#pragma once

#include <cmath>

template <typename real>
inline real round(real a, size_t n)
{
    real orders = (real)pow(10.0, n);
    return round(a * orders) / orders;
}
