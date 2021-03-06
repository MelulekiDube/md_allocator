
#pragma once
#include "memory_blk.h"
#include <stdlib.h>
#include <string.h>
namespace dube {

#define As(type) (type)(*this)

template <typename Allocator1, typename Allocator2>
bool swapAllocator(Block &block, size_t size_delta, Allocator1 originalOwner,
                   Allocator2 newOwner) {
  size_t size_require = size_delta + block._size;
  Block newBlock = newOwner.allocate(size_require);
  /* if Allocator 2 can satisfy the requests the we want to move the contents in
   * blcok.ptr to the new block and mkae block.ptr point to the same location as
   * newBlock.ptr*/
  if (newBlock._ptr) {
    // copy contents from block.ptr to the result
    memcpy(newBlock._ptr, block._ptr, block._size);

    /* deallocate from oritinal allocator since the contents of the block are
     * now in memeory managed by newAllocator */
    originalOwner.deallocate(block);

    // update the size and ptr of block
    block._size = size_require;
    block._ptr = newBlock._ptr;
    return true;
  }
  return false;
}
}; // namespace dube