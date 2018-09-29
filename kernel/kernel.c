#include "simpleos.h"
Kernel_t Kernel;

void SimpleOS_assertFailed()
{
    while(1);
}

SIMPLE_INLINE Kernel_t *SimpleOS_getKernel(void)
{
    return &Kernel;
}

uint32_t SimpleOS_Kernel_init(void)
{
    SimpleOS_initScheduler();
    SimpleOS_IdleThread_init();
    Kernel.IdleTime = 0;
    Kernel.TimeStamp = 0;
    return SIMPLE_ERR_OK;
}

uint32_t SimpleOS_wakeSleepThread(void)
{

    Thread_t *Thread;

    Thread = Scheduler.SleepingList.Head;
    while(Thread != SIMPLE_NULL)
    {
        Thread->SleepTime -= 1;
        if(Thread->SleepTime == 0)
        {
            if(Thread->Prev == SIMPLE_NULL)
            {
                if(Thread->Next == SIMPLE_NULL)
                {
                    Scheduler.SleepingList.Head = SIMPLE_NULL;
                    Scheduler.SleepingList.Tail = SIMPLE_NULL;
                }
                else
                {
                    Thread->Next->Prev = SIMPLE_NULL;
                    Scheduler.SleepingList.Head = Thread->Next;
                }
            }
            else
            {
                if(Thread->Next == SIMPLE_NULL)
                {
                    Thread->Prev->Next = SIMPLE_NULL;
                    Scheduler.SleepingList.Tail = Thread->Prev;
                }
                else
                {
                    Thread->Prev->Next = Thread->Next;
                    Thread->Next->Prev = Thread->Prev;
                }
            }
            SimpleOS_setThreadReady(Thread);
        }
        Thread = Thread->Next;
    }
    return SIMPLE_ERR_OK;
}

void SimpleOS_TimeTickHandler(void)
{
    SimpleOS_checkIPCList(&Scheduler.IPCList);
    SimpleOS_wakeSleepThread();
}
