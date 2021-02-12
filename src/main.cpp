// custom allocator example
#include "fallback_allocator.hpp"
#include "freelist.h"
#include "stack_allocator.h"
#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>

int main() {
  FallbackAllocator<StackAllocator, FreeList<mal>>
}