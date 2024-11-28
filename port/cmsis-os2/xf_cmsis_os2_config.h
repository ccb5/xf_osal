/**
 * @file xf_cmsis_os2_config.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-09-24
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_CMSIS_OS2_CONFIG_H__
#define __XF_CMSIS_OS2_CONFIG_H__

/* ==================== [Includes] ========================================== */

#include "xf_osal_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#if (!defined(XF_CMSIS_KERNEL_GET_INFO_ENABLE) || (XF_CMSIS_KERNEL_GET_INFO_ENABLE) || defined(__DOXYGEN__))
#define XF_CMSIS_KERNEL_GET_INFO_IS_ENABLE (1)
#else
#define XF_CMSIS_KERNEL_GET_INFO_IS_ENABLE (0)
#endif

#if (!defined(XF_CMSIS_KERNEL_GET_STATE_ENABLE) || (XF_CMSIS_KERNEL_GET_STATE_ENABLE) || defined(__DOXYGEN__))
#define XF_CMSIS_KERNEL_GET_STATE_IS_ENABLE (1)
#else
#define XF_CMSIS_KERNEL_GET_STATE_IS_ENABLE (0)
#endif

#if (!defined(XF_CMSIS_KERNEL_LOCK_ENABLE) || (XF_CMSIS_KERNEL_LOCK_ENABLE) || defined(__DOXYGEN__))
#define XF_CMSIS_KERNEL_LOCK_IS_ENABLE (1)
#else
#define XF_CMSIS_KERNEL_LOCK_IS_ENABLE (0)
#endif

#if (!defined(XF_CMSIS_KERNEL_UNLOCK_ENABLE) || (XF_CMSIS_KERNEL_UNLOCK_ENABLE) || defined(__DOXYGEN__))
#define XF_CMSIS_KERNEL_UNLOCK_IS_ENABLE (1)
#else
#define XF_CMSIS_KERNEL_UNLOCK_IS_ENABLE (0)
#endif

#if (!defined(XF_CMSIS_THREAD_GET_NAME_ENABLE) || (XF_CMSIS_THREAD_GET_NAME_ENABLE) || defined(__DOXYGEN__))
#define XF_CMSIS_THREAD_GET_NAME_IS_ENABLE (1)
#else
#define XF_CMSIS_THREAD_GET_NAME_IS_ENABLE (0)
#endif

#if (!defined(XF_CMSIS_THREAD_SET_PRIORITY_ENABLE) || (XF_CMSIS_THREAD_SET_PRIORITY_ENABLE) || defined(__DOXYGEN__))
#define XF_CMSIS_THREAD_SET_PRIORITY_IS_ENABLE (1)
#else
#define XF_CMSIS_THREAD_SET_PRIORITY_IS_ENABLE (0)
#endif

#if (!defined(XF_CMSIS_THREAD_YIELD_ENABLE) || (XF_CMSIS_THREAD_YIELD_ENABLE) || defined(__DOXYGEN__))
#define XF_CMSIS_THREAD_YIELD_IS_ENABLE (1)
#else
#define XF_CMSIS_THREAD_YIELD_IS_ENABLE (0)
#endif

#if (!defined(XF_CMSIS_THREAD_DELETE_NULL_BASE_ON_TERMINATE_ENABLE) || (XF_CMSIS_THREAD_DELETE_NULL_BASE_ON_TERMINATE_ENABLE) || defined(__DOXYGEN__))
#define XF_CMSIS_THREAD_DELETE_NULL_BASE_ON_TERMINATE_IS_ENABLE (1)
#else
#define XF_CMSIS_THREAD_DELETE_NULL_BASE_ON_TERMINATE_IS_ENABLE (0)
#endif

#if (!defined(XF_CMSIS_THREAD_GET_COUNT_ENABLE) || (XF_CMSIS_THREAD_GET_COUNT_ENABLE) || defined(__DOXYGEN__))
#define XF_CMSIS_THREAD_GET_COUNT_IS_ENABLE (1)
#else
#define XF_CMSIS_THREAD_GET_COUNT_IS_ENABLE (0)
#endif

#if (!defined(XF_CMSIS_THREAD_ENUMERATE_ENABLE) || (XF_CMSIS_THREAD_ENUMERATE_ENABLE) || defined(__DOXYGEN__))
#define XF_CMSIS_THREAD_ENUMERATE_IS_ENABLE (1)
#else
#define XF_CMSIS_THREAD_ENUMERATE_IS_ENABLE (0)
#endif

#if (!defined(XF_CMSIS_THREAD_NOTIFY_ENABLE) || (XF_CMSIS_THREAD_NOTIFY_ENABLE) || defined(__DOXYGEN__))
#define XF_CMSIS_THREAD_NOTIFY_IS_ENABLE (1)
#else
#define XF_CMSIS_THREAD_NOTIFY_IS_ENABLE (0)
#endif

#if (!defined(XF_CMSIS_TIMER_GET_NAME_ENABLE) || (XF_CMSIS_TIMER_GET_NAME_ENABLE) || defined(__DOXYGEN__))
#define XF_CMSIS_TIMER_GET_NAME_IS_ENABLE (1)
#else
#define XF_CMSIS_TIMER_GET_NAME_IS_ENABLE (0)
#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_CMSIS_OS2_CONFIG_H__
