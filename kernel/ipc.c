#include "simpleos.h"

ParameterPipe_t Pipe;

uint32_t SimpleOS_IPC_initIPC(IPC_t *IPC, const char *Name)
{
    uint32_t i = 0;
    if((!IPC) || (!Name))
        return SIMPLE_ERR_ILIGALARG;
    for(i = 0 ; i < SIMPLE_NAME_MAX ; i ++)
    {
        if(Name[i])
            IPC->Name[i] = Name[i];
        else
        {
            IPC->Name[i] = 0;
            break;
        }        
    }
    IPC->Head = SIMPLE_NULL;
    IPC->Tail = SIMPLE_NULL;
    IPC->Prev = SIMPLE_NULL;
    IPC->Next = SIMPLE_NULL;
    return SIMPLE_ERR_OK;
}

uint32_t SimpleOS_IPC_suspendThread(IPC_t *IPC, Thread_t *Thread, uint32_t Time)
{
    SimpleOS_setThreadSuspend(Thread);
    Thread->SleepTime= Time;
    if(IPC->Next == SIMPLE_NULL)
    {
        IPC->Head = Thread;
        IPC->Tail = Thread;
        Thread->Next = SIMPLE_NULL;
        Thread->Prev = SIMPLE_NULL;
    }
    else
    {
        Thread->Next = SIMPLE_NULL;
        Thread->Prev = IPC->Tail;
        IPC->Tail->Next = Thread;
        IPC->Tail = Thread;
    }
    return SIMPLE_ERR_OK;
}

uint32_t SimpleOS_IPC_resumeThread(IPC_t *IPC, Thread_t *Thread, uint32_t ErrorCode)
{
    if(Thread->Prev == SIMPLE_NULL)
    {
        if(Thread->Next == SIMPLE_NULL)
        {
            IPC->Head = SIMPLE_NULL;
            IPC->Tail = SIMPLE_NULL;
        }
        else
        {
            Thread->Next->Prev = SIMPLE_NULL;
            IPC->Head = Thread->Next;
        }
    }
    else
    {
        if(Thread->Next == SIMPLE_NULL)
        {
            Thread->Prev->Next = SIMPLE_NULL;
            IPC->Tail = Thread->Prev;
        }
        else
        {
            Thread->Prev->Next = SIMPLE_NULL;
            Thread->Next->Prev = SIMPLE_NULL;
        }
    }
    SimpleOS_setThreadReady(Thread);
    return SIMPLE_ERR_OK;
}

uint32_t SimpleOS_IPC_initIPCList(IPC_List_t *IPCList)
{
    IPCList->Semaphore.Head = SIMPLE_NULL;
    IPCList->Semaphore.Tail = SIMPLE_NULL;

    IPCList->Mailbox.Head = SIMPLE_NULL;
    IPCList->Mailbox.Tail = SIMPLE_NULL;

    IPCList->Mutex.Head = SIMPLE_NULL;
    IPCList->Mutex.Tail = SIMPLE_NULL;

    IPCList->Eventgroup.Head = SIMPLE_NULL;
    IPCList->Eventgroup.Tail = SIMPLE_NULL;

    IPCList->Queue.Head = SIMPLE_NULL;
    IPCList->Queue.Tail = SIMPLE_NULL;
    IPCList->Pipe = &Pipe;
    return SIMPLE_ERR_OK;
}

uint32_t SimpleOS_IPC_addIPCNode(IPC_Chain_t *IPCChain,IPC_t *IPC)
{
    if(IPCChain->Head == SIMPLE_NULL)
    {        
        IPC->Next = SIMPLE_NULL;
        IPC->Prev = SIMPLE_NULL;
        IPCChain->Head = IPC;
        IPCChain->Tail = IPC;
    }
    else
    {
        IPC->Next = SIMPLE_NULL;
        IPC->Prev = IPCChain->Tail;
        IPCChain->Tail->Next = IPC;
        IPCChain->Tail = IPC;
    }
    return SIMPLE_ERR_OK;
}

