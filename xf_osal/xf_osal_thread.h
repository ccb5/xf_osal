/**
 * @file xf_osal_thread.h
 * @author cangyu(sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2023-12-06
 *
 * @copyright Copyright(c) 2023, CorAL. All rights reserved.
 *
 */

#if XF_OSAL_THREAD_IS_ENABLE

#ifndef __XF_OSAL_THREAD_H__
#define __XF_OSAL_THREAD_H__

/* ==================== [Includes] ========================================== */

#include "xf_osal_kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#ifndef __NO_RETURN
#   if defined(__CC_ARM)
#       define __NO_RETURN __declspec(noreturn)
#   elif defined(__ARMCC_VERSION) &&(__ARMCC_VERSION >= 6010050)
#       define __NO_RETURN __attribute__((__noreturn__))
#   elif defined(__GNUC__)
#       define __NO_RETURN __attribute__((__noreturn__))
#   elif defined(__ICCARM__)
#       define __NO_RETURN __noreturn
#   else
#       define __NO_RETURN
#   endif
#endif

#define XF_OSAL_DETACHED       0x00000000U ///< Thread created in detached mode (default)
#define XF_OSAL_JOINABLE       0x00000001U ///< Thread created in joinable mode
#define XF_OSAL_UNPRIVILEGED   0x00000002U ///< Thread runs in unprivileged mode
#define XF_OSAL_PRIVIEGED      0x00000004U ///< Thread runs in privileged mode

#define MAX_BITS_TASK_NOTIFY        31U

#define THREAD_FLAGS_INVALID_BITS   (~((1UL << MAX_BITS_TASK_NOTIFY)  - 1U))

/* ==================== [Typedefs] ========================================== */

typedef enum _xf_osal_priority_t {
    XF_OSAL_PRIORITY_NONE          =  0,         ///< No priority(not initialized).
    XF_OSAL_PRIORITY_IDLE          =  1,         ///< Reserved for Idle thread.
    XF_OSAL_PRIORITY_LOW           =  8,         ///< Priority: low
    XF_OSAL_PRIORITY_LOW1          =  8 + 1,     ///< Priority: low + 1
    XF_OSAL_PRIORITY_LOW2          =  8 + 2,     ///< Priority: low + 2
    XF_OSAL_PRIORITY_LOW3          =  8 + 3,     ///< Priority: low + 3
    XF_OSAL_PRIORITY_LOW4          =  8 + 4,     ///< Priority: low + 4
    XF_OSAL_PRIORITY_LOW5          =  8 + 5,     ///< Priority: low + 5
    XF_OSAL_PRIORITY_LOW6          =  8 + 6,     ///< Priority: low + 6
    XF_OSAL_PRIORITY_LOW7          =  8 + 7,     ///< Priority: low + 7
    XF_OSAL_PRIORITY_BELOW_NORMAL   = 16,         ///< Priority: below normal
    XF_OSAL_PRIORITY_BELOW_NORMAL1  = 16 + 1,     ///< Priority: below normal + 1
    XF_OSAL_PRIORITY_BELOW_NORMAL2  = 16 + 2,     ///< Priority: below normal + 2
    XF_OSAL_PRIORITY_BELOW_NORMAL3  = 16 + 3,     ///< Priority: below normal + 3
    XF_OSAL_PRIORITY_BELOW_NORMAL4  = 16 + 4,     ///< Priority: below normal + 4
    XF_OSAL_PRIORITY_BELOW_NORMAL5  = 16 + 5,     ///< Priority: below normal + 5
    XF_OSAL_PRIORITY_BELOW_NORMAL6  = 16 + 6,     ///< Priority: below normal + 6
    XF_OSAL_PRIORITY_BELOW_NORMAL7  = 16 + 7,     ///< Priority: below normal + 7
    XF_OSAL_PRIORITY_NORMOL        = 24,         ///< Priority: normal
    XF_OSAL_PRIORITY_NORMOL1       = 24 + 1,     ///< Priority: normal + 1
    XF_OSAL_PRIORITY_NORMOL2       = 24 + 2,     ///< Priority: normal + 2
    XF_OSAL_PRIORITY_NORMOL3       = 24 + 3,     ///< Priority: normal + 3
    XF_OSAL_PRIORITY_NORMOL4       = 24 + 4,     ///< Priority: normal + 4
    XF_OSAL_PRIORITY_NORMOL5       = 24 + 5,     ///< Priority: normal + 5
    XF_OSAL_PRIORITY_NORMOL6       = 24 + 6,     ///< Priority: normal + 6
    XF_OSAL_PRIORITY_NORMOL7       = 24 + 7,     ///< Priority: normal + 7
    XF_OSAL_PRIORITY_ABOVE_NORMAL   = 32,         ///< Priority: above normal
    XF_OSAL_PRIORITY_ABOVE_NORMAL1  = 32 + 1,     ///< Priority: above normal + 1
    XF_OSAL_PRIORITY_ABOVE_NORMAL2  = 32 + 2,     ///< Priority: above normal + 2
    XF_OSAL_PRIORITY_ABOVE_NORMAL3  = 32 + 3,     ///< Priority: above normal + 3
    XF_OSAL_PRIORITY_ABOVE_NORMAL4  = 32 + 4,     ///< Priority: above normal + 4
    XF_OSAL_PRIORITY_ABOVE_NORMAL5  = 32 + 5,     ///< Priority: above normal + 5
    XF_OSAL_PRIORITY_ABOVE_NORMAL6  = 32 + 6,     ///< Priority: above normal + 6
    XF_OSAL_PRIORITY_ABOVE_NORMAL7  = 32 + 7,     ///< Priority: above normal + 7
    XF_OSAL_PRIORITY_HIGH          = 40,         ///< Priority: high
    XF_OSAL_PRIORITY_HIGH1         = 40 + 1,     ///< Priority: high + 1
    XF_OSAL_PRIORITY_HIGH2         = 40 + 2,     ///< Priority: high + 2
    XF_OSAL_PRIORITY_HIGH3         = 40 + 3,     ///< Priority: high + 3
    XF_OSAL_PRIORITY_HIGH4         = 40 + 4,     ///< Priority: high + 4
    XF_OSAL_PRIORITY_HIGH5         = 40 + 5,     ///< Priority: high + 5
    XF_OSAL_PRIORITY_HIGH6         = 40 + 6,     ///< Priority: high + 6
    XF_OSAL_PRIORITY_HIGH7         = 40 + 7,     ///< Priority: high + 7
    XF_OSAL_PRIORITY_REALTIME      = 48,         ///< Priority: realtime
    XF_OSAL_PRIORITY_REALTIME1     = 48 + 1,     ///< Priority: realtime + 1
    XF_OSAL_PRIORITY_REALTIME2     = 48 + 2,     ///< Priority: realtime + 2
    XF_OSAL_PRIORITY_REALTIME3     = 48 + 3,     ///< Priority: realtime + 3
    XF_OSAL_PRIORITY_REALTIME4     = 48 + 4,     ///< Priority: realtime + 4
    XF_OSAL_PRIORITY_REALTIME5     = 48 + 5,     ///< Priority: realtime + 5
    XF_OSAL_PRIORITY_REALTIME6     = 48 + 6,     ///< Priority: realtime + 6
    XF_OSAL_PRIORITY_REALTIME7     = 48 + 7,     ///< Priority: realtime + 7
    XF_OSAL_PRIORITY_ISR           = 56,         ///< Reserved for ISR deferred thread.
    XF_OSAL_PRIORITY_ERROR         = -1,         ///< System cannot determine priority or illegal priority.
    XF_OSAL_PRIORITY_RESERVED      = 0x7FFFFFFF  ///< Prevents enum down-size compiler optimization.
} xf_osal_priority_t;

