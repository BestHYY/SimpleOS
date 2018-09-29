#include "simpleos.h"

void SimpleOS_Thread_exit(Thread_t *Thread)
{
    if(Thread->ExtiHook != SIMPLE_NULL)
        Thread->ExtiHook();
	if(Thread->Next != SIMPLE_NULL)
		Thread->Next->Prev = Thread->Prev;
	if(Thread->Prev != SIMPLE_NULL)
		Thread->Prev->Next = Thread->Next;
}

uint32_t *SimpleOS_Thread_setStack(Thread_t *Thread, uint32_t *StackBase, uint32_t StackSize)
{
    uint32_t *SP;
    uint32_t i;
    Thread->StackBase = StackBase + StackSize;
	Thread->StackBase = (uint32_t *)((uint32_t)(Thread->StackBase) & 0xFFFFFFF8);
	SP = Thread->StackBase;
    for(i = 0 ; i < StackSize / 4 ; i ++)
    {
        StackBase[i] = 0xCCCCCCCC;
    }
    SP ++;
#if USE_FPU     	/* Registers stacked as if auto-saved on exception        */
	*(--SP) = (uint32_t)0x00000000u; //No Name Register
	*(--SP) = (uint32_t)0x00001000u; //FPSCR
	*(--SP) = (uint32_t)0x00000015u; //s15
	*(--SP) = (uint32_t)0x00000014u; //s14
	*(--SP) = (uint32_t)0x00000013u; //s13
	*(--SP) = (uint32_t)0x00000012u; //s12
	*(--SP) = (uint32_t)0x00000011u; //s11
	*(--SP) = (uint32_t)0x00000010u; //s10
	*(--SP) = (uint32_t)0x00000009u; //s9
	*(--SP) = (uint32_t)0x00000008u; //s8
	*(--SP) = (uint32_t)0x00000007u; //s7
	*(--SP) = (uint32_t)0x00000006u; //s6
	*(--SP) = (uint32_t)0x00000005u; //s5
	*(--SP) = (uint32_t)0x00000004u; //s4
	*(--SP) = (uint32_t)0x00000003u; //s3
	*(--SP) = (uint32_t)0x00000002u; //s2
	*(--SP) = (uint32_t)0x00000001u; //s1
	*(--SP) = (uint32_t)0x00000000u; //s0
#endif
    *(--SP) = (uint32_t)0x01000000u;                            /* xPSR                                                   */
    *(--SP) = (uint32_t)(Thread->Run);                          /* Entry Point                                            */
    *(--SP) = (uint32_t)SimpleOS_Thread_exit;                   /* R14 (LR)                                               */
    *(--SP) = (uint32_t)0x12121212u;                            /* R12                                                    */
    *(--SP) = (uint32_t)0x03030303u;                            /* R3                                                     */
    *(--SP) = (uint32_t)0x02020202u;                            /* R2                                                     */
    *(--SP) = (uint32_t)Thread;                                 /* R1                                                     */
    *(--SP) = (uint32_t)Thread->Paramater;                      /* R0 : argument                                          */
#if USE_FPU
	*(--SP) = (uint32_t)0x00000031u; //s31
	*(--SP) = (uint32_t)0x00000030u; //s30
	*(--SP) = (uint32_t)0x00000029u; //s29
	*(--SP) = (uint32_t)0x00000028u; //s28
	*(--SP) = (uint32_t)0x00000027u; //s27
	*(--SP) = (uint32_t)0x00000026u; //s26
	*(--SP) = (uint32_t)0x00000025u; //s25
	*(--SP) = (uint32_t)0x00000024u; //s24
	*(--SP) = (uint32_t)0x00000023u; //s23
	*(--SP) = (uint32_t)0x00000022u; //s22
	*(--SP) = (uint32_t)0x00000021u; //s21
	*(--SP) = (uint32_t)0x00000020u; //s20
	*(--SP) = (uint32_t)0x00000019u; //s19
	*(--SP) = (uint32_t)0x00000018u; //s18
	*(--SP) = (uint32_t)0x00000017u; //s17
	*(--SP) = (uint32_t)0x00000016u; //s16
#endif
	/* Remaining registers saved on process stack             */
    *(--SP) = (uint32_t)0x11111111u;                            /* R11                                                    */
    *(--SP) = (uint32_t)0x10101010u;                            /* R10                                                    */
    *(--SP) = (uint32_t)0x09090909u;                            /* R9                                                     */
    *(--SP) = (uint32_t)0x08080808u;                            /* R8                                                     */
    *(--SP) = (uint32_t)0x07070707u;                            /* R7                                                     */
    *(--SP) = (uint32_t)0x06060606u;                            /* R6                                                     */
    *(--SP) = (uint32_t)0x05050505u;                            /* R5                                                     */
    *(--SP) = (uint32_t)0x04040404u;                            /* R4                                                     */
    Thread->SP = SP;
    return (SP);
}

