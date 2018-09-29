#include "simple_cppdef.h"

void Thread::exec(Thread *Thread)
{
    Thread->run();
    Thread->exit();
}

void Thread::run(void)
{
}

void Thread::exit(void)
{
}

Thread::Thread(void)
{
}

Thread::Thread(const char *Name, uint32_t Priority, uint32_t TickNum, uint32_t StackSizeinBytes)
{
    uint32_t *Stack;
    this->ThreadPointer = (Thread_t*)malloc(sizeof(Thread_t));
    Stack = (uint32_t*)malloc(StackSizeinBytes);
    SimpleOS_Thread_init(this->ThreadPointer,(Thread_Entry)this->exec,this,Stack,StackSizeinBytes,Priority,TickNum,Name);
}

Thread::Thread(const char *Name, uint32_t Priority, uint32_t TickNum, uint32_t* Stack, uint32_t StackSizeinBytes)
{
    SimpleOS_Thread_init(this->ThreadPointer,(Thread_Entry)this->exec,this,Stack,StackSizeinBytes,Priority,TickNum,Name);
}

uint32_t Thread::pendParameter(uint32_t Time)
{
    return SimpleOS_IPC_pendParameterPipe(&this->ThreadEntity,Time);
}

void Thread::start(void)
{
    SimpleOS_Scheduler_startThread(SimpleOS_getScheduler(),&this->ThreadEntity);
}

void Thread::sleep(uint32_t Ticks)
{
    SimpleOS_Thread_sleep(Ticks);
}
