/**
 * @file xf_osal_def.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief xf_osal 内部公共定义、函数、数据。
 * @version 1.0
 * @date 2024-11-27
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_OSAL_DEF_H__
#define __XF_OSAL_DEF_H__

/* ==================== [Includes] ========================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define XF_OSAL_WAIT_FOREVER        0xFFFFFFFFU /*!< 一直等待。*/

/**
 * @brief 标志选项。 @ref xf_osal_event_wait() 或 @ref xf_osal_thread_notify_wait().
 */
#define XF_OSAL_WAIT_ANY            0x00000000U /*!< 等待任何标志（默认）。*/
#define XF_OSAL_WAIT_ALL            0x00000001U /*!< 等待所有标志。*/
#define XF_OSAL_NO_CLEAR            0x00000002U /*!< 不要清除已指定等待的标志。*/

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __XF_OSAL_DEF_H__ */