uint32_t SimpleOS_IPC_removeIPCNode(IPC_Chain_t *IPCChain,IPC_t *IPC)
{
    if(IPC->Prev == SIMPLE_NULL)
    {
        if(IPC->Next == SIMPLE_NULL)
        {
            IPCChain->Head = SIMPLE_NULL;
            IPCChain->Tail = SIMPLE_NULL;
        }
        else
        {
            IPC->Next->Prev = SIMPLE_NULL;
            IPCChain->Head = IPC->Next;
        }
    }
    else
    {
        if(IPC->Next == SIMPLE_NULL)
        {
            IPC->Prev->Next = SIMPLE_NULL;
            IPCChain->Tail = IPC->Prev;
        }
        else
        {
            IPC->Prev->Next = IPC->Next;
            IPC->Next->Prev = IPC->Prev;
        }
    }
    return SIMPLE_ERR_OK;
}

SIMPLE_INLINE uint32_t SimpleOS_IPC_checkChain(IPC_Chain_t *Chain)
{
    IPC_t *IPC;
    Thread_t *Thread;
    for(IPC = Chain->Head; IPC; IPC = IPC->Next)
    {
        Thread = IPC->Head;
        for(Thread = IPC->Head; Thread ; Thread = Thread->Next)
        {
            Thread->SleepTime -= 1;
            if(Thread->SleepTime == 0)
            {
                SimpleOS_IPC_resumeThread(IPC,Thread,SIMPLE_ERR_TIMEOUT);
            }
        }
    }
    return SIMPLE_ERR_OK;
}

SIMPLE_INLINE uint32_t SimpleOS_IPC_checkParameterPipe(ParameterPipe_t *Pipe)
{
    Thread_t *Thread;
    Thread = Pipe->Head;
    while(Thread != SIMPLE_NULL)
    {
        Thread->SleepTime -= 1;
        if(Thread->SleepTime == 0)
        {
            SimpleOS_IPC_resumeThread(Pipe,Thread,SIMPLE_ERR_TIMEOUT);
        }
        Thread = Thread->Next;
    }
    return SIMPLE_ERR_OK;
}

uint32_t SimpleOS_checkIPCList(IPC_List_t *IPC_List)
{
    SimpleOS_IPC_checkChain(&IPC_List->Semaphore);
    SimpleOS_IPC_checkChain(&IPC_List->Mailbox);
    SimpleOS_IPC_checkChain(&IPC_List->Mutex);
    SimpleOS_IPC_checkChain(&IPC_List->Eventgroup);
    SimpleOS_IPC_checkChain(&IPC_List->Queue);
    SimpleOS_IPC_checkParameterPipe(IPC_List->Pipe);
    return SIMPLE_ERR_OK;
}


/********************************************************************* Semaphore *********************************************************************/

uint32_t SimpleOS_IPC_initIPCSemaphore(Semaphore_t *Semaphore, const char *Name, uint32_t InitValue)
{
    uint32_t Res;
    SimpleOS_enterCritical();
    Res = SimpleOS_IPC_initIPC(&Semaphore->Parent,Name);
    if(Res)
    {
        SimpleOS_exitCritical();
        return Res;
    }
    Semaphore->SemaphoreCount = InitValue;
    SimpleOS_IPC_addIPCNode(&Scheduler.IPCList.Semaphore,&Semaphore->Parent);
    SimpleOS_exitCritical();
    return SIMPLE_ERR_OK;
}

uint32_t SimpleOS_IPC_pendSemaphore(Semaphore_t *Semaphore, uint32_t Time)
{
    Thread_t *Thread;
    if(!Semaphore)
        return SIMPLE_ERR_ILIGALARG;
    SimpleOS_enterCritical();
    if(Semaphore->SemaphoreCount > 0)
    {
        Semaphore->SemaphoreCount -= 1;
        SimpleOS_exitCritical();
        return SIMPLE_ERR_OK;
    }
    if(Time == 0)
    {
        SimpleOS_exitCritical();
        return SIMPLE_ERR_TIMEOUT;
    }
    Thread = SimpleOS_getCurrentThread();
    SimpleOS_IPC_suspendThread(&Semaphore->Parent,Thread,Time);
    SimpleOS_exitCritical();
    SimpleOS_schedule();
    if(Semaphore->SemaphoreCount == 0)
        return SIMPLE_ERR_TIMEOUT;
    Semaphore->SemaphoreCount -= 1;
    return SIMPLE_ERR_OK;
}

