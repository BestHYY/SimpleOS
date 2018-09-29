#ifndef _SIMPLE_DEF_H_
#define _SIMPLE_DEF_H_
#include "stdint.h"
#include "simple_cfg.h"
#include "simple_cdef.h"
class Scheduler_t;

class Thread_t;

extern Scheduler_t Scheduler;

 /*
  * Thread Class Defination
  */
class Thread_t
{
    friend class Scheduler_t;
    friend class Semaphore_Chain_t;
    friend class MailBox_Chain_t;
    friend class Mutex_Chain_t;
    friend class EventGroup_Chain_t;
    friend class Queue_Chain_t;
    
private:
    uint32_t *SP;              //Stack Pointer
    uint32_t *Stack;           //Stack
    void     *parameter;       //Parameter Sended to thread
    uint32_t SetPriority;      //Initiate Priority
    uint32_t CurrentPriority;  //Current Priority
    uint32_t Status;           //Thread Status
    uint32_t ErrorCode;        //Thread ErrorCode
    uint32_t SetTickNum;       //Thread Tick Num 
    uint32_t RemainTickNum;    //Thread Remain Tick
    uint32_t SleepTime;        //Thread Sleeping Time Last

    uint32_t EventSet;
    uint32_t Option;

    Thread_t *Next;
    Thread_t *Prev;
public:
    char Name[SIMPLE_NAME_MAX];

private:
    uint32_t *setStack(uint32_t StackSizeinBytes);
    uint32_t *setStack(uint32_t* Stack,uint32_t StackSizeinBytes);
    uint32_t setName(char *Name);
    uint32_t setPriority(uint32_t Priority);
    uint32_t setTickNum(uint32_t Ticknum);
    uint32_t changePriority(uint32_t Priority);
    uint32_t recoverPriority(void);

    virtual void run(void);
    virtual void exit(void);
public:
    Thread_t(uint32_t StackSizeinBytes = SIMPLE_DEFAULT_STACK_SIZE);
    Thread_t(char *name, uint32_t Priority, uint32_t TickNum, uint32_t StackSizeinBytes = SIMPLE_DEFAULT_STACK_SIZE);
    Thread_t(char *name, uint32_t Priority, uint32_t TickNum, uint32_t* Stack, uint32_t StackSizeinBytes);
    static void exec(Thread_t *Thread);
    void start(void);
    void sleep(uint32_t ticks);
};

typedef struct _Thread_List_t
{
    Thread_t *Head;
    Thread_t *Tail;
} ThreadList__t;

typedef struct _IPCList_t
{
    IPC_t *Head;
    IPC_t *Tail;
}IPCList_t;


/*
 * Scheduler Class
 */
class Scheduler_t
{
    friend class Thread_t;
private:
    Thread_t *ReadyThread;
    Thread_t *RunningThread;
    uint32_t ReadyPriority;
    uint32_t ReadyGroup;
    uint32_t ReadyItem[8];
    uint32_t CriticalLevel;
    Thread_List_t ReadyList[SIMPLE_PRIORITY_MAX];
    Thread_List_t SleepingList;
    IPCList_t IPCList;

    uint32_t Status;
public:
    Scheduler_t();
    Thread_t *getHighReady(void);
    static Scheduler_t *getScheduler(uint32_t Id = 0);
    static void schedule(void);
    Thread_t *getCurrentThread(void);
    void setThreadReady(Thread_t *Thread);
    void setThreadSuspend(Thread_t *Thread);
    void startThread(Thread_t *Thread);
    void suspendThread(Thread_t *Thread);
    void resumeThread(Thread_t *Thread);
    void deleteThread(Thread_t *Thread);
    void start(void);
    void pause(void);
 };


#endif
