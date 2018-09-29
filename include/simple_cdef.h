#ifndef _SIMPLE_C_DEF_H_
#define _SIMPLE_C_DEF_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "simple_cfg.h"
#include "stdint.h"

#if defined(__CC_ARM)                         /* ARM Compiler */
    #define SIMPLE_INLINE           inline
#elif defined(__IAR_SYSTEMS_ICC__)            /* IAR Complier */
    #define SIMPLE_INLINE           inline
#elif defined(__GNUC__)                       /* GNU Complier */
    #define SIMPLE_INLINE           inline
#else
    #error "Can't Recognized Complier"
#endif

/*
 * SimpleOSOS Error Code Defination
 */
#define SIMPLE_ERR_OK            0x00
#define SIMPLE_ERR_ERR           0x01
#define SIMPLE_ERR_EMPTY         0x02
#define SIMPLE_ERR_FULL          0x04
#define SIMPLE_ERR_NOTEXIST      0x08
#define SIMPLE_ERR_TIMEOUT       0x10
#define SIMPLE_ERR_ILIGALARG     0x20



/*
 * Status Defination
 */
#define SIMPLE_STATUS_INIT        0x00
#define SIMPLE_STATUS_READY       0x01
#define SIMPLE_STATUS_RUN         0x02
#define SIMPLE_STATUS_SUSPEND     0x04
#define SIMPLE_STATUS_EXIT        0x08

#define SIMPLE_STATUS_EMPTY       0x00
#define SIMPLE_STATUS_FULL        0xFFFFFFFF
/*
 * IPC Status Defination
 */
#define SIMPLE_IPC_EMPTY          0x00
#define SIMPLE_IPC_READY          0x01
#define SIMPLE_IPC_FULL           0x02

/*
 * Eventgroup Option Defination
 */
#define SIMPLE_EVENT_AND          0x01
#define SIMPLE_EVENT_OR           0x02
#define SIMPLE_EVENT_CLEAR        0x04

/*
 * ParameterPipe Pending Flag
 */
#define SIMPLE_PIPE_PENDING       0x08



#define SIMPLE_WAIT_LONGEST       0xFFFFFFFF

#define SIMPLE_FALSE              (0)
#define SIMPLE_TRUE               (~SIMPLE_FALSE)

#define SIMPLE_NULL               (0)
#define SIMPLE_UNUSED(p)          ((void)(p))

typedef void (*Thread_Entry)(void * args);
#define SIMPLE_ALIGN(x,y)         ((x+y-1) & y)

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

typedef struct _IPC_t
{
    struct _IPC_t *Prev;
    struct _IPC_t *Next;
    Thread_t *Head;
    Thread_t *Tail;
    char Name[SIMPLE_NAME_MAX];
}IPC_t;
uint32_t SimpleOS_IPC_initIPC(IPC_t *IPC, const char *Name);
uint32_t SimpleOS_IPC_suspendThread(IPC_t *IPC, Thread_t *Thread, uint32_t Time);
uint32_t SimpleOS_IPC_resumeThread(IPC_t *IPC, Thread_t *Thread, uint32_t ErrorCode);

typedef struct _Semaphore_t
{
    IPC_t Parent;
    uint32_t SemaphoreCount;
}Semaphore_t;
uint32_t SimpleOS_IPC_initIPCSemaphore(Semaphore_t *Semaphore, const char *Name, uint32_t InitValue);
uint32_t SimpleOS_IPC_pendemaphore(Semaphore_t *Semaphore, uint32_t Time);
uint32_t SimpleOS_IPC_sendSemaphore(Semaphore_t *Semaphore);

typedef struct _Mailbox_t
{
    IPC_t Parent;
    uint32_t Mail;
    uint32_t Status;
}Mailbox_t;
uint32_t SimpleOS_IPC_initMailbox(Mailbox_t *Mailbox, const char *Name);
uint32_t SimpleOS_IPC_pendMail(Mailbox_t *Mailbox, void *Address, uint32_t Time);
uint32_t SimpleOS_IPC_sendMail(Mailbox_t *Mailbox, uint32_t Mail);

typedef struct _Mutex_t
{
    IPC_t Parent;
    uint32_t PendPriority;
    Thread_t *Owner;
    uint32_t Holds;
}Mutex_t;
uint32_t SimpleOS_IPC_initMutex(Mutex_t *Mutex, const char *Name);
uint32_t SimpleOS_IPC_pendMutex(Mutex_t *Mutex, uint32_t Time);
uint32_t SimpleOS_IPC_sendMutex(Mutex_t *Mutex);

