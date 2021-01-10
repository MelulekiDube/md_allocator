#pragma once
#include "memory_blk.h"

class Mallocator {
public:
  Block allocate(size_t);
  void deallocate(Block);
};