uint32_t SimpleOS_IPC_sendSemaphore(Semaphore_t *Semaphore)
{
    SimpleOS_enterCritical();
    Semaphore->SemaphoreCount += 1;
    if(Semaphore->Parent.Head != SIMPLE_NULL)                              //Some one is waiting Sem
    {  
        SimpleOS_IPC_resumeThread(&Semaphore->Parent,Semaphore->Parent.Head,SIMPLE_ERR_OK);
        SimpleOS_exitCritical();
        SimpleOS_schedule();
    }
    SimpleOS_exitCritical();
    return SIMPLE_ERR_OK;
}

/********************************************************************* Mailbox *********************************************************************/

uint32_t SimpleOS_IPC_initMailbox(Mailbox_t *Mailbox, const char *Name)
{
    uint32_t Res;
    SimpleOS_enterCritical();
    Res = SimpleOS_IPC_initIPC(&Mailbox->Parent,Name);
    if(Res)
    {
        SimpleOS_exitCritical();
        return Res;
    }
    Mailbox->Mail = 0x00000000;
    Mailbox->Status = SIMPLE_STATUS_EMPTY;
    SimpleOS_IPC_addIPCNode(&Scheduler.IPCList.Mailbox,&Mailbox->Parent);
    SimpleOS_exitCritical();
    return SIMPLE_ERR_OK;
}
    
uint32_t SimpleOS_IPC_pendMail(Mailbox_t *Mailbox, void *Address, uint32_t Time)
{
    Thread_t *Thread;
    SimpleOS_enterCritical();
    if(Mailbox->Status == SIMPLE_STATUS_EMPTY)
    {
        if(Time == 0)
        {
            SimpleOS_exitCritical();
            return SIMPLE_ERR_TIMEOUT;
        }
        else
        {
            Thread = SimpleOS_getCurrentThread();
            SimpleOS_IPC_suspendThread(&Mailbox->Parent, Thread, Time);
            SimpleOS_exitCritical();
            SimpleOS_schedule();
        }
    }
    SimpleOS_enterCritical();
    if(Mailbox->Status != SIMPLE_STATUS_FULL)
    {
        SimpleOS_exitCritical();
        return SIMPLE_ERR_TIMEOUT;
    }
    *(uint32_t*)Address = Mailbox->Mail;
    Mailbox->Mail = 0x00000000;
    Mailbox->Status = SIMPLE_STATUS_EMPTY;
    SimpleOS_exitCritical();
    return SIMPLE_ERR_OK;
    
}

uint32_t SimpleOS_IPC_sendMail(Mailbox_t *Mailbox, uint32_t Mail)
{
    if(!Mailbox)
        return SIMPLE_ERR_ILIGALARG;
    if(Mailbox->Status != SIMPLE_STATUS_EMPTY)
        return SIMPLE_ERR_FULL;
    SimpleOS_enterCritical();
    Mailbox->Status = SIMPLE_STATUS_FULL;
    Mailbox->Mail = Mail;
    if(Mailbox->Parent.Head != SIMPLE_NULL)                              //Some one is waiting Mail
    {
        SimpleOS_IPC_resumeThread(&Mailbox->Parent,Mailbox->Parent.Head,SIMPLE_ERR_OK);
        SimpleOS_exitCritical();
        SimpleOS_schedule();
    }
    SimpleOS_exitCritical();
    return SIMPLE_ERR_OK;
}

/********************************************************************* Mutex *********************************************************************/

uint32_t SimpleOS_IPC_initMutex(Mutex_t *Mutex, const char *Name)
{
    uint32_t Res;
    SimpleOS_enterCritical();
    Res = SimpleOS_IPC_initIPC(&Mutex->Parent,Name);
    if(Res)
    {
        SimpleOS_exitCritical();
        return SIMPLE_ERR_ILIGALARG;
    }
    Mutex->Holds = 0;
    Mutex->PendPriority = SIMPLE_PRIORITY_MAX - 1;
    Mutex->Owner = SIMPLE_NULL;
    SimpleOS_IPC_addIPCNode(&Scheduler.IPCList.Mutex,&Mutex->Parent);
    SimpleOS_exitCritical();
    return SIMPLE_ERR_OK;
}

