/**
 * @file xf_osal_thread.c
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

#if XF_OSAL_THREAD_IS_ENABLE

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

xf_osal_thread_t xf_osal_thread_create(xf_osal_thread_func_t func, void *argument, const xf_osal_thread_attr_t *attr)
{
    return (xf_osal_thread_t)osThreadNew((osThreadFunc_t)func, argument, (const osThreadAttr_t *)attr);
}

const char *xf_osal_thread_get_name(xf_osal_thread_t thread)
{
#if XF_CMSIS_THREAD_GET_NAME_IS_ENABLE
    return osThreadGetName((osThreadId_t)thread);
#else
    return NULL;
#endif
}

xf_osal_thread_t xf_osal_thread_get_current(void)
{
    return (xf_osal_thread_t)osThreadGetId();
}

xf_osal_state_t xf_osal_thread_get_state(xf_osal_thread_t thread)
{
    return (xf_osal_state_t)osThreadGetState((osThreadId_t)thread);
}

uint32_t xf_osal_thread_get_stack_space(xf_osal_thread_t thread)
{
    return osThreadGetStackSpace((osThreadId_t)thread);
}

xf_err_t xf_osal_thread_set_priority(xf_osal_thread_t thread, xf_osal_priority_t priority)
{
#if XF_CMSIS_THREAD_SET_PRIORITY_IS_ENABLE
    osStatus_t status = osThreadSetPriority((osThreadId_t)thread, (osPriority_t)priority);
    xf_err_t err = transform_to_xf_err(status);

    return err;
#else
    return XF_ERR_NOT_SUPPORTED;
#endif
}

xf_osal_priority_t xf_osal_thread_get_priority(xf_osal_thread_t thread)
{
    return (xf_osal_priority_t)osThreadGetPriority((osThreadId_t)thread);
}

xf_err_t xf_osal_thread_yield(void)
{
#if XF_CMSIS_THREAD_YIELD_IS_ENABLE
    osStatus_t status = osThreadYield();
    xf_err_t err = transform_to_xf_err(status);

    return err;
#else
    return XF_ERR_NOT_SUPPORTED;
#endif
}

xf_err_t xf_osal_thread_suspend(xf_osal_thread_t thread)
{
    osStatus_t status = osThreadSuspend((osThreadId_t)thread);
    xf_err_t err = transform_to_xf_err(status);

    return err;
}

xf_err_t xf_osal_thread_resume(xf_osal_thread_t thread)
{
    osStatus_t status = osThreadResume((osThreadId_t)thread);
    xf_err_t err = transform_to_xf_err(status);

    return err;
}

xf_err_t xf_osal_thread_delete(xf_osal_thread_t thread)
{
    xf_err_t err = XF_OK;

    osStatus_t status;
    if (NULL == thread) {
#if XF_CMSIS_THREAD_DELETE_NULL_BASE_ON_TERMINATE_IS_ENABLE
        thread = osThreadGetId();
        if (NULL == thread) {
            return XF_FAIL;
        }
#else
        osThreadExit();
#endif
    }
    status = osThreadTerminate((osThreadId_t)thread);
    err = transform_to_xf_err(status);

    return err;
}

uint32_t xf_osal_thread_get_count(void)
{
#if XF_CMSIS_THREAD_GET_COUNT_IS_ENABLE
    return osThreadGetCount();
#else
    return 0U;
#endif
}

uint32_t xf_osal_thread_get_active_count(xf_osal_thread_t *thread_array, uint32_t array_items)
{
#if XF_CMSIS_THREAD_GET_ACTIVE_COUNT_IS_ENABLE
    return osThreadEnumerate((osThreadId_t)thread_array, array_items);
#else
    return 0U;
#endif
}

xf_err_t xf_osal_thread_notify_set(xf_osal_thread_t thread, uint32_t notify)
{
#if XF_CMSIS_THREAD_NOTIFY_IS_ENABLE
    uint32_t status = osThreadFlagsSet((osThreadId_t)thread, (uint32_t)notify);
    xf_err_t err = (status & osFlagsError) ? (transform_to_xf_err(status)) : (XF_OK);

    return err;
#else
    return XF_ERR_NOT_SUPPORTED;
#endif
}

xf_err_t xf_osal_thread_notify_clear(uint32_t notify)
{
#if XF_CMSIS_THREAD_NOTIFY_IS_ENABLE
    uint32_t status = osThreadFlagsClear((uint32_t)notify);
    xf_err_t err = (status & osFlagsError) ? (transform_to_xf_err(status)) : (XF_OK);

    return err;
#else
    return XF_ERR_NOT_SUPPORTED;
#endif
}

uint32_t xf_osal_thread_notify_get(void)
{
#if XF_CMSIS_THREAD_NOTIFY_IS_ENABLE
    return osThreadFlagsGet();
#else
    return 0U;
#endif
}

xf_err_t xf_osal_thread_notify_wait(uint32_t notify, uint32_t options, uint32_t timeout)
{
#if XF_CMSIS_THREAD_NOTIFY_IS_ENABLE
    uint32_t status = osThreadFlagsWait(notify, options, timeout);
    xf_err_t err = (status & osFlagsError) ? (transform_to_xf_err(status)) : (XF_OK);

    return err;
#else
    return XF_ERR_NOT_SUPPORTED;
#endif
}

xf_err_t xf_osal_delay(uint32_t ticks)
{
    osStatus_t status = osDelay(ticks);
    xf_err_t err = transform_to_xf_err(status);

    return err;
}

xf_err_t xf_osal_delay_until(uint32_t ticks)
{
    osStatus_t status = osDelayUntil(ticks);
    xf_err_t err = transform_to_xf_err(status);

    return err;
}

xf_err_t xf_osal_delay_ms(uint32_t ms)
{
    return (xf_osal_delay(xf_osal_kernel_ms_to_ticks(ms)));
}

/* ==================== [Static Functions] ================================== */

#endif