typedef struct _xf_osal_thread_attr_t {
    const char                   *name;   ///< name of the thread
    uint32_t                 attr_bits;   ///< attribute bits
    void                      *cb_mem;    ///< memory for control block
    uint32_t                   cb_size;   ///< size of provided memory for control block
    void                   *stack_mem;    ///< memory for stack
    uint32_t                stack_size;   ///< size of stack
    xf_osal_priority_t              priority;   ///< initial thread priority(default: XF_OSAL_PRIORITY_NORMOL)
    uint32_t                  reserved;   ///< reserved(must be 0)
} xf_osal_thread_attr_t;

typedef void *xf_osal_thread_t;
typedef void(*xf_osal_thread_func_t)(void *argument);

/* ==================== [Global Prototypes] ================================= */

xf_osal_thread_t xf_osal_thread_create(xf_osal_thread_func_t func, void *argument, const xf_osal_thread_attr_t *attr);
const char *xf_osal_thread_get_name(xf_osal_thread_t thread);
xf_osal_thread_t xf_osal_thread_get_current(void);
xf_osal_state_t xf_osal_thread_get_state(xf_osal_thread_t thread);
uint32_t xf_osal_thread_get_stack_space(xf_osal_thread_t thread);
xf_err_t xf_osal_thread_set_priority(xf_osal_thread_t thread, xf_osal_priority_t priority);
xf_osal_priority_t xf_osal_thread_get_priority(xf_osal_thread_t thread);
xf_err_t xf_osal_thread_yield(void);
xf_err_t xf_osal_thread_suspend(xf_osal_thread_t thread);
xf_err_t xf_osal_thread_resume(xf_osal_thread_t thread);
xf_err_t xf_osal_thread_delete(xf_osal_thread_t thread);
uint32_t xf_osal_thread_get_count(void);
uint32_t xf_osal_thread_enumerate(
    xf_osal_thread_t *thread_array, uint32_t array_items);

xf_err_t xf_osal_thread_notify_set(xf_osal_thread_t thread, uint32_t notify);
xf_err_t xf_osal_thread_notify_clear(uint32_t notify);
uint32_t xf_osal_thread_notify_get(void);
xf_err_t xf_osal_thread_notify_wait(uint32_t notify, uint32_t options, uint32_t timeout);

xf_err_t xf_osal_delay(uint32_t ticks);
xf_err_t xf_osal_delay_until(uint32_t ticks);

xf_err_t xf_osal_delay_ms(uint32_t ms);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_OSAL_THREAD_H__

#endif // XF_OSAL_THREAD_ENABLE
