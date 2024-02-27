#pragma once

#include <cmath>

template <typename real>
inline real round(real a, size_t n)
{
    real orders = real(pow(real(10.0), real(n)));
    return real(round(a * orders)) / orders;
}