void SimpleOS_Thread_setEntry(Thread_t *Thread, Thread_Entry Run)
{
    Thread->Run = Run;
}

void SimpleOS_Thread_setExitHook(Thread_t *Thread, void (*ExtiHook)(void))
{
    Thread->ExtiHook = ExtiHook;
}

void SimpleOS_Thread_setParameter(Thread_t *Thread,void *parameter)
{
	Thread->Paramater = parameter;
}

void SimpleOS_Thread_setHook(Thread_t *Thread, void (*Hook)(void))
{
    Thread->Hook = Hook;
}

void SimpleOS_Thread_setName(Thread_t *Thread, const char *Name)
{
    uint32_t i = 0;
    while(Name[i])
    {
        Thread->Name[i] = Name[i];
        i++;
    }
    while(i++ < SIMPLE_NAME_MAX)
        Thread->Name[i] = 0;
}

uint32_t SimpleOS_Thread_setPriority(Thread_t *Thread, uint32_t Priority)
{
    Thread->SetPriority     = Priority;
    Thread->CurrentPriority = Priority;
    return SIMPLE_ERR_OK;
}

uint32_t SimpleOS_Thread_setTimePiece(Thread_t *Thread, uint32_t TimePiece)
{
    Thread->SetTimePiece    = TimePiece;
    Thread->RemainTimePiece = TimePiece;
    return SIMPLE_ERR_OK;
}

uint32_t SimpleOS_Thread_changePriority(Thread_t *Thread, uint32_t Priority)
{
    if(Thread->Status == SIMPLE_STATUS_READY)
    {
        SimpleOS_setThreadSuspend(Thread);
        Thread->CurrentPriority = Priority;
        SimpleOS_setThreadReady(Thread);
    }
    else
    {
        Thread->CurrentPriority = Priority;
    }
    return SIMPLE_ERR_OK;
}

uint32_t SimpleOS_Thread_recoverPriority(Thread_t *Thread)
{

    SimpleOS_enterCritical();
    if(Thread->Status == SIMPLE_STATUS_READY)
    {
    
        SimpleOS_setThreadSuspend(Thread);
        Thread->CurrentPriority = Thread->SetPriority;
        SimpleOS_setThreadReady(Thread);
    }
    else
    {
        Thread->CurrentPriority = Thread->SetPriority;
    }
    SimpleOS_exitCritical();
    return SIMPLE_ERR_OK;
}

uint32_t SimpleOS_Thread_init(Thread_t *Thread,Thread_Entry Entry,void *Parameter, uint32_t *StackBase, uint32_t StackSize, uint32_t Priority, uint32_t TimePiece, const char *Name)
{
    SimpleOS_Thread_setName(Thread,Name);
    SimpleOS_Thread_setEntry(Thread,Entry);
    SimpleOS_Thread_setParameter(Thread,Parameter);
    SimpleOS_Thread_setPriority(Thread,Priority);
    SimpleOS_Thread_setTimePiece(Thread,TimePiece);
    SimpleOS_Thread_setStack(Thread,StackBase,StackSize);
    return SIMPLE_ERR_OK;
}

void SimpleOS_Thread_sleep(uint32_t Tick)
{

    Thread_t *Thread;

    Thread = SimpleOS_getCurrentThread();
    SimpleOS_enterCritical();
    SimpleOS_setThreadSuspend(Thread);
    Thread->SleepTime = Tick;
    if(Scheduler.SleepingList.Head == SIMPLE_NULL)
    {
        Thread->Prev = SIMPLE_NULL;
        Thread->Next = SIMPLE_NULL;
        Scheduler.SleepingList.Head = Thread;
        Scheduler.SleepingList.Tail = Thread;
    }
    else
    {
        Thread->Prev = Scheduler.SleepingList.Tail;
        Thread->Next = SIMPLE_NULL;
        Scheduler.SleepingList.Tail->Next = Thread;
        Scheduler.SleepingList.Tail = Thread;
    }
    SimpleOS_exitCritical();
    
}

