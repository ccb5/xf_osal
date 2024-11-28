/**
 * @file xf_osal_timer.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief 创建和控制定时器和定时器回调函数。
 * @version 0.1
 * @date 2023-12-13
 *
 * @copyright Copyright (c) 2023, CorAL. All rights reserved.
 *
 */

#if XF_OSAL_TIMER_IS_ENABLE || defined(__DOXYGEN__)

#ifndef __XF_OSAL_TIMER_H__
#define __XF_OSAL_TIMER_H__

/* ==================== [Includes] ========================================== */

#include "xf_osal_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief 定时器类型。
 */
typedef enum _xf_osal_timer_type_t {
    XF_OSAL_TIMER_ONCE             = 0,     /*!< 单次定时器 */
    XF_OSAL_TIMER_PERIODIC         = 1,     /*!< 重复定时器 */
} xf_osal_timer_type_t;

/**
 * @brief 定时器句柄。
 */
typedef void *xf_osal_timer_t;

/**
 * @brief 定时器的属性结构。
 */
typedef struct _xf_osal_timer_attr_t {
    const char *name;       /*!< 定时器的名称，指向可读字符串。默认值: NULL. */
    uint32_t    attr_bits;  /*!< 属性位，保留，默认值: 0. */
    void       *cb_mem;     /*!< 控制块的内存，默认值: NULL, 即自动动态分配内存。 */
    uint32_t    cb_size;    /*!< 控制块内存大小（单位字节），不使用静态分配时设为默认值: 0. */
} xf_osal_timer_attr_t;

/**
 * @brief 定时器回调函数。
 */
typedef void (*xf_osal_timer_func_t)(void *argument);

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief 创建并初始化定时器。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param func      定时器回调函数。
 * @param type      定时器类型，单次或者周期定时器。见 @ref xf_osal_timer_type_t.
 * @param argument  定时器回调函数的参数。见 @ref xf_osal_timer_func_t.
 * @param attr      定时器属性。填入 NULL 时使用默认属性。
 * @return xf_osal_timer_t
 *      - NULL                  创建失败
 *      - (OTHER)               定时器句柄
 */
xf_osal_timer_t xf_osal_timer_create(
    xf_osal_timer_func_t func, xf_osal_timer_type_t type, void *argument,
    xf_osal_timer_attr_t *attr);

/**
 * @brief 获取定时器的名称。
 *
 * @note @b 可以 在中断服务函数中调用。
 *
 * @param timer 定时器句柄。
 * @return const char* 定时器名称字符串（以 '\0' 结尾）。
 */
const char *xf_osal_timer_get_name(xf_osal_timer_t timer);

/**
 * @brief 启动或重新启动定时器。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param timer 定时器句柄。
 * @param ticks 定时的时间刻度。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_RESOURCE       定时器处于无效状态
 *      - XF_ERR_ISR            禁止在中断服务函数中调用
 *      - XF_ERR_INVALID_ARG    无效参数
 */
xf_err_t xf_osal_timer_start(xf_osal_timer_t timer, uint32_t ticks);

/**
 * @brief 停止定时器。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param timer 定时器句柄。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_RESOURCE       定时器没有运行
 *      - XF_ERR_ISR            禁止在中断服务函数中调用
 *      - XF_ERR_INVALID_ARG    无效参数
 */
xf_err_t xf_osal_timer_stop(xf_osal_timer_t timer);

/**
 * @brief 检查定时器是否正在运行。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param timer 定时器句柄。
 * @return uint32_t
 *      - 0                     未运行或发生错误
 *      - 1                     正在运行
 */
uint32_t xf_osal_timer_is_running(xf_osal_timer_t timer);

/**
 * @brief 删除定时器。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param timer 定时器句柄。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_RESOURCE       定时器处于无效状态
 *      - XF_ERR_ISR            禁止在中断服务函数中调用
 *      - XF_ERR_INVALID_ARG    无效参数
 */
xf_err_t xf_osal_timer_delete(xf_osal_timer_t timer);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_OSAL_TIMER_H__

#endif // XF_OSAL_TIMER_IS_ENABLE
