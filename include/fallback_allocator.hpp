#pragma once
#include "base_allocator.h"
#include <math.h>

template <typename Primary, typename Fallback>
class FallbackAllocator : private Primary, private Fallback {
private:
  /*
  Tries to allocate with primary and then will then use fallbac
  We are designing with composibility
  */
public:
  static constexpr unsigned align = 8;

  static constexpr size_t goodsize(size_t n) {
    // we want to always give out memoory that 8 bytes aligned
    size_t size = n % align;
    if (size)
      return n + 1;
    return n;
  }

  Block allocate(size_t n) {
    size_t actual_size = Block result = Primary::allocate(n);

    if (!result._ptr) {
      result = Fallback::allocate(n);
    }

    return result;
  }

  Block allocateAll() {
    Block result = Primary::allocateAll();

    if (!result._ptr) {
      result = Fallback::allocateAll();
    }

    return result;
  }

  bool expand(Block &block, size_t size_delta) {
    if (Primary::owns(block)) {
      // if the primary owns the allocator and can't satisfy the request we will
      // try to move the contents to another
      if (Primary::expand(block, size_delta) || dube::swapAllocator(block, size_delta, As(Primary), As(Fallback)) {
        return true;
      }
    }
    if (Fallback::owns(block)) {
        if (Fallback::expand(block, size_delta) || dube::swapAllocator(block, size_delta, As(Fallback), As(Primary)) {
        return true;
      }
    }
    return false;
  }

  void reallocate(Block &block, size_t n) {
    if (n > block._size) {
      expand(b, n);
    } else {
      if (Primary::owns(block)) {
        Primary::reallocate(block, n);
      } else {
        Fallback::reallocate(block, n);
      }
    }
  }

  void deallocate(Block b) {
    if (Primary::owns(b)) {
      Primary::deallocate(b);
    } else {
      Fallback::deallocate(b);
    }
  }

  void deallocateAll(Block &block) {
    Primary::deallocateAll();
    Fallback::deallocateAll();
  }

  Block alignedAllocate(size_t n) {
    Block result = Primary::alignedAllocate(n);
    if (result)
      return result;
    return Fallback::alignedAllocate(n);
  }

  Block alignedReAllocate(Block &block, size_t new_size) {
    if (Primary::owns(block)) {
      return Primary::alignedReAllocate(block, new_size);
    }
    return Fallback::alignedReAllocate(block, new_size);
  }

  bool owns(const Block &b) {
    // Relies on Method defination Failure is not an Error
    return Primary::owns(b) || Fallback::owns(b);
  }
};