
    IMPORT Scheduler
	IMPORT SimpleOS_schedule
	IMPORT __SystemCall_Table
	
    EXPORT SimpleOS_disableInterrupt
    EXPORT SimpleOS_enableInterrupt
    EXPORT SimpleOS_enterCritical
    EXPORT SimpleOS_exitCritical
    EXPORT SimpleOS_TriggerContextSwitch
    EXPORT SimpleOS_Scheduler_getHighReadyThread
    EXPORT PendSV_Handler
    EXPORT SVC_Handler
    
    
NVIC_INT_CTRL   EQU     0xE000ED04          ;Interrupt control state register.
NVIC_SYSPRI14   EQU     0xE000ED22          ;System priority register (priority 14).
NVIC_PENDSV_PRI EQU         0xFFFF          ;PendSV priority value (lowest).
NVIC_PENDSVSET  EQU     0x10000000          ;Value to trigger PendSV exception.

    PRESERVE8
    THUMB


    AREA CODE, CODE, READONLY
SimpleOS_disableInterrupt
    CPSID   I
    BX      LR

SimpleOS_enableInterrupt
    CPSIE   I
    BX      LR

SimpleOS_enterCritical
    CPSID   I
    LDR     R0,=Scheduler
    LDR     R1,[R0,#48]                     ;R1 = Scheduler.CriticalLevel
    ADD     R1,R1,#1
    STR     R1,[R0,#48]
    CPSIE   I
    BX      LR

SimpleOS_exitCritical
    CPSID   I
    LDR     R0,=Scheduler
    LDR     R1,[R0,#48]                     ;R1 = Scheduler.CriticalLevel
    SUB     R1,R1,#1
    STR     R1,[R0,#48]
    CPSIE   I
	CMP		R1,#0
	IT		EQ
	BLEQ	SimpleOS_schedule

    BX      LR


SimpleOS_Scheduler_getHighReadyThread
    LDR     R0,=Scheduler
    LDR     R1,[R0,#12]
    RBIT    R1,R1
    CLZ     R3,R1                           ;R3 = HighRreadyGroup

    LSL		R3,R3,#2						;&ReadyItem[HighReadyGroup] = HighReadyGroup * 4 + Offset
	ADD     R2,R3,#16                       ;R2 = &ReadyItem[HighReadyGroup]
    LDR     R1,[R0,R2]                      ;R1 = ReadyItem[HighReadyGroup]
    RBIT    R1,R1
    CLZ     R2,R1                           ;R2 = HighReadyItem

    LSL		R1,R3,#5                        ;R1 = HighReadyGroup * 32
    ADD     R1,R1,R2                        ;R1 = HighReadyGroup * 32 + HighReadyItem
	STR		R1,[R0,#8]
	
	;MOV 	R5,#52
	LSL		R6,R1,#3						;R6 = HighReadyPriority * 8
	ADD		R5,R6,#60
	LDR		R2,[R0,R5]
	STR		R2,[R0]							;ReadyThread = ReadyList[HighReadyPriority]
    BX      LR


SimpleOS_TriggerContextSwitch
  LDR     R0, =NVIC_INT_CTRL                ; Trigger the PendSV exception (causes context switch)
  LDR     R1, =NVIC_PENDSVSET
  STR     R1, [R0]
  BX      LR

PendSV_Handler
    CPSID   I
	LDR     R0,=Scheduler                   ;R0 = &Scheduler
    MRS     R1,PSP
    CBZ     R1, PendSVHandler_nosave        ;Skip register save the first time
	TST     lr, #0x10
	IT		EQ
    VSTMFDEQ  R1!, {S16 - S31}       		;push FPU register s16~s31
	SUBS    R1, R1, #0x20                   ;Save remaining regs r4-11 on process stack
    STM     R1, {R4-R11}
	LDR		R2,[R0,#4]
	STR		R1,[R2]
	;STR		R1,[R2]
PendSVHandler_nosave    
	LDR     R1,[R0]                         ;R1 = Scheduler.ReadyThread
    STR     R1,[R0,#4]                         ;Scheduler.RunningThread = Scheduler.ReadyThread
    
    LDR     R0,[R1]                         ;R0  = Scheduler.ReadyThread->SP

    LDM     R0, {R4-R11}                    ;Restore r4-11 from new process stack
    ADDS    R0, R0, #0x20

    TST 	R14, #0x10
	IT 		EQ
	VLDMIAEQ R0!, {S16-S31} 
	
    MSR     PSP, R0                         ;Load PSP with new process SP
    ORR     LR, LR, #0x04                   ;Ensure exception return uses process stack
    CPSIE   I
    BX  LR
	
SVC_Handler
	CPSID	I
	PUSH	{R5,R6,R7,LR}
    TST     LR,#0x04
    ITE     EQ
    MRSEQ   R5,MSP
    MRSNE   R5,PSP
    LDR     R6,[R5,#24]
    LDR     R7,[R6,#-2]
    AND     R7,#0x000000FF                  ;get SVC No.
	LDR		R6,=__SystemCall_Table
	LSL		R7,R7,#2
	LDR		R7,[R6,R7]
    STR     R7,[R5,#24]
	POP		{R5,R6,R7,LR}
	
	CPSIE	I
    BX      LR

BusFault_Handler
	B 		BusFault_Handler
	BX		LR
	
    END