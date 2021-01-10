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
  static constexpr unsigned align();
  static constexpr goodsize(size_t);

  Block allocate(size_t n) {
    Block result = Primary::allocate(n);

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

  //@Todo start here tomorrow
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
    // I want to check which between P and F owns memory b and then call the one
    // that I want
    // we can add a new primitive bool owns(Block) that must be defined in
    // primary fallback does not necesarily have to implement owns
    // FallbackAllocator will have to implement owns if both implement it (Good
    // Citizenry)
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

  bool owns(const Block &b) {
    // Relies on Method defination Failure is not an Error
    return Primary::owns(b) || Fallback::owns(b);
  }
};