#ifndef _SIMPLE_SCHEDULER_H_
#define _SIMPLE_SCHEDULER_H_

#ifdef __cplusplus
extern"C"{
#endif

#include "simple_def.h"
#include "simple_thread.h"
#include "simple_ipc.h"

typedef struct 
{
    Thread_t *Head;
    Thread_t *Tail;
} Thread_Chain_t;

typedef struct
{
    Thread_t        *ReadyThread;
    Thread_t        *RunningThread;
    uint32_t        ReadyPriority;
    uint32_t        ReadyGroup;
    uint32_t        ReadyItem[8];
    uint32_t        CriticalNestLevel;
    uint32_t        InterruptNestLevel;
    uint32_t        Status;
    Thread_Chain_t  ReadyList[SIMPLE_PRIORITY_MAX];
    Thread_Chain_t  SleepingList;
    Thread_Chain_t  DeletedList;
    IPC_List_t      IPCList;
}Scheduler_t;


extern Scheduler_t Scheduler;
void SimpleOS_initScheduler(void);
Thread_t *SimpleOS_getCurrentThread(void);
void SimpleOS_startThread(Thread_t *Thread);
void SimpleOS_deleteThread(Thread_t *Thread);
void SimpleOS_setThreadReady(Thread_t *Thread);
void SimpleOS_setThreadSuspend(Thread_t *Thread);
void SimpleOS_resumeThread(Thread_t *Thread);
void SimpleOS_suspendThread(Thread_t *Thread);
void SimpleOS_startScheduler(void);
void SimpleOS_pauseScheduler(void);
void SimpleOS_schedule(void);


#ifdef __cplusplus
}
#endif
#endif
