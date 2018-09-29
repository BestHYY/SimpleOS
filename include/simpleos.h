#ifndef _SimpleOS_C_H_
#define _SimpleOS_C_H_

#ifdef __cplusplus
extern "C" {
#endif
    
#include "simple_cfg.h"
#include "simple_cdef.h"
#include "stdint.h"
void SimpleOS_disableInterrupt(void);
void SimpleOS_enableInterrupt(void);
void SimpleOS_enterCritical(void);
void SimpleOS_exitCritical(void);
void SimpleOS_TriggerContextSwitch(void);
void SimpleOS_Scheduler_getHighReadyThread(void);
void PendSV_Handler(void);

#ifdef __cplusplus
}
#endif

#endif
