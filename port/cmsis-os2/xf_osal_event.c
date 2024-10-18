/**
 * @file xf_osal_event.c
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

#if XF_OSAL_EVENT_IS_ENABLE

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

xf_osal_event_t xf_osal_event_create(const xf_osal_event_attr_t *attr)
{
    return (xf_osal_event_t)osEventFlagsNew((const osEventFlagsAttr_t *)attr);
}

xf_err_t xf_osal_event_set(xf_osal_event_t event, uint32_t flags)
{
    uint32_t status = osEventFlagsSet((osEventFlagsId_t)event, flags);
    xf_err_t err = (status & osFlagsError) ? (transform_to_xf_err(status)) : (XF_OK);

    return err;
}

xf_err_t xf_osal_event_clear(xf_osal_event_t event, uint32_t flags)
{
    uint32_t status = osEventFlagsClear((osEventFlagsId_t)event, flags);
    xf_err_t err = (status & osFlagsError) ? (transform_to_xf_err(status)) : (XF_OK);

    return err;
}

uint32_t xf_osal_event_get(xf_osal_event_t event)
{
    return osEventFlagsGet((osEventFlagsId_t)event);
}

xf_err_t xf_osal_event_wait(xf_osal_event_t event, uint32_t flags, uint32_t options, uint32_t timeout)
{
    uint32_t status = osEventFlagsWait((osEventFlagsId_t)event, flags, options, timeout);
    xf_err_t err = (status & osFlagsError) ? (transform_to_xf_err(status)) : (XF_OK);

    return err;
}

xf_err_t xf_osal_event_delete(xf_osal_event_t event)
{
    uint32_t status = osEventFlagsDelete((osEventFlagsId_t)event);
    xf_err_t err = (status & osFlagsError) ? (transform_to_xf_err(status)) : (XF_OK);

    return err;
}

/* ==================== [Static Functions] ================================== */

#endif
