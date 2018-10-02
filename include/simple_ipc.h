#ifndef _IPC_H_
#define _IPC_H_
#ifdef __cplusplus
extern"C"{
#endif

#include "simple_def.h"
#include "simple_thread.h"
#include "simple_scheduler.h"

/*
 * Inter-Process-Communication Base Class
 */
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

/*
 * Semaphore
 */
typedef struct _Semaphore_t
{
    IPC_t Parent;
    uint32_t SemaphoreCount;
}Semaphore_t;
uint32_t SimpleOS_IPC_initIPCSemaphore(Semaphore_t *Semaphore, const char *Name, uint32_t InitValue);
uint32_t SimpleOS_IPC_pendemaphore(Semaphore_t *Semaphore, uint32_t Time);
uint32_t SimpleOS_IPC_sendSemaphore(Semaphore_t *Semaphore);

/*
 * Mailbox
 */
typedef struct _Mailbox_t
{
    IPC_t Parent;
    uint32_t Mail;
    uint32_t Status;
}Mailbox_t;
uint32_t SimpleOS_IPC_initMailbox(Mailbox_t *Mailbox, const char *Name);
uint32_t SimpleOS_IPC_pendMail(Mailbox_t *Mailbox, void *Address, uint32_t Time);
uint32_t SimpleOS_IPC_sendMail(Mailbox_t *Mailbox, uint32_t Mail);

/*
 * Mutex
 */
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

/*
 * EventGroup
 */
typedef struct _Eventgroup_t
{
    IPC_t Parent;
    uint32_t EventSet;
    uint32_t Status;
}Eventgroup_t;
uint32_t SimpleOS_IPC_initEventgroup(Eventgroup_t *Eventgroup, const char *Name);
uint32_t SimpleOS_IPC_pendEvent(Eventgroup_t *Eventgroup, uint32_t Event,uint32_t Flag, uint32_t Time);
uint32_t SimpleOS_IPC_sendEvent(Eventgroup_t *Eventgroup, uint32_t Event);

/*
 * Message Queue
 */
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

/*
 * IPC operation structure & methods
 */
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
}IPC_List_t;
uint32_t SimpleOS_IPC_initIPCList(IPC_List_t *IPCList);
uint32_t SimpleOS_IPC_addIPCNode(IPC_Chain_t *IPCChain,IPC_t *IPC);
uint32_t SimpleOS_IPC_removeIPCNode(IPC_Chain_t *IPCChain,IPC_t *IPC);
uint32_t SimpleOS_IPC_checkChain(IPC_Chain_t *Chain);
uint32_t SimpleOS_checkIPCList(IPC_List_t *IPC_List);

#ifdef __cplusplus
}
#endif
#endif
