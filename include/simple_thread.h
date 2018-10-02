#ifndef _SIMPLE_THREAD_H_
#define _SIMPLE_THREAD_H_
#ifdef __cplusplus
extern"C"{
#endif
#include "simple_def.h"

typedef void (*Thread_Entry)(void * args);

typedef struct _Thread_t
{
    uint32_t *SP;
    uint32_t *StackBase;
    void     *Paramater;
    uint16_t SetPriority;
    uint16_t CurrentPriority;
    uint16_t Status;
    uint16_t ErrorCode;
    uint32_t SetTimePiece;
    uint32_t RemainTimePiece;
    uint32_t SleepTime;
    uint32_t IPCSet;
    uint32_t IPCOpt;
    
    void (*Run)(void * args);
    void (*Hook)(void);
    void (*ExtiHook)(void);
    
    char     Name[SIMPLE_NAME_MAX];
    struct _Thread_t *Next;
    struct _Thread_t *Prev;
}Thread_t;
uint32_t *SimpleOS_Thread_setStack(Thread_t *Thread, uint32_t *StackBase, uint32_t StackSize);
void SimpleOS_Thread_setParameter(Thread_t *Thread,void *parameter);
void SimpleOS_Thread_setName(Thread_t *Thread, const char *Name);
void SimpleOS_Thread_setEntry(Thread_t *Thread, Thread_Entry Run);
uint32_t SimpleOS_Thread_setPriority(Thread_t *Thread, uint32_t Priority);
uint32_t SimpleOS_Thread_setTimePiece(Thread_t *Thread, uint32_t TimePiece);
uint32_t SimpleOS_Thread_changePriority(Thread_t *Thread, uint32_t Priority);
uint32_t SimpleOS_Thread_recoverPriority(Thread_t *Thread);
uint32_t SimpleOS_Thread_init(Thread_t *Thread,Thread_Entry Entry,void *Parameter, uint32_t *StackBase, uint32_t StackSize, uint32_t Priority, uint32_t TimePiece, const char *Name);
void SimpleOS_Thread_sleep(uint32_t Tick);

#ifdef __cplusplus
}
#endif
#endif
