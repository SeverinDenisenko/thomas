#pragma once

namespace cmp
{

constexpr bool debug()
{
#ifdef NDEBUG
    return false;
#else
    return true;
#endif
}
} // namespace cmp