/**
 * @file xf_osal_kernel.c
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

#if XF_OSAL_KERNEL_IS_ENABLE

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

xf_err_t xf_osal_kernel_get_info(xf_osal_version_t *version, char *id_buf, uint32_t id_size)
{
#if XF_CMSIS_KERNEL_GET_INFO_IS_ENABLE
    osStatus_t status = osKernelGetInfo((osVersion_t *)version, id_buf, id_size);
    xf_err_t err = transform_to_xf_err(status);

    return err;
#else
    return XF_ERR_NOT_SUPPORTED;
#endif
}

xf_osal_state_t xf_osal_kernel_get_state(void)
{
#if XF_CMSIS_KERNEL_GET_STATE_IS_ENABLE
    osKernelState_t state = osKernelGetState();

    return (xf_osal_state_t)state;
#else
    return XF_OSAL_ERROR;
#endif
}

xf_err_t xf_osal_kernel_lock(void)
{
#if XF_CMSIS_KERNEL_LOCK_IS_ENABLE
    /* res: 调度器之前的锁定状态 */
    int32_t res = osKernelLock();
    xf_err_t err = XF_OK;
    if (res < 0) {
        err = transform_to_xf_err(res);
        return err;
    }
    return XF_OK;
#else
    return XF_ERR_NOT_SUPPORTED;
#endif
}

xf_err_t xf_osal_kernel_unlock(void)
{
#if XF_CMSIS_KERNEL_UNLOCK_IS_ENABLE
    /* res: 调度器之前的锁定状态 */
    int32_t res = osKernelUnlock();
    xf_err_t err = XF_OK;
    if (res < 0) {
        err = transform_to_xf_err(res);
        return err;
    }
    return XF_OK;
#else
    return XF_ERR_NOT_SUPPORTED;
#endif
}

uint32_t xf_osal_kernel_get_tick_count(void)
{
    return osKernelGetTickCount();
}

uint32_t xf_osal_kernel_get_tick_freq(void)
{
    return osKernelGetTickFreq();
}

uint32_t xf_osal_kernel_ticks_to_ms(uint32_t ticks)
{
    return ticks * 1000 / osKernelGetTickFreq();
}

uint32_t xf_osal_kernel_ms_to_ticks(uint32_t ms)
{
    return ms * osKernelGetTickFreq() / 1000;
}

/* ==================== [Static Functions] ================================== */

#endif
