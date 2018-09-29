#include "simple_cppdef.h"

Semaphore::Semaphore(const char *Name, uint32_t SemaphoreCount)
{
    SimpleOS_IPC_initIPCSemaphore(&this->SemaphoreEntity,(char*)Name,SemaphoreCount);
}

uint32_t Semaphore::pend(uint32_t Time)
{
    return SimpleOS_IPC_pendSemaphore(&this->SemaphoreEntity,Time);
}

uint32_t Semaphore::send()
{
    return SimpleOS_IPC_sendSemaphore(&this->SemaphoreEntity);
}

MailBox::MailBox(const char *Name)
{
    SimpleOS_IPC_initMailBox(&this->MailBoxEntity,(char*)Name);
}
uint32_t MailBox::pend(void *Dst,uint32_t Time)
{
    return SimpleOS_IPC_pendMail(&this->MailBoxEntity,Dst,Time);
}

uint32_t MailBox::send(uint32_t Mail)
{
    return SimpleOS_IPC_sendMail(&this->MailBoxEntity,Mail);
}

Mutex::Mutex(const char *Name)
{
    SimpleOS_IPC_initMutex(&this->MutexEntity,(char*)Name);
}

uint32_t Mutex::pend(uint32_t Time)
{
    return SimpleOS_IPC_pendMutex(&this->MutexEntity,Time);
}

uint32_t Mutex::send()
{
    return SimpleOS_IPC_sendMutex(&this->MutexEntity);
}

EventGroup::EventGroup(const char *Name)
{
    SimpleOS_IPC_initEventGroup(&this->EventGroupEntity,(char*)Name);
}

uint32_t EventGroup::pend(uint32_t Event,uint32_t *Dst,uint32_t Flag, uint32_t Time)
{
    return SimpleOS_IPC_pendEvent(&this->EventGroupEntity,Event,Dst,Flag,Time);
}

uint32_t EventGroup::send(uint32_t Event)
{
    return SimpleOS_IPC_sendEvent(&this->EventGroupEntity,Event);
}

Queue::Queue(void *Pool, uint32_t PoolSize, const char *Name)
{
    SimpleOS_IPC_initQueue(&this->QueueEntity,(char*)Name,Pool,PoolSize);
}

Queue::Queue(uint32_t PoolSize, const char *Name)
{
    this->Pool = SimpleOS_malloc(PoolSize);
    SimpleOS_IPC_initQueue(&this->QueueEntity,(char*)Name,this->Pool,PoolSize);
}
uint32_t Queue::pend(void *Dst, uint32_t Lenth, uint32_t Time)
{
    return SimpleOS_IPC_pendQueue(&this->QueueEntity,Dst,Lenth,Time);
}

uint32_t Queue::send(void *Src, uint32_t Lenth)
{
    return SimpleOS_IPC_sendQueue(&this->QueueEntity,Src,Lenth);
}
