#include "simpleos.h"
Thread_t IdleThread;
uint32_t IdleThreadStack[128];

void IdleThread_Task(void *Parameter)
{
    Kernel_t *Kernel = SimpleOS_getKernel();
    while(SIMPLE_TRUE)
    {
        Kernel->IdleTime += 1;
        if(Kernel->IdleTime == 0xFFFFFFFF)
            Kernel->IdleTime = 0;
    }
}

uint32_t SimpleOS_IdleThread_init(void)
{
    SimpleOS_Thread_init(&IdleThread,IdleThread_Task,SIMPLE_NULL,&IdleThreadStack[0],sizeof(IdleThreadStack),SIMPLE_PRIORITY_MAX - 1,1,"Idle");
    SimpleOS_startThread(&IdleThread);
    return SIMPLE_ERR_OK;
}
