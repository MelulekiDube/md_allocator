#pragma once

#include "memory_blk.h"
#include <stdlib.h>
#include <algorithm>    // std::max

#define WORD_SIZE 4

template <class A, size_t s> class FreeList {
  A parent_;
  struct Node {
    Node *next;
  } root_;

public:
  FreeList(A allocator) : parent_(allocator) { root_ = nullptr; }

  static constexpr unsigned align();
  static constexpr size_t goodsize(size_t n) {
    return std::max(n, s);
  }

  Block allocate(size_t n) {
    if (n == s && root_) {
      Block b = {root_, n};
      root_ = root_.next;
      return b;
    }
    return parent_.allocate(n);
  }

  void deallocate(Block &b) {
    if (b._size != s)
      return parent_.deallocate(b);
    auto p = (Node *)b._ptr;

    p.next = root_;
    root_ = p;
  }

  bool expand(Block &block, size_t n) {
    Block temp;
    if(n <= block._size) return false; // we cant expand to same size or less size
    if (block._size == s) { // if it originally belonged here we deallocate and
      // then use parent to allocate more space
      temp = parent_.allocate(n);
      if(!temp._ptr) return false;
      memcpy(temp._ptr, block._ptr, std::min(s, n));
      deallocate(block);
      return true;
    }
    if (n == s) {
      if(root_) {
        temp = {root_, n};
        root_ = root_.next;
        memcpy(temp._ptr, block._ptr, std::min(block._size, s));
        parent_.deallocate(block);
        return true;
      }
    }
    temp = parent_.expand(block, n); // otherwise we just call the expand from the parent_
    if(!temp._ptr) return false;
    memcpy(temp._ptr, block._ptr, std::min(block._size, n));
    return true;
  }

  void reallocate(Block &block, size_t n) {
    Block temp;
    if(n == block._size) return ; // we cant expand to same size
    if (block._size == s) { // if it originally belonged here we deallocate and
      // then use parent to allocate more space
      temp = parent_.allocate(n);
      if(!temp._ptr) return ;
      memcpy(temp._ptr, block._ptr, std::min(s, n));
      deallocate(block);
      return;
    }
    if (n == s) {
      if(root_) {
        temp = {root_, n};
        root_ = root_.next;
        memcpy(temp._ptr, block._ptr, std::min(block._size, s));
        parent_.deallocate(block);
        return ;
      }
    }
    temp = parent_.expand(block, n); // otherwise we just call the expand from the parent_
    if(!temp._ptr) return;
    memcpy(temp._ptr, block._ptr, std::min(block._size, n));
  }

  void deallocateAll() {
    // since we get all the blocks from parent allocator we can just call it
    // here
    parent_.deallocateAll();
    root_ = nullptr;
    ;
  }

  Block alignedAllocate(size_t n) {
    if (n == s && root_) {
      Node* current = root_, *prev = nullptr;
      while(current){
        if(!(current%WORD_SIZE)) break;
        prev = current;
        current = current->next;
      }
      if(current){
        prev->next = current+n;
        return {current, n};
      }
    }
    return parent_.alignedAllocate(n);
  }

  Block alignedReAllocate(Block &block, size_t new_size) {

  }

  bool owns(const Block &block) const {
    return block._size == s || parent_.owns(block);
  }
};

/*
we can add max_size of list
we also allocate more than we need (bigger chunks and store the rest in memory)

Also have a an upper bound of default size of chunk
*/