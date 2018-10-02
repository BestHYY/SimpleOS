#ifndef _HEAP_H_
#define _HEAP_H_

#include "simple_def.h"

typedef struct _Heap_Node_t
{
    uint32_t Next;
    uint32_t Prev;
}Heap_Node_t;
uint32_t SimpleOS_initHeap(uint32_t StartAddr, uint32_t HeapSizeinBytes);
uint32_t SimpleOS_Heap_add(uint32_t StartAddr, uint32_t HeapSizeinBytes);
void *SimpleOS_malloc(uint32_t Size);
void SimpleOS_free(void *p);
void SimpleOS_getHeapInfo(uint32_t *HeapSize, uint32_t *Used, uint32_t *Unused);

#endif
