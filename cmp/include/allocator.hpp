#pragma once

#include <cstdint>
#include <cstdlib>

/*
 * It may be nessesary to change allocation during development, so better to be prepared.
 * For example, jemalloc, numa_alloc, aligned_alloc or custom allocator may be better for our task.
 * If all vectors have the same length, maybe simple linear allocator will work faster then conventional malloc.
 */

namespace cmp
{

class allocator
{
public:
    static void* allocate(size_t size);

    static void deallocate(void* ptr);
};
} // namespace cmp