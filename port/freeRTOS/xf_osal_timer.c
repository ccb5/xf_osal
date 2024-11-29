/**
 * @file xf_osal_timer.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-09-10
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_osal_internal.h"

#if XF_OSAL_TIMER_IS_ENABLE

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

typedef struct {
    xf_osal_timer_func_t func;
    void         *arg;
} timer_callback_t;

/* ==================== [Static Prototypes] ================================= */

static void timer_callback(TimerHandle_t hTimer);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

xf_osal_timer_t xf_osal_timer_create(xf_osal_timer_func_t func, xf_osal_timer_type_t type, void *argument,
                                     xf_osal_timer_attr_t *attr)
{
    const char *name;
    TimerHandle_t hTimer;
    timer_callback_t *callb;
    UBaseType_t reload;
    int32_t mem;
    uint32_t callb_dyn;

    hTimer = NULL;

    if ((IRQ_Context() == 0U) && (func != NULL)) {
        callb     = NULL;
        callb_dyn = 0U;

#if (configSUPPORT_STATIC_ALLOCATION == 1)
        /* Static memory allocation is available: check if memory for control block */
        /* is provided and if it also contains space for callback and its argument  */
        if ((attr != NULL) && (attr->cb_mem != NULL)) {
            if (attr->cb_size >= (sizeof(StaticTimer_t) + sizeof(timer_callback_t))) {
                callb = (timer_callback_t *)((uint32_t)attr->cb_mem + sizeof(StaticTimer_t));
            }
        }
#endif

#if (configSUPPORT_DYNAMIC_ALLOCATION == 1)
        /* Dynamic memory allocation is available: if memory for callback and */
        /* its argument is not provided, allocate it from dynamic memory pool */
        if (callb == NULL) {
            callb = (timer_callback_t *)pvPortMalloc(sizeof(timer_callback_t));

            if (callb != NULL) {
                /* Callback memory was allocated from dynamic pool, set flag */
                callb_dyn = 1U;
            }
        }
#endif

        if (callb != NULL) {
            callb->func = func;
            callb->arg  = argument;

            if (type == XF_OSAL_TIMER_ONCE) {
                reload = pdFALSE;
            } else {
                reload = pdTRUE;
            }

            mem  = -1;
            name = NULL;

            if (attr != NULL) {
                if (attr->name != NULL) {
                    name = attr->name;
                }

                if ((attr->cb_mem != NULL) && (attr->cb_size >= sizeof(StaticTimer_t))) {
                    /* The memory for control block is provided, use static object */
                    mem = 1;
                } else {
                    if ((attr->cb_mem == NULL) && (attr->cb_size == 0U)) {
                        /* Control block will be allocated from the dynamic pool */
                        mem = 0;
                    }
                }
            } else {
                mem = 0;
            }
            /* Store callback memory dynamic allocation flag */
            callb = (timer_callback_t *)((uint32_t)callb | callb_dyn);
            /*
              timer_callback function is always provided as a callback and is used to call application
              specified function with its argument both stored in structure callb.
            */
            if (mem == 1) {
#if (configSUPPORT_STATIC_ALLOCATION == 1)
                hTimer = xTimerCreateStatic(name, 1000, reload, callb, timer_callback, (StaticTimer_t *)attr->cb_mem);
#endif
            } else {
                if (mem == 0) {
#if (configSUPPORT_DYNAMIC_ALLOCATION == 1)
                    hTimer = xTimerCreate(name, 1000, reload, callb, timer_callback);
#endif
                }
            }

#if (configSUPPORT_DYNAMIC_ALLOCATION == 1)
            if ((hTimer == NULL) && (callb != NULL) && (callb_dyn == 1U)) {
                /* Failed to create a timer, release allocated resources */
                callb = (timer_callback_t *)((uint32_t)callb & ~1U);

                vPortFree(callb);
            }
#endif
        }
    }

    /* Return timer ID */
    return ((xf_osal_timer_t)hTimer);
}

