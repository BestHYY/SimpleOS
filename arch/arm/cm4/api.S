	PRESERVE8
    THUMB
	IMPORT  Scheduler
	EXPORT	setThreadName								;0
	EXPORT	setThreadEntry								;1
	EXPORT	setThreadParamater							;2
	EXPORT	setThreadPriority							;3
	EXPORT	setThreadTimePiece							;4
	EXPORT	setThreadStack								;5
	EXPORT	sleep										;6
	;Scheduler API
	EXPORT	schdule										;7
	EXPORT	pauseScheduler								;8
	EXPORT	startScheduler								;9
	EXPORT	suspendThread								;10
	EXPORT	resumeThread								;11
	EXPORT	startThread									;12
	EXPORT	deleteThread								;13
	;IPC	API
	EXPORT	initSemaphore								;14
	EXPORT	pendSemaphore								;15
	EXPORT	sendSemaphore								;16
	EXPORT	initMail									;17
	EXPORT	pendMail									;18
	EXPORT	sendMail									;19
	EXPORT	initMutex									;20
	EXPORT	pendMutex									;21
	EXPORT	sendMutex									;22
	EXPORT	initEvent									;23
	EXPORT	pendEvent									;24
	EXPORT	sendEvent									;25
	EXPORT	initQueue									;26
	EXPORT	pendQueue									;27
	EXPORT	sendQueue									;28
	;Heap API
	;EXPORT	addHeap										;31
	;EXPORT	malloc										;32
	;EXPORT	free										;33
	;EXPORT	getHeapInfo									;34
		
	AREA    DATA,DATA, READONLY
		
	;Thread API
	IMPORT	SimpleOS_Thread_setName						;0
	IMPORT	SimpleOS_Thread_setEntry					;1
	IMPORT	SimpleOS_Thread_setParameter				;2
	IMPORT	SimpleOS_Thread_setPriority					;3
	IMPORT	SimpleOS_Thread_setTimePiece				;4
	IMPORT	SimpleOS_Thread_setStack					;5
	IMPORT	SimpleOS_Thread_sleep						;6
	;Scheduler API
	IMPORT	SimpleOS_schedule							;7
	IMPORT	SimpleOS_pauseScheduler						;8
	IMPORT	SimpleOS_startScheduler						;9
	IMPORT	SimpleOS_suspendThread						;10
	IMPORT	SimpleOS_resumeThread						;11
	IMPORT	SimpleOS_startThread						;12
	IMPORT	SimpleOS_deleteThread						;13
	;IPC	API
	IMPORT	SimpleOS_IPC_initIPCSemaphore				;14
	IMPORT	SimpleOS_IPC_pendSemaphore					;15
	IMPORT	SimpleOS_IPC_sendSemaphore					;16
	IMPORT	SimpleOS_IPC_initMailbox					;17
	IMPORT	SimpleOS_IPC_pendMail						;18
	IMPORT	SimpleOS_IPC_sendMail						;19
	IMPORT	SimpleOS_IPC_initMutex						;20
	IMPORT	SimpleOS_IPC_pendMutex						;21
	IMPORT	SimpleOS_IPC_sendMutex						;22
	IMPORT	SimpleOS_IPC_initEventgroup					;23
	IMPORT	SimpleOS_IPC_pendEvent      				;24
	IMPORT	SimpleOS_IPC_sendEvent						;25
	IMPORT	SimpleOS_IPC_initQueue						;26
	IMPORT	SimpleOS_IPC_pendQueue						;27
	IMPORT	SimpleOS_IPC_sendQueue						;28
	;Heap API
	;IMPORT	SimpleOS_Heap_add							;31
	;IMPORT	SimpleOS_malloc								;32
	;IMPORT	SimpleOS_free								;33
	;IMPORT	SimpleOS_getHeapInfo						;34
		
	EXPORT	__SystemCall_Table
__SystemCall_Table
	;Thread API
	DCD	SimpleOS_Thread_setName						;0
	DCD	SimpleOS_Thread_setEntry					;1
	DCD	SimpleOS_Thread_setParameter				;2
	DCD	SimpleOS_Thread_setPriority					;3
	DCD	SimpleOS_Thread_setTimePiece				;4
	DCD	SimpleOS_Thread_setStack					;5
	DCD	SimpleOS_Thread_sleep						;6
	;Scheduler API
	DCD	SimpleOS_schedule							;7
	DCD	SimpleOS_pauseScheduler						;8
	DCD	SimpleOS_startScheduler						;9
	DCD	SimpleOS_suspendThread						;10
	DCD	SimpleOS_resumeThread						;11
	DCD	SimpleOS_startThread						;12
	DCD	SimpleOS_deleteThread						;13
	;IPC	API
	DCD	SimpleOS_IPC_initIPCSemaphore				;14
	DCD	SimpleOS_IPC_pendSemaphore					;15
	DCD	SimpleOS_IPC_sendSemaphore					;16
	DCD	SimpleOS_IPC_initMailbox					;17
	DCD	SimpleOS_IPC_pendMail						;18
	DCD	SimpleOS_IPC_sendMail						;19
	DCD	SimpleOS_IPC_initMutex						;20
	DCD	SimpleOS_IPC_pendMutex						;21
	DCD	SimpleOS_IPC_sendMutex						;22
	DCD	SimpleOS_IPC_initEventgroup					;23
	DCD	SimpleOS_IPC_pendEvent      				;24
	DCD	SimpleOS_IPC_sendEvent						;25
	DCD	SimpleOS_IPC_initQueue						;26
	DCD	SimpleOS_IPC_pendQueue						;27
	DCD	SimpleOS_IPC_sendQueue						;28
	;Heap API
	;DCD	SimpleOS_Heap_add							;31
	;DCD	SimpleOS_malloc								;32
	;DCD	SimpleOS_free								;33
	;DCD	SimpleOS_getHeapInfo						;34
		

	AREA CODE, CODE, READONLY

setThreadName
	SVC	0
	BX	LR
setThreadEntry
	SVC	1
	BX	LR
setThreadParamater
	SVC	2
	BX	LR	
setThreadPriority
	SVC	3
	BX	LR
setThreadTimePiece
	SVC	4
	BX	LR
setThreadStack
	SVC	5
	BX	LR
sleep
	SVC	6
	BX	LR
	
schdule
	SVC	7
	BX	LR
pauseScheduler
	SVC	8
	BX	LR
startScheduler
	SVC	9
	BX	LR
suspendThread
	SVC	10
	BX	LR
resumeThread
	SVC	11
	BX	LR
startThread
	SVC	12
	BX	LR
deleteThread
	SVC	13
	BX	LR
	
initSemaphore
	SVC	14
	BX	LR
pendSemaphore
	SVC	15
	BX	LR
sendSemaphore
	SVC	16
	BX	LR
	
initMail
	SVC	17
	BX	LR
pendMail
	SVC	18
	BX	LR
sendMail
	SVC	19
	BX	LR
	
initMutex
	SVC	20
	BX	LR
pendMutex
	SVC	21
	BX	LR
sendMutex
	SVC	22
	BX	LR

initEvent
	SVC	23
	BX	LR
pendEvent
	SVC	24
	BX	LR
sendEvent
	SVC	25
	BX	LR

initQueue
	SVC	26
	BX	LR
pendQueue
	SVC	27
	BX	LR
sendQueue
	SVC	28
	BX	LR
pendParameter
	SVC	29
	BX	LR
sendParameter
	SVC	30
	BX	LR

SVC_Schedule
	SUB		R5,#32
	MSR		PSP,R5
	BL		SimpleOS_schedule
	BX		LR
	
	END