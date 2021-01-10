#pragma once
#include <stdlib.h>
#include "memory_blk.h"


template<typename Primary, typename Fallback>
class FallbackAllocator : private Primary, private Fallback {
    /*
    Tries to allocate with primary and then will then use fallbac
    We are designing with composibility
    */
    public:
     Blk allocate(size_t n){
        Blk r = Primary::allocate(n);
        if(!r._ptr) r = Fallback::allocate(n);
       
        return r;
     }

     void deallocate(Blk b){
         //I want to check which between P and F owns memory b and then call the one that I want
        // we can add a new primitive bool owns(Blk) that must be defined in primary
        // fallback does not necesarily have to implement owns
        // FallbackAllocator will have to implement owns if both implement it (Good Citizenry)
        if(Primary::owns(b)) Primary::deallocate(b);
        else Fallback::deallocate(b);
     }

     bool owns(Blk b){
        //Relies on Method defination Failure is not an Error
        return Primary::owns(b) || Fallback::owns(b);
     }
};