uint32_t SimpleOS_IPC_pendMutex(Mutex_t *Mutex, uint32_t Time)
{
    Thread_t *Thread;
    SimpleOS_enterCritical();
    Thread = SimpleOS_getCurrentThread();
    if(Mutex->Owner == Thread)
    {
        Mutex->Holds += 1;
        SimpleOS_exitCritical();
        return SIMPLE_ERR_OK;
    }
    if(Mutex->Holds == 0)   //Mutex Available
    {
        Mutex->Owner = Thread;
        Mutex->PendPriority = Thread->CurrentPriority;
        Mutex->Holds = 1;
        SimpleOS_exitCritical();
        return SIMPLE_ERR_OK;
    }
    if(Time == 0)                 //No Waiting
    {
        SimpleOS_exitCritical();
        return SIMPLE_ERR_TIMEOUT;
    }
    else
    {
        if(Thread->CurrentPriority < Mutex->PendPriority)           //Raise Priority
        {
            SimpleOS_Thread_changePriority(Mutex->Owner,Thread->CurrentPriority);
        }
        SimpleOS_IPC_suspendThread(&Mutex->Parent,Thread,Time);
        SimpleOS_exitCritical();
        SimpleOS_schedule();
        SimpleOS_enterCritical();
        if(Mutex->Holds != 0)
        {
            SimpleOS_exitCritical();
            return SIMPLE_ERR_TIMEOUT;
        }
        Mutex->Owner = Thread;
        Mutex->PendPriority = Thread->CurrentPriority;
        Mutex->Holds = 1;
        SimpleOS_exitCritical();
        return SIMPLE_ERR_OK;
    }
}

uint32_t SimpleOS_IPC_sendMutex(Mutex_t *Mutex)
{
    Thread_t *Thread;
    SimpleOS_enterCritical();
    Thread = SimpleOS_getCurrentThread();
    if(Mutex->Owner != Thread)                  // Current Thread do not own the mutex
    {
        SimpleOS_exitCritical();
        return SIMPLE_ERR_ERR;
    }
    Mutex->Holds -= 1;                          //else, sub Mutex holds number
    if(Mutex->Holds == 0)
    {
        SimpleOS_Thread_recoverPriority(Mutex->Owner);
        if(Mutex->Parent.Head != SIMPLE_NULL)    //Some one is waiting
        {
            Thread = Mutex->Parent.Head;
            SimpleOS_IPC_resumeThread(&Mutex->Parent,Thread,SIMPLE_ERR_OK);
            SimpleOS_exitCritical();
            SimpleOS_schedule();
        }
        else
        {
            Mutex->Holds = 0;
            Mutex->Owner = SIMPLE_NULL;
            Mutex->PendPriority = 0;
        }
    }
    SimpleOS_exitCritical();
    return SIMPLE_ERR_OK;
}

/********************************************************************* Eventgroup *********************************************************************/

uint32_t SimpleOS_IPC_initEventgroup(Eventgroup_t *Eventgroup, const char *Name)
{
    uint32_t Res;
    SimpleOS_enterCritical();
    Res = SimpleOS_IPC_initIPC(&Eventgroup->Parent,Name);
    if(Res)
    {
        SimpleOS_exitCritical();
        return Res;
    }
    Eventgroup->EventSet = 0x00000000;
    SimpleOS_IPC_addIPCNode(&Scheduler.IPCList.Eventgroup,&Eventgroup->Parent);
    SimpleOS_exitCritical();
    return SIMPLE_ERR_OK;
}

