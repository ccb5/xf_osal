/**
 * @file xf_osal_event.h
 * @author cangyu(sky.kirto@qq.com)
 * @brief 事件标志可以同步线程。
 * @version 0.1
 * @date 2023-12-06
 *
 * @copyright Copyright(c) 2023, CorAL. All rights reserved.
 *
 * XF_OSAL 中的事件标志管理功能允许您控制或等待事件标志。
 * 每个信号最多有 31 个事件标志（实际可用个数根据对接平台决定）。
 *
 * 对于线程：
 *
 * - 可以等待事件标志 @ref xf_osal_event_wait.
 * - 可以在任何其他给定线程中设置一个或多个标志 @ref xf_osal_event_set
 * - 可以清除自己的信号或其他线程的信号 @ref xf_osal_event_clear
 *
 * 当线程唤醒并恢复执行时，其信号标志会自动清除（除非指定了 @ref XF_OSAL_NO_CLEAR. ）。
 *
 * @note 可以在中断服务函数中调用的函数：
 *      - xf_osal_event_set()
 *      - xf_osal_event_clear()
 *      - xf_osal_event_get()
 *      - xf_osal_event_wait()
 */

#if XF_OSAL_EVENT_IS_ENABLE || defined(__DOXYGEN__)

#ifndef __XF_OSAL_EVENT_H__
#define __XF_OSAL_EVENT_H__

/* ==================== [Includes] ========================================== */

#include "xf_osal_def.h"

/**
 * @cond XFAPI_USER
 * @ingroup group_xf_osal
 * @defgroup group_xf_osal_event event
 * @brief 事件标志可以同步线程。
 * @endcond
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/**
 * @brief FreeRTOS 对接中，事件组里用户可用的位数(bit0 ~ bit23)。
 */
#define MAX_BITS_EVENT_GROUPS       24U

/**
 * @brief FreeRTOS 对接里，无效（保留）事件位掩码(bit24 ~ bit31)。
 */
#define XF_OSAL_EVENT_FLAGS_INVALID_BITS    (~((1UL << MAX_BITS_EVENT_GROUPS) - 1U))

/* ==================== [Typedefs] ========================================== */

/**
 * @brief 事件句柄。
 */
typedef void *xf_osal_event_t;

/**
 * @brief 事件标志的属性结构。
 */
typedef struct _xf_osal_event_attr_t {
    const char  *name;       /*!< 事件标志的名称，指向可读字符串。默认值: NULL. */
    uint32_t     attr_bits;  /*!< 属性位，保留，默认值: 0. */
    void        *cb_mem;     /*!< 控制块的内存，默认值: NULL, 即自动动态分配内存。 */
    uint32_t     cb_size;    /*!< 控制块内存大小（单位字节），不使用静态分配时设为默认值: 0. */
} xf_osal_event_attr_t;

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief 创建并初始化事件标志对象。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param attr 事件标志属性。填入 NULL 时使用默认属性，即使用内核内存分配事件控制块。
 * @return xf_osal_event_t
 *      - NULL                  创建失败
 *      - (OTHER)               事件句柄
 */
xf_osal_event_t xf_osal_event_create(const xf_osal_event_attr_t *attr);

/**
 * @brief 设置指定的事件标志。
 *
 * @note @b 可以 在中断服务函数中调用。
 *
 * @param event 事件句柄。从 @ref xf_osal_event_create() 获取。
 * @param flags 需要设置的标志。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_INVALID_ARG    无效参数，句柄无效或 flags 设置了最高位
 */
xf_err_t xf_osal_event_set(xf_osal_event_t event, uint32_t flags);

/**
 * @brief 清除指定的事件标志。
 *
 * @note @b 可以 在中断服务函数中调用。
 *
 * @param event 事件句柄。从 @ref xf_osal_event_create() 获取。
 * @param flags 需要清除的标志。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_RESOURCE       事件句柄处于无效状态
 *      - XF_ERR_INVALID_ARG    无效参数，句柄无效或 flags 设置了最高位
 */
xf_err_t xf_osal_event_clear(xf_osal_event_t event, uint32_t flags);

/**
 * @brief 获取当前事件标志。
 *
 * @note @b 可以 在中断服务函数中调用。
 *
 * @param event 事件句柄。从 @ref xf_osal_event_create() 获取。
 * @return uint32_t 当前事件标志
 */
uint32_t xf_osal_event_get(xf_osal_event_t event);

/**
 * @brief 等待一个或多个事件标志发出信号。
 *
 * @note timeout 为 NULL 时， @b 可以 在中断服务函数中调用。
 *
 * @param event 事件句柄。从 @ref xf_osal_event_create() 获取。
 * @param flags 需要等待的标志。
 * @param options 指定标志选项。
 *      - 见 @ref XF_OSAL_WAIT_ANY.
 *      - 见 @ref XF_OSAL_WAIT_ALL.
 *      - 见 @ref XF_OSAL_NO_CLEAR. 如果设置此标志，则不会自动清除等待的标志。
 * @param timeout 超时值，单位 ticks.
 *                可以为 0 或 @ref XF_OSAL_WAIT_FOREVER.
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_TIMEOUT        超时
 *      - XF_ERR_RESOURCE       在未指定超时的情况下，未设置等待标记
 *      - XF_ERR_INVALID_ARG    无效参数，句柄无效或 flags 设置了最高位
 */
xf_err_t xf_osal_event_wait(
    xf_osal_event_t event, uint32_t flags, uint32_t options, uint32_t timeout);

/**
 * @brief 删除事件标志对象。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param event 事件句柄。从 @ref xf_osal_event_create() 获取。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_ISR            禁止在中断服务函数中调用
 *      - XF_ERR_RESOURCE       事件句柄处于无效状态
 *      - XF_ERR_INVALID_ARG    无效参数，句柄无效或 flags 设置了最高位
 */
xf_err_t xf_osal_event_delete(xf_osal_event_t event);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * End of defgroup group_xf_osal_event event
 * @}
 */

#endif // __XF_OSAL_EVENT_H__

#endif // XF_OSAL_EVENT_IS_ENABLE
