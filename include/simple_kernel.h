#ifndef _SIMPLE_KERNEL_H_
#define _SIMPLE_KERNEL_H_
#ifdef __cplusplus
extern"C"{
#endif

#include "simple_def.h"
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

#ifdef __cplusplus
}
#endif
#endif