uint32_t SimpleOS_IPC_pendEvent(Eventgroup_t *Eventgroup, uint32_t Event, uint32_t Flag, uint32_t Time)
{
    Thread_t *Thread;
    uint32_t EventReceived;
    uint32_t Result;
    SimpleOS_enterCritical();
    EventReceived = Event & Eventgroup->EventSet;
    Result = SIMPLE_FALSE;
    if(Flag & SIMPLE_EVENT_OR)              //Use OR method to get Event
    {
        if(EventReceived)
        {
            Result = SIMPLE_TRUE;
        }
    }
    else                                    //Use AND method to get Event
    {
        if(EventReceived == Event)
        {
            Result = SIMPLE_TRUE;
        }
    }
    if(Result)
    {
        if(Flag & SIMPLE_EVENT_CLEAR)
        {
            Eventgroup->EventSet &= ~EventReceived;
        }
        SimpleOS_exitCritical();
        return EventReceived;
    }
    if(Time == 0)                           //No Event get but time out
    {
        SimpleOS_exitCritical();
        return 0;
    }
    Thread = SimpleOS_getCurrentThread();
    Thread->IPCOpt = Flag;
    Thread->IPCSet = Event;
    Thread->ErrorCode = SIMPLE_ERR_OK;
    SimpleOS_IPC_suspendThread(&(Eventgroup->Parent), Thread, Time);           //Suspend current thread
    SimpleOS_exitCritical();
    SimpleOS_schedule();                                                    //Wait until timeout or Event satisfied
    SimpleOS_enterCritical();
    Result = SIMPLE_FALSE;
    EventReceived = 0;
    if(Flag & SIMPLE_EVENT_OR)              //Use OR method to get Event
    {
        if(EventReceived)
        {
            Result = SIMPLE_TRUE;
        }
    }
    else                                    //Use AND method to get Event
    {
        if(EventReceived == Event)
        {
            Result = SIMPLE_TRUE;
        }
    }
    if(Result)
    {
        if(Flag & SIMPLE_EVENT_CLEAR)
        {
            Eventgroup->EventSet &= ~EventReceived;
        }
    }
    SimpleOS_exitCritical();
    return EventReceived;
}
            
uint32_t SimpleOS_IPC_sendEvent(Eventgroup_t *Eventgroup, uint32_t Event)
{
    Thread_t *Thread;
    uint32_t Result;
    uint32_t ShouldSched;
    SimpleOS_enterCritical();
    Eventgroup->EventSet |= Event;
    if(Eventgroup->Parent.Head != SIMPLE_NULL)
    {
        ShouldSched = SIMPLE_FALSE;
        for(Thread = Eventgroup->Parent.Head; Thread; Thread = Thread->Next)
        {
            Result = SIMPLE_FALSE;
            if((Thread->IPCOpt & SIMPLE_EVENT_OR) && (Eventgroup->EventSet & Thread->IPCSet))
            {
                Result = SIMPLE_TRUE;
            }
            else if((Thread->IPCOpt & SIMPLE_EVENT_AND) && ((Eventgroup->EventSet & Thread->IPCSet) == Thread->IPCSet))
            {
                Result = SIMPLE_TRUE;
            }
            if(Result)
            {
                if(Thread->IPCOpt & SIMPLE_EVENT_CLEAR)
                {
                    Eventgroup->EventSet &= ~(Thread->IPCSet);
                }
                SimpleOS_IPC_resumeThread(&Eventgroup->Parent, Thread, 0);
                ShouldSched = SIMPLE_TRUE;
            }
        }
    }
    SimpleOS_exitCritical();
    if(ShouldSched)
    {
        SimpleOS_schedule();
    }
    return SIMPLE_ERR_OK;
}

/********************************************************************* Queue *********************************************************************/

uint32_t SimpleOS_IPC_initQueue(Queue_t *Queue, const char *Name, void *PoolAddr, uint32_t PoolSizeinBytes)
{
    uint32_t Res;
    Res = SimpleOS_IPC_initIPC(&Queue->Parent,Name);
    if(Res)
        return SIMPLE_ERR_ILIGALARG;
    SimpleOS_IPC_addIPCNode(&Scheduler.IPCList.Queue,&Queue->Parent);
    Queue->Pool = PoolAddr;
    Queue->PoolSize = PoolSizeinBytes;
    Queue->MessageHead = 0;
    Queue->MessageSize = 0;
    return SIMPLE_ERR_OK;
}