typedef struct _Eventgroup_t
{
    IPC_t Parent;
    uint32_t EventSet;
    uint32_t Status;
}Eventgroup_t;
typedef struct
{
    uint32_t Event;
    uint32_t *Dst;
    uint32_t Flag;
}Eventgroup_Option;
uint32_t SimpleOS_IPC_initEventgroup(Eventgroup_t *Eventgroup, const char *Name);
uint32_t SimpleOS_IPC_pendEvent(Eventgroup_t *Eventgroup, uint32_t Event,uint32_t Flag, uint32_t Time);
uint32_t SimpleOS_IPC_sendEvent(Eventgroup_t *Eventgroup, uint32_t Event);

typedef struct _Queue_t
{
    IPC_t Parent;
    void *Pool;
    uint32_t PoolSize;
    uint32_t MessageHead;
    uint32_t MessageSize;
}Queue_t;
uint32_t SimpleOS_IPC_initQueue(Queue_t *Queue, const char *Name, void *PoolAddr, uint32_t PoolSizeinBytes);
uint32_t SimpleOS_IPC_initQueueComplete(Queue_t *Queue);
uint32_t SimpleOS_IPC_putQueue(Queue_t *Queue, void *Data, uint32_t Size);
uint32_t SimpleOS_IPC_getQueue(Queue_t *Queue, void *Dst, uint32_t Size);
uint32_t SimpleOS_IPC_pendQueue(Queue_t *Queue, void *Dst, uint32_t Lenth, uint32_t Time);
uint32_t SimpleOS_IPC_receiveQueue(Queue_t *Queue, void *Dst, uint32_t Lenth, uint32_t Time);
uint32_t SimpleOS_IPC_sendQueue(Queue_t *Queue, void *Src, uint32_t Lenth);
uint32_t SimpleOS_IPC_sendQueueComplete(Queue_t *Queue);

typedef IPC_t ParameterPipe_t;
extern ParameterPipe_t Pipe;
uint32_t SimpleOS_IPC_initParameterPipe(ParameterPipe_t *Pipe);
uint32_t SimpleOS_IPC_pendParameterPipe(Thread_t *Thread, uint32_t Time);
uint32_t SimpleOS_IPC_sendParameterPipe(Thread_t *Thread, void *Parameter);

typedef struct 
{
    Thread_t *Head;
    Thread_t *Tail;
} Thread_Chain_t;

typedef struct _IPC_Chain_t
{
    IPC_t *Head;
    IPC_t *Tail;
}IPC_Chain_t;

typedef struct
{
    IPC_Chain_t Semaphore;
    IPC_Chain_t Mailbox;
    IPC_Chain_t Mutex;
    IPC_Chain_t Eventgroup;
    IPC_Chain_t Queue;
    ParameterPipe_t *Pipe;
}IPC_List_t;
uint32_t SimpleOS_IPC_initIPCList(IPC_List_t *IPCList);
uint32_t SimpleOS_IPC_addIPCNode(IPC_Chain_t *IPCChain,IPC_t *IPC);
uint32_t SimpleOS_IPC_removeIPCNode(IPC_Chain_t *IPCChain,IPC_t *IPC);
uint32_t SimpleOS_IPC_checkChain(IPC_Chain_t *Chain);
uint32_t SimpleOS_IPC_checkParameterPipe(ParameterPipe_t *Pipe);
uint32_t SimpleOS_checkIPCList(IPC_List_t *IPC_List);

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

typedef struct
{
    uint32_t TimeStamp;
    uint32_t IdleTime;
}Kernel_t;
void SimpleOS_assertFailed(void);
Kernel_t *SimpleOS_getKernel(void);
uint32_t SimpleOS_Kernel_init(void);
void SimpleOS_TimeTickHandler(void);
uint32_t SimpleOS_IdleThread_init(void);


typedef struct
{
    uint32_t (*init)(void *Arg);
    uint32_t (*read)(uint8_t *Buffer, uint32_t Pos, uint32_t SizetoRead, uint32_t *SizeRead);
    uint32_t (*write)(uint8_t *Buffer, uint32_t Pos, uint32_t SizetoWrite, uint32_t *SizeWroten);
    uint32_t (*deinit)(void *Arg);
}Simple_Device_t;
#ifdef __cplusplus
}
#endif

#endif
