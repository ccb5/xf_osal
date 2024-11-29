/**
 * @file xf_osal_timer.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-09-20
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_osal_internal.h"

#if XF_OSAL_TIMER_IS_ENABLE

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

xf_osal_timer_t xf_osal_timer_create(xf_osal_timer_func_t func, xf_osal_timer_type_t type, void *argument,
                                     xf_osal_timer_attr_t *attr)
{
    return (xf_osal_timer_t)osTimerNew((osTimerFunc_t)func, (osTimerType_t)type, argument, (const osTimerAttr_t *)attr);
}

const char *xf_osal_timer_get_name(xf_osal_timer_t timer)
{
#if XF_CMSIS_TIMER_GET_NAME_IS_ENABLE
    return osTimerGetName((osTimerId_t)timer);
#else
    return NULL;
#endif
}

xf_err_t xf_osal_timer_start(xf_osal_timer_t timer, uint32_t ticks)
{
    osStatus_t status = osTimerStart((osTimerId_t)timer, ticks);
    xf_err_t err = transform_to_xf_err(status);

    return err;
}

xf_err_t xf_osal_timer_stop(xf_osal_timer_t timer)
{
    osStatus_t status = osTimerStop((osTimerId_t)timer);
    xf_err_t err = transform_to_xf_err(status);

    return err;
}

uint32_t xf_osal_timer_is_running(xf_osal_timer_t timer)
{
    return osTimerIsRunning((osTimerId_t)timer);
}

xf_err_t xf_osal_timer_delete(xf_osal_timer_t timer)
{
    osStatus_t status = osTimerDelete((osTimerId_t)timer);
    xf_err_t err = transform_to_xf_err(status);

    return err;
}

/* ==================== [Static Functions] ================================== */

#endif
