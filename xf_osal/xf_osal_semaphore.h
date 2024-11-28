/**
 * @file xf_osal_semaphore.h
 * @author cangyu(sky.kirto@qq.com)
 * @brief 信号量可以管理和保护对共享资源的访问。
 * @version 0.1
 * @date 2023-12-06
 *
 * @copyright Copyright(c) 2023, CorAL. All rights reserved.
 *
 */

#if XF_OSAL_SEMAPHORE_IS_ENABLE || defined(__DOXYGEN__)

#ifndef __XF_OSAL_SEMAPHORE_H__
#define __XF_OSAL_SEMAPHORE_H__

/* ==================== [Includes] ========================================== */

#include "xf_osal_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief 信号量句柄。
 */
typedef void *xf_osal_semaphore_t;

/**
 * @brief 信号量的属性结构。
 */
typedef struct _xf_osal_semaphore_attr_t {
    const char *name;       /*!< 信号量的名称，指向可读字符串。默认值: NULL. */
    uint32_t    attr_bits;  /*!< 属性位，保留，默认值: 0. */
    void       *cb_mem;     /*!< 控制块的内存，默认值: NULL, 即自动动态分配内存。 */
    uint32_t    cb_size;    /*!< 控制块内存大小（单位字节），不使用静态分配时设为默认值: 0. */
} xf_osal_semaphore_attr_t;

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief 创建并初始化信号量对象。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param max_count     可用令牌的最大数量。
 * @param initial_count 可用令牌的初始数量。
 * @param attr          信号量属性。填入 NULL 时使用默认属性。
 * @return xf_osal_semaphore_t
 *      - NULL                  创建失败
 *      - (OTHER)               信号量句柄
 */
xf_osal_semaphore_t xf_osal_semaphore_create(
    uint32_t max_count, uint32_t initial_count, const xf_osal_semaphore_attr_t *attr);

/**
 * @brief 获取信号量令牌，如果没有可用令牌则超时。
 *
 * @note 如果 timeout 为 0，则 @b 可以 在中断服务函数中调用。
 *
 * @param semaphore 信号量句柄。从 @ref xf_osal_semaphore_create() 获取。
 * @param timeout 超时时间，单位 tick.
 *      - 如需以 ms 为单位，请配合 @ref xf_osal_kernel_ms_to_ticks() 使用。
 *      - 一直等待，直到获取到信号量（等待语义）：填入 @ref XF_OSAL_WAIT_FOREVER.
 *      - 尝试获取信号量（尝试语义），无论成功与否都立刻返回：填入 0.
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_TIMEOUT        超时，指定时间内无法获取令牌
 *      - XF_ERR_RESOURCE       未指定超时时无法获取令牌
 *      - XF_ERR_INVALID_ARG    无效参数
 */
xf_err_t xf_osal_semaphore_acquire(xf_osal_semaphore_t semaphore, uint32_t timeout);

/**
 * @brief 释放信号量令牌直至初始最大计数。
 *
 * @note @b 可以 在中断服务函数中调用。
 *
 * @param semaphore 信号量句柄。从 @ref xf_osal_semaphore_create() 获取。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_RESOURCE       无法释放（已达到最大令牌计数）
 *      - XF_ERR_INVALID_ARG    无效参数
 *      - XF_ERR_ISR            禁止在中断服务函数中调用
 */
xf_err_t xf_osal_semaphore_release(xf_osal_semaphore_t semaphore);

/**
 * @brief 获取当前信号量令牌计数。
 *
 * @note @b 可以 在中断服务函数中调用。
 *
 * @param semaphore 信号量句柄。从 @ref xf_osal_semaphore_create() 获取。
 * @return uint32_t 可用令牌的数量。
 */
uint32_t xf_osal_semaphore_get_count(xf_osal_semaphore_t semaphore);

/**
 * @brief 删除信号量对象。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param semaphore 信号量句柄。从 @ref xf_osal_semaphore_create() 获取。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_RESOURCE       信号量处于无效状态
 *      - XF_ERR_INVALID_ARG    无效参数
 *      - XF_ERR_ISR            禁止在中断服务函数中调用
 */
xf_err_t xf_osal_semaphore_delete(xf_osal_semaphore_t semaphore);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_OSAL_SEMAPHORE_H__

#endif // XF_OSAL_SEMAPHORE_IS_ENABLE