uint32_t SimpleOS_IPC_putQueue(Queue_t *Queue, void *Data, uint32_t Size)
{
    uint32_t Pos = Queue->MessageHead + Queue->MessageSize;
    for(uint32_t i = 0; i < Size; i ++)
    {
        ((uint8_t*)Queue->Pool)[Pos] = ((uint8_t*)Data)[i];
        Pos += 1;
        if(Pos >= Queue->PoolSize)
            Pos = 0;
    }
    Queue->MessageSize += Size;
    return SIMPLE_ERR_OK;
}

uint32_t SimpleOS_IPC_getQueue(Queue_t *Queue, void *Dst, uint32_t Size)
{
    uint32_t Pos = Queue->MessageHead;
    for(uint32_t i = 0; i < Size; i ++)
    {
        ((uint8_t*)Dst)[i] = ((uint8_t*)Queue->Pool)[Pos];
        if(Pos >= Queue->PoolSize)
            Pos = 0;
    }
    Queue->MessageHead = Pos;
    Queue->MessageSize = Queue->MessageSize - Size;
    return SIMPLE_ERR_OK;
}

uint32_t SimpleOS_IPC_pendQueue(Queue_t *Queue, void *Dst, uint32_t Lenth, uint32_t Time)
{
    Thread_t *Thread;
    if((!Queue) || (!Dst))
        return SIMPLE_ERR_ILIGALARG;
    if(Queue->PoolSize < Lenth)
        return SIMPLE_ERR_ILIGALARG;
    if(Queue->MessageSize >= Lenth)
        return SimpleOS_IPC_getQueue(Queue, Dst, Lenth);
    if(Time == 0)
        return SIMPLE_ERR_TIMEOUT;
    Thread = SimpleOS_getCurrentThread();
    Thread->IPCOpt = Lenth;
    SimpleOS_IPC_suspendThread(&Queue->Parent, Thread, Time);
    SimpleOS_schedule();
    if(Queue->MessageSize < Lenth)
        return SimpleOS_IPC_getQueue(Queue, Dst, Lenth);
    else
        return SIMPLE_ERR_TIMEOUT;
}

uint32_t SimpleOS_IPC_sendQueue(Queue_t *Queue, void *Src, uint32_t Lenth)
{
    if((!Queue) || (!Src))
        return SIMPLE_ERR_ILIGALARG;
    if((Queue->PoolSize - Queue->MessageSize) < Lenth)
        return SIMPLE_ERR_FULL;
    SimpleOS_IPC_putQueue(Queue, Src, Lenth);
    if(!Queue->Parent.Head)
    {
        Thread_t *Thread = Queue->Parent.Head;
        if(Thread->IPCOpt <= Queue->MessageSize)
        {
            SimpleOS_IPC_resumeThread(&Queue->Parent, Thread, SIMPLE_ERR_OK);
            SimpleOS_schedule();
        }
    }
    return SIMPLE_ERR_OK;
}

uint32_t SimpleOS_IPC_initParameterPipe(ParameterPipe_t *Pipe)
{
    SimpleOS_IPC_initIPC(Pipe,"Pipe");
    return SIMPLE_ERR_OK;
}
    
uint32_t SimpleOS_IPC_pendParameterPipe(Thread_t *Thread, uint32_t Time)
{
    if(Thread == SIMPLE_NULL)
        return SIMPLE_ERR_NOTEXIST;
    if(Time == 0)
        return SIMPLE_ERR_TIMEOUT;
    SimpleOS_enterCritical();
    SimpleOS_IPC_suspendThread(&Pipe,Thread,Time);
    Thread->IPCOpt = SIMPLE_PIPE_PENDING;
    SimpleOS_exitCritical();
    SimpleOS_schedule();
    
    return Thread->ErrorCode;
}

uint32_t SimpleOS_IPC_sendParameterPipe(Thread_t *Thread, void *Parameter)
{
    if(Thread == SIMPLE_NULL)
        return SIMPLE_ERR_NOTEXIST;
    SimpleOS_enterCritical();
    if(Thread->IPCOpt == SIMPLE_PIPE_PENDING)
    {
        Thread->Paramater = Parameter;
        SimpleOS_exitCritical();
        SimpleOS_schedule();
        return SIMPLE_ERR_OK;
    }
    else
    {
        SimpleOS_exitCritical();
        return SIMPLE_ERR_NOTEXIST;
    }
}
