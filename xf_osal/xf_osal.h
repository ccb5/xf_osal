/**
 * @file xf_osal.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2023-12-07
 *
 * @copyright Copyright (c) 2023, CorAL. All rights reserved.
 *
 */

#ifndef __XF_OSAL_H__
#define __XF_OSAL_H__

/* ==================== [Includes] ========================================== */

#include "xf_osal_config_internal.h"
#include "xf_utils.h"

#if XF_OSAL_KERNEL_IS_ENABLE
#include "xf_osal_kernel.h"
#endif

#if XF_OSAL_THREAD_IS_ENABLE
#include "xf_osal_thread.h"
#endif

#if XF_OSAL_TIMER_IS_ENABLE
#include "xf_osal_timer.h"
#endif

#if XF_OSAL_EVENT_IS_ENABLE
#include "xf_osal_event.h"
#endif

#if XF_OSAL_MUTEX_IS_ENABLE
#include "xf_osal_mutex.h"
#endif

#if XF_OSAL_SEMAPHORE_IS_ENABLE
#include "xf_osal_semaphore.h"
#endif

#if XF_OSAL_QUEUE_IS_ENABLE
#include "xf_osal_queue.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_OSAL_H__
