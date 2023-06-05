#pragma once

/*-----------------
     Allocator
-------------------*/

class BaseAllocator
{
public:
    static void*    Alloc(int32 size);
    static void     Release(void* ptr);
};

/*-------------------
     StompAllocator
---------------------*/