#ifndef _SIMPLE_API_H_
#define _SIMPLE_API_H_
#ifdef __cplusplus
extern "C"{
#endif
#include "simpleos.h"
uint32_t setThreadName(Thread_t *Thread, const char *Name);                                             //0
uint32_t setThreadEntry(Thread_t *Thread, Thread_Entry Run);                                            //1
uint32_t setThreadParamater(Thread_t *Thread,void *parameter);                                          //2
uint32_t setThreadPriority(Thread_t *Thread, uint32_t Priority);                                        //3
uint32_t setThreadTimePiece(Thread_t *Thread, uint32_t TimePiece);                                      //4
uint32_t setThreadStack(Thread_t *Thread, uint32_t *StackBase, uint32_t StackSize);                     //5
uint32_t sleep(uint32_t Tick);                                                                          //6
//Scheduler API
uint32_t schdule(void);                                                                                 //7
uint32_t pauseScheduler(void);                                                        //8
uint32_t startScheduler(void);                                                        //9
uint32_t suspendThread(Thread_t *Thread);                                       //10
uint32_t resumeThread(Thread_t *Thread);                                        //11
uint32_t startThread(Thread_t *Thread);                                         //12
uint32_t deleteThread(Thread_t *Thread);                                        //13
//IPC API
uint32_t initSemaphore(Semaphore_t *Semaphore, const char *Name, uint32_t InitValue);                   //14
uint32_t pendSemaphore(Semaphore_t *Semaphore, uint32_t Time);                                          //15
uint32_t sendSemaphore(Semaphore_t *Semaphore);                                                         //16
uint32_t initMail(Mailbox_t *Mailbox, const char *Name);                                                //17
uint32_t pendMail(Mailbox_t *Mailbox, void *Address, uint32_t Time);                                    //18
uint32_t sendMail(Mailbox_t *Mailbox, uint32_t Mail);                                                   //19
uint32_t initMutex(Mutex_t *Mutex, const char *Name);                                                   //20
uint32_t pendMutex(Mutex_t *Mutex, uint32_t Time);                                                      //21
uint32_t sendMutex(Mutex_t *Mutex);                                                                     //22
uint32_t initEvent(Eventgroup_t *Eventgroup, const char *Name);                                         //23
uint32_t pendEvent(Eventgroup_t *Eventgroup, uint32_t Event, uint32_t Flag, uint32_t Time);                 //24
uint32_t sendEvent(Eventgroup_t *Eventgroup, uint32_t Event);                                           //25
uint32_t initQueue(Queue_t *Queue, const char *Name, void *PoolAddr, uint32_t PoolSizeinBytes);         //26
uint32_t pendQueue(Queue_t *Queue, void *Dst, uint32_t Lenth, uint32_t Time);                           //27
uint32_t sendQueue(Queue_t *Queue, void *Src, uint32_t Lenth);                                          //28
uint32_t pendParameter(Thread_t *Thread, uint32_t Time);                                                //29
uint32_t sendParameter(Thread_t *Thread, void *Parameter);                                              //30
//Heap API
uint32_t addHeap(uint32_t StartAddr, uint32_t HeapSizeinBytes);                                         //31
uint32_t malloc(uint32_t Size);                                                                         //32
uint32_t free(void *p);                                                                                 //33
uint32_t getHeapInfo(uint32_t *HeapSize, uint32_t *Used, uint32_t *Unused);                             //34
#ifdef __cplusplus
}
#endif

#endif