const char *xf_osal_timer_get_name(xf_osal_timer_t timer)
{
    TimerHandle_t hTimer = (TimerHandle_t)timer;
    const char *p;

    if (hTimer == NULL) {
        p = NULL;
    } else if (IRQ_Context() != 0U) {
        /* Retrieve the name even though the function is not allowed to be called from ISR */
        /* Function implementation allows this therefore we make an exception.             */
        p = pcTimerGetName(hTimer);
    } else {
        p = pcTimerGetName(hTimer);
    }

    /* Return name as null-terminated string */
    return (p);
}

xf_err_t xf_osal_timer_start(xf_osal_timer_t timer, uint32_t ticks)
{
    TimerHandle_t hTimer = (TimerHandle_t)timer;
    xf_err_t stat = XF_OK;

    if (IRQ_Context() != 0U) {
        stat = XF_ERR_ISR;
    } else if ((hTimer == NULL) || (ticks == 0U)) {
        stat = XF_ERR_INVALID_ARG;
    } else {
        if (xTimerChangePeriod(hTimer, ticks, 0) == pdPASS) {
            stat = XF_OK;
        } else {
            stat = XF_ERR_RESOURCE;
        }
    }

    /* Return execution status */
    return (stat);
}

xf_err_t xf_osal_timer_stop(xf_osal_timer_t timer)
{
    TimerHandle_t hTimer = (TimerHandle_t)timer;
    xf_err_t stat;

    if (IRQ_Context() != 0U) {
        stat = XF_ERR_ISR;
    } else if (hTimer == NULL) {
        stat = XF_ERR_INVALID_ARG;
    } else {
        if (xTimerIsTimerActive(hTimer) == pdFALSE) {
            stat = XF_ERR_RESOURCE;
        } else {
            if (xTimerStop(hTimer, 0) == pdPASS) {
                stat = XF_OK;
            } else {
                stat = XF_FAIL;
            }
        }
    }

    /* Return execution status */
    return (stat);

}

uint32_t xf_osal_timer_is_running(xf_osal_timer_t timer)
{
    TimerHandle_t hTimer = (TimerHandle_t)timer;
    uint32_t running;

    if ((IRQ_Context() != 0U) || (hTimer == NULL)) {
        running = 0U;
    } else {
        running = (uint32_t)xTimerIsTimerActive(hTimer);
    }

    /* Return 0: not running, 1: running */
    return (running);
}

xf_err_t xf_osal_timer_delete(xf_osal_timer_t timer)
{
    TimerHandle_t hTimer = (TimerHandle_t)timer;
    xf_err_t stat;
#ifndef USE_FreeRTOS_HEAP_1
#if (configSUPPORT_DYNAMIC_ALLOCATION == 1)
    timer_callback_t *callb;
#endif

    if (IRQ_Context() != 0U) {
        stat = XF_ERR_ISR;
    } else if (hTimer == NULL) {
        stat = XF_ERR_INVALID_ARG;
    } else {
#if (configSUPPORT_DYNAMIC_ALLOCATION == 1)
        callb = (timer_callback_t *)pvTimerGetTimerID(hTimer);
#endif

        if (xTimerDelete(hTimer, 0) == pdPASS) {
#if (configSUPPORT_DYNAMIC_ALLOCATION == 1)
            if ((uint32_t)callb & 1U) {
                /* Callback memory was allocated from dynamic pool, clear flag */
                callb = (timer_callback_t *)((uint32_t)callb & ~1U);

                /* Return allocated memory to dynamic pool */
                vPortFree(callb);
            }
#endif
            stat = XF_OK;
        } else {
            stat = XF_ERR_RESOURCE;
        }
    }
#else
    stat = XF_FAIL;
#endif

    /* Return execution status */
    return (stat);

}

/* ==================== [Static Functions] ================================== */

static void timer_callback(TimerHandle_t hTimer)
{
    timer_callback_t *callb;

    /* Retrieve pointer to callback function and argument */
    callb = (timer_callback_t *)pvTimerGetTimerID(hTimer);

    /* Remove dynamic allocation flag */
    callb = (timer_callback_t *)((uint32_t)callb & ~1U);

    if (callb != NULL) {
        callb->func(callb->arg);
    }
}

#endif
