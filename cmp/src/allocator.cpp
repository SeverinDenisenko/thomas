#include "allocator.hpp"

namespace cmp
{

void* allocator::allocate(size_t size)
{
    return malloc(size);
}

void allocator::deallocate(void* ptr)
{
    free(ptr);
}
} // namespace cmp