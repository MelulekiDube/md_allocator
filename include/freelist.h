#pragma once

#include <stdlib.h>
#include "memory_blk.h"

template<class A, size_t s> class FreeList {
    A parent_;
    struct Node {Node* next;} root_;

    public:
    Blk allocate(size_t n){
        if(n == s && root_){
            Blk b = {root, _n};
            root_ = root_.next;
            return b;
        }
        return parent_.allocate(n);
    }

    void deallocate(Blk b){
        if(b._size != s) return parent_.deallocate(b);
        auto p = (Node*) b._ptr;

        p.next = root_;
        root_ = p;
    }

    bool owns(Blk b){
        return b._size == s || parent_.owns(b);
    }
};

/*
we can add max_size of list
we also allocate more than we need (bigger chunks and store the rest in memory)

Also have a an upper bound of default size of chunk
*/ 