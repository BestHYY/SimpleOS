#include "heap.h"
struct 
{
    Heap_Node_t *Head;
    uint32_t HeapSize;
    uint32_t HeapUsed;
    uint32_t HeapUnUsed;
}Heap;

/*
 * Heap Flag Defination
 */
#define SIMPLE_HEAP_USED          0x00000001
#define SIMPLE_HEAP_UNUSED        0xFFFFFFFE

#define SIMPLE_HEAP_END           0x00000002

#define SIMPLE_HEAPISUSED(x)      ((x)->Next & SIMPLE_HEAP_USED)
#define SIMPLE_HEAPNEXT(x)        ((x)->Next & 0xFFFFFFFC)
#define SIMPLE_HEAPUSED(x)        ((x)->Next |= SIMPLE_HEAP_USED)
#define SIMPLE_HEAPUNUSED(x)      ((x)->Next &= SIMPLE_HEAP_UNUSED)
#define SIMPLE_HEAPNODESIZE(x)    (((x)->Next & SIMPLE_HEAP_UNUSED) - ((uint32_t)x) - sizeof(Heap_Node_t))

uint32_t SimpleOS_Heap_init(uint32_t StartAddr, uint32_t HeapSizeinBytes)
{
    StartAddr = SIMPLE_ALIGN(StartAddr, 4);
    HeapSizeinBytes = SIMPLE_ALIGN(HeapSizeinBytes,4);
    
    Heap.Head = (Heap_Node_t*)(StartAddr + HeapSizeinBytes - sizeof(Heap_Node_t));
    Heap.Head->Next = SIMPLE_NULL | SIMPLE_HEAP_END;
    Heap.Head->Prev = StartAddr;
    
    Heap.Head = (Heap_Node_t*)(Heap.Head->Prev);
    Heap.Head->Next = StartAddr + HeapSizeinBytes - sizeof(Heap_Node_t);
    Heap.Head->Prev = SIMPLE_NULL;
    
    return SIMPLE_ERR_OK;
}

uint32_t SimpleOS_Heap_add(uint32_t StartAddr, uint32_t HeapSizeinBytes)
{
    Heap_Node_t *Start,*End;
    StartAddr = SIMPLE_ALIGN(StartAddr, 4);
    HeapSizeinBytes = SIMPLE_ALIGN(HeapSizeinBytes,4);
    
    Start = (Heap_Node_t*)StartAddr;
    End = (Heap_Node_t*)(StartAddr + HeapSizeinBytes - sizeof(Heap_Node_t));
    Start->Next = (uint32_t)End;
    Start->Prev = SIMPLE_NULL;
    
    End->Next = (uint32_t)Heap.Head | SIMPLE_HEAP_END;
    End->Prev = (uint32_t)Start;
    Heap.Head->Prev = (uint32_t)End;
    
    Heap.Head = Start;
    return SIMPLE_ERR_OK;
}

void *SimpleOS_malloc(uint32_t Size)
{
    Heap_Node_t *Empty,*New;
    uint32_t NodeSize;
    Empty = Heap.Head;
    while(SIMPLE_HEAPNEXT(Empty) != SIMPLE_NULL)
    {
        if(Empty->Next & SIMPLE_HEAP_END)
        {
            Empty = (Heap_Node_t*)SIMPLE_HEAPNEXT(Empty);
        }
        if(SIMPLE_HEAPNODESIZE((Heap_Node_t*)Empty) < Size)
        {
            Empty = (Heap_Node_t*)SIMPLE_HEAPNEXT(Empty);
        }
        else
        {
            break;
        }
    }
    NodeSize = SIMPLE_HEAPNODESIZE((Heap_Node_t*)Empty);
    if(NodeSize < Size + sizeof(Heap_Node_t) + 4)
    {
        SIMPLE_HEAPUSED(Empty);
        Heap.HeapUnUsed -= NodeSize;
        Heap.HeapUsed += NodeSize;
    }
    else
    {
        New = (Heap_Node_t*)((uint32_t)Empty + Size);
        New->Next = (uint32_t)Empty->Next & SIMPLE_HEAP_UNUSED;
        New->Prev = (uint32_t)Empty;
        ((Heap_Node_t*)(New->Next))->Prev = (uint32_t)New;
        Empty->Next = ((uint32_t)New & SIMPLE_HEAP_USED);
        
        Heap.HeapUnUsed -= NodeSize + sizeof(Heap_Node_t);
        Heap.HeapUsed += NodeSize + sizeof(Heap_Node_t);
    }
    return (void*)(Empty + sizeof(Heap_Node_t));
}

void SimpleOS_free(void *p)
{
    Heap_Node_t *Block;
    Heap_Node_t *NextBlock,*PrevBlock;
    Block = (Heap_Node_t*)((uint32_t)p - sizeof(Heap_Node_t));
    NextBlock = (Heap_Node_t*)(Block->Next & SIMPLE_HEAP_UNUSED);
    PrevBlock = (Heap_Node_t*)(Block->Prev);
    
    SIMPLE_HEAPUNUSED(Block);
    Heap.HeapUnUsed += Block->Next - (uint32_t)Block;
    Heap.HeapUsed -= Block->Next - (uint32_t)Block;
    if(((NextBlock->Next & SIMPLE_HEAP_USED) == SIMPLE_NULL) && ((NextBlock->Next & SIMPLE_HEAP_END) == SIMPLE_NULL))
    {
        NextBlock = (Heap_Node_t*)(NextBlock->Next);
        Block->Next = NextBlock->Next & SIMPLE_HEAP_UNUSED;
        NextBlock->Prev = (uint32_t)Block;
        Heap.HeapUnUsed += sizeof(Heap_Node_t);
    }
    if((PrevBlock->Next & SIMPLE_HEAP_USED) == SIMPLE_NULL)
    {
        PrevBlock->Next = NextBlock->Next & SIMPLE_HEAP_UNUSED;
        NextBlock->Prev = (uint32_t)PrevBlock;
        Heap.HeapUnUsed += sizeof(Heap_Node_t);
    }
}

void SimpleOS_getHeapInfo(uint32_t *HeapSize, uint32_t *Used, uint32_t *Unused)
{
    if(HeapSize)
        *HeapSize = Heap.HeapSize;
    if(Used)
        *Used = Heap.HeapUsed;
    if(Unused)
        *Unused = Heap.HeapUnUsed;
}
