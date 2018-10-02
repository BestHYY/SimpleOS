#ifndef _SIMPLE_C_DEF_H_
#define _SIMPLE_C_DEF_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "simple_cfg.h"

#if SIMPLE_USE_STDINT
    #include "stdint.h"
#else
    typedef unsigned int uint32_t
    typedef signed int int32_t
    typedef unsigned short uint16_t
    typedef signed short int16_t
    typedef unsigned char uint8_t
    typedef signed char int8_t
#endif

#if defined(__CC_ARM)                         /* ARM Compiler */
    #define SIMPLE_INLINE           inline
#elif defined(__IAR_SYSTEMS_ICC__)            /* IAR Complier */
    #define SIMPLE_INLINE           inline
#elif defined(__GNUC__)                       /* GNU Complier */
    #define SIMPLE_INLINE           inline
#else
    #error "Can't Recognized Complier"
#endif

/*
 * SimpleOSOS Error Code Defination
 */
#define SIMPLE_ERR_OK            0x00
#define SIMPLE_ERR_ERR           0x01
#define SIMPLE_ERR_EMPTY         0x02
#define SIMPLE_ERR_FULL          0x04
#define SIMPLE_ERR_NOTEXIST      0x08
#define SIMPLE_ERR_TIMEOUT       0x10
#define SIMPLE_ERR_ILIGALARG     0x20

/*
 * Status Defination
 */
#define SIMPLE_STATUS_INIT        0x00
#define SIMPLE_STATUS_READY       0x01
#define SIMPLE_STATUS_RUN         0x02
#define SIMPLE_STATUS_SUSPEND     0x04
#define SIMPLE_STATUS_EXIT        0x08

//#define SIMPLE_STATUS_EMPTY       0x00
//#define SIMPLE_STATUS_FULL        0xFFFFFFFF
/*
 * IPC Status Defination
 */
#define SIMPLE_IPC_EMPTY          0x00
#define SIMPLE_IPC_READY          0x01
#define SIMPLE_IPC_FULL           0x02

/*
 * Eventgroup Option Defination
 */
#define SIMPLE_EVENT_AND          0x01
#define SIMPLE_EVENT_OR           0x02
#define SIMPLE_EVENT_CLEAR        0x04

#define SIMPLE_WAIT_LONGEST       0xFFFFFFFF

#define SIMPLE_NULL               (0)
#define SIMPLE_UNUSED(p)          ((void)(p))

#define SIMPLE_ALIGN(x,y)         ((x+y-1) & y)

typedef enum
{
    SIMPLE_FALSE = 0,
    SIMPLE_TRUE
}Simple_Bool_t;

#ifdef __cplusplus
}
#endif

#endif
