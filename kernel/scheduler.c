#include "simpleos.h"
Scheduler_t Scheduler;

void SimpleOS_initScheduler(void)
{
    Scheduler.ReadyThread = SIMPLE_NULL;
    Scheduler.RunningThread = SIMPLE_NULL;
    Scheduler.ReadyPriority = SIMPLE_PRIORITY_MAX - 1;
    Scheduler.ReadyGroup = 0x00000000;
    for(uint32_t i = 0 ; i < 8 ; i ++)
    {
        Scheduler.ReadyItem[i] = 0;
    }
    Scheduler.CriticalNestLevel = 0;
    Scheduler.InterruptNestLevel = 0;
    Scheduler.Status = SIMPLE_STATUS_INIT;
    for(uint32_t i = 0 ; i < SIMPLE_PRIORITY_MAX ; i ++)
    {
        Scheduler.ReadyList[i].Head = SIMPLE_NULL;
        Scheduler.ReadyList[i].Tail = SIMPLE_NULL;
    }
    Scheduler.SleepingList.Head = SIMPLE_NULL;
    Scheduler.SleepingList.Tail = SIMPLE_NULL;
    Scheduler.DeletedList.Head = SIMPLE_NULL;
    Scheduler.DeletedList.Tail = SIMPLE_NULL;
    SimpleOS_IPC_initIPCList(&Scheduler.IPCList);
}

Thread_t *SimpleOS_getCurrentThread(void)
{
    return Scheduler.RunningThread;
}

Thread_t *SimpleOS_getHighReadyThread(void)
{
    SimpleOS_Scheduler_getHighReadyThread();
    return Scheduler.ReadyThread;
}

void SimpleOS_addThreadtoList(Thread_Chain_t *Chain,Thread_t *Thread)
{
    if(!Chain)
        return;
    if(!Thread)
        return;
    if(Chain->Head == SIMPLE_NULL)
    {
        Chain->Head = Thread;
        Chain->Tail = Thread;
        Thread->Prev = SIMPLE_NULL;
        Thread->Next = SIMPLE_NULL;
    }
    else
    {
        Chain->Tail->Next = Thread;
        Thread->Prev = Chain->Tail;
        Thread->Next = SIMPLE_NULL;
        Chain->Tail = Thread;
    }
}

void SimpleOS_removeThreadfromList(Thread_Chain_t *Chain, Thread_t *Thread)
{
    if(!Chain)
        return;
    if(!Thread)
        return;
    if(Thread->Prev == SIMPLE_NULL)
    {
        if(Thread->Next == SIMPLE_NULL)
        {
            Chain->Head = SIMPLE_NULL;
            Chain->Tail = SIMPLE_NULL;
        }
        else
        {
            Thread->Next->Prev = SIMPLE_NULL;
            Chain->Head = Thread->Next;
        }
    }
    else
    {
        if(Thread->Next == SIMPLE_NULL)
        {
            Thread->Prev->Next = SIMPLE_NULL;
            Chain->Tail = Thread->Prev;
        }
        else
        {
            Thread->Prev->Next = Thread->Next;
            Thread->Next->Prev = Thread->Prev;
        }
    }
}

void SimpleOS_startThread(Thread_t *Thread)
{
    uint32_t Priority;
    if(Thread->Status == SIMPLE_STATUS_INIT)
    {
        Thread->Status = SIMPLE_STATUS_READY;                       //Set Thread Status to Ready
        Thread->RemainTimePiece = Thread->SetTimePiece;             //Restart round robin tick piece
        Priority = Thread->CurrentPriority;                         //Set Priority Bit
        Scheduler.ReadyGroup |= 1 << (Priority / 32);
        Scheduler.ReadyItem[Priority / 32] |= 1 << ((Priority % 32));
        SimpleOS_addThreadtoList(&Scheduler.ReadyList[Priority],Thread);
    }
}

void SimpleOS_setThreadReady(Thread_t *Thread)
{
    uint32_t Priority;
    Thread->Status = SIMPLE_STATUS_READY;
    Thread->RemainTimePiece = Thread->SetTimePiece;
    Priority = Thread->CurrentPriority;
    Scheduler.ReadyGroup |= Priority / 32;
    Scheduler.ReadyItem[Scheduler.ReadyGroup - 1] |= 1<< (Priority % 32);
    SimpleOS_addThreadtoList(&Scheduler.ReadyList[Priority],Thread);
    SimpleOS_getHighReadyThread();
}

void SimpleOS_setThreadSuspend(Thread_t *Thread)
{
    uint32_t Priority;
    uint32_t Group,Item;
    Priority = Thread->CurrentPriority;
    Group = Priority / 32;
    Item = Priority % 32;
    Thread->Status = SIMPLE_STATUS_SUSPEND;
    SimpleOS_removeThreadfromList(&Scheduler.ReadyList[Priority],Thread);
    if(Scheduler.ReadyList[Priority].Head == SIMPLE_NULL)         //No same priority thread ready
    {
        Scheduler.ReadyItem[Group] &= ~(1 << Item);
        if(Scheduler.ReadyItem[Group] == 0x00000000)              //This Group has no ready thread
        {
            Scheduler.ReadyGroup &= ~(1 << Group);
        }
    }
    SimpleOS_getHighReadyThread();
}

void SimpleOS_resumeThread(Thread_t *Thread)
{
    SimpleOS_setThreadReady(Thread);
    SimpleOS_schedule();
}

void SimpleOS_suspendThread(Thread_t *Thread)
{
    SimpleOS_setThreadSuspend(Thread);
    SimpleOS_schedule();
}

void SimpleOS_deleteThread(Thread_t *Thread)
{
    if(Thread->Status==SIMPLE_STATUS_READY)                  //Target thread is ready for schdule
    {
        SimpleOS_suspendThread(Thread);
    }
    Thread->Status = SIMPLE_STATUS_EXIT;
    SimpleOS_addThreadtoList(&Scheduler.DeletedList,Thread);
}

void SimpleOS_startScheduler(void)
{
    Scheduler.Status = SIMPLE_STATUS_RUN;
    SimpleOS_schedule();
}

void SimpleOS_pauseScheduler(void)
{
    Scheduler.Status = SIMPLE_STATUS_SUSPEND;
}

SIMPLE_INLINE void SimpleOS_schedule(void)
{
    SimpleOS_disableInterrupt();
    SimpleOS_Scheduler_getHighReadyThread();
    if((Scheduler.CriticalNestLevel == 0) && (Scheduler.ReadyThread != Scheduler.RunningThread))
    {
        SimpleOS_enableInterrupt();
        SimpleOS_TriggerContextSwitch();
    }
    SimpleOS_enableInterrupt();
}
