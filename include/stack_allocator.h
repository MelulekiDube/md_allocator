#pragma once
#include "memory_blk.h"
#include <stdlib.h>

namespace {
#define WORD_ALIGN 4
int roundToAligned(int n) {
  int padding = n % WORD_ALIGN;
  return n + padding;
}
}; // namespace

template <size_t s> class StackAllocator {
  char _d[s];
  char *_p;

public:
  Block allocate(size_t n) {
    auto n1 = roundToAligned(n);
    if (n1 > (_d + s) - _p) {
      return {nullptr, 0};
    }
    Block result = {_p, n};
    _p += n1;

    return result;
  }

  void deallocate(Block b) {
    char *ptr = static_cast<char *>(b._ptr);
    if (ptr + roundToAligned(b._size) == _p) {
      // we only deallocate things to the side and not anything in the middle
      _p = ptr;
    }
  }

  void deallocateAll() { _p = _d; }

  bool owns(Block b) { return b._ptr >= _d && b._ptr < _d + s; }
};