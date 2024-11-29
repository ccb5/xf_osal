/**
 * @file xf_osal_queue.h
 * @author cangyu(sky.kirto@qq.com)
 * @brief 以类似 FIFO 的操作在线程之间交换消息。
 * @version 0.1
 * @date 2023-12-06
 *
 * @copyright Copyright(c) 2023, CorAL. All rights reserved.
 *
 */

#if XF_OSAL_QUEUE_IS_ENABLE || defined(__DOXYGEN__)

#ifndef __XF_OSAL_QUEUE_H__
#define __XF_OSAL_QUEUE_H__

/* ==================== [Includes] ========================================== */

#include "xf_osal_def.h"

/**
 * @cond XFAPI_USER
 * @ingroup group_xf_osal
 * @defgroup group_xf_osal_queue queue
 * @brief 以类似 FIFO 的操作在线程之间交换消息。
 * @endcond
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief 消息队列句柄。
 */
typedef void *xf_osal_queue_t;

/**
 * @brief 消息队列的属性结构。
 */
typedef struct _xf_osal_queue_attr_t {
    const char *name;       /*!< 消息队列的名称，指向可读字符串。默认值: NULL. */
    uint32_t    attr_bits;  /*!< 属性位，保留，默认值: 0. */
    void       *cb_mem;     /*!< 控制块的内存，默认值: NULL, 即自动动态分配内存。 */
    uint32_t    cb_size;    /*!< 控制块内存大小（单位字节），不使用静态分配时设为默认值: 0. */
    void       *mq_mem;     /*!< 用于存储数据的内存，默认值: NULL, 即自动动态分配内存。 */
    uint32_t    mq_size;    /*!< 数据内存大小（单位字节），不使用静态分配时设为默认值: 0. */
} xf_osal_queue_attr_t;

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief 创建并初始化消息队列对象。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param msg_count 队列中的最大消息数。
 * @param msg_size  最大消息大小（以字节为单位）。
 * @param attr      消息队列属性。填入 NULL 时使用默认属性。
 * @return xf_osal_queue_t
 *      - NULL                  创建失败
 *      - (OTHER)               队列句柄
 */
xf_osal_queue_t xf_osal_queue_create(
    uint32_t msg_count, uint32_t msg_size, const xf_osal_queue_attr_t *attr);

/**
 * @brief 将消息放入队列，如果队列已满，则超时。
 *
 * @note 如果 timeout 为 0，则 @b 可以 在中断服务函数中调用。
 *
 * @param queue     队列句柄。从 @ref xf_osal_queue_create() 获取。
 * @param msg_ptr   指向缓冲区的指针，其中包含要放入队列的消息。
 * @param msg_prio  消息优先级。
 * @param timeout   超时时间，单位 tick.
 *      - 如需以 ms 为单位，请配合 @ref xf_osal_kernel_ms_to_ticks() 使用。
 *      - 一直等待，直到成功放入消息（等待语义）：填入 @ref XF_OSAL_WAIT_FOREVER.
 *      - 尝试放入消息（尝试语义），无论成功与否都立刻返回：填入 0.
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_TIMEOUT        超时，无法在给定时间内放入消息
 *      - XF_ERR_RESOURCE       队列中没有足够的空间
 *      - XF_ERR_INVALID_ARG    无效参数
 */
xf_err_t xf_osal_queue_put(
    xf_osal_queue_t queue, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout);

/**
 * @brief 从队列获取消息，如果队列为空，则超时。
 *
 * @note 如果 timeout 为 0，则 @b 可以 在中断服务函数中调用。
 *
 * @param queue         队列句柄。从 @ref xf_osal_queue_create() 获取。
 * @param[out] msg_ptr  指向从队列获取消息的缓冲区的指针。
 * @param[out] msg_prio 指向消息优先级缓冲区的指针或 NULL。
 * @param timeout       超时时间，单位 tick.
 *      - 如需以 ms 为单位，请配合 @ref xf_osal_kernel_ms_to_ticks() 使用。
 *      - 一直等待，直到成功获取消息（等待语义）：填入 @ref XF_OSAL_WAIT_FOREVER.
 *      - 尝试获取消息（尝试语义），无论成功与否都立刻返回：填入 0.
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_TIMEOUT        超时，无法在给定时间内获取消息
 *      - XF_ERR_RESOURCE       队列中没有数据
 *      - XF_ERR_INVALID_ARG    无效参数
 */
xf_err_t xf_osal_queue_get(
    xf_osal_queue_t queue, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout);

/**
 * @brief 获取消息队列中排队的消息数。
 *
 * @note @b 可以 在中断服务函数中调用。
 *
 * @param queue 队列句柄。从 @ref xf_osal_queue_create() 获取。
 * @return uint32_t 排队消息的数量。
 */
uint32_t xf_osal_queue_get_count(xf_osal_queue_t queue);

/**
 * @brief 将消息队列重置为初始空状态。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param queue 队列句柄。从 @ref xf_osal_queue_create() 获取。
 * @return xf_err_t 指示函数执行状态的状态代码。
 */
xf_err_t xf_osal_queue_reset(xf_osal_queue_t queue);

/**
 * @brief 删除消息队列对象。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param queue 队列句柄。从 @ref xf_osal_queue_create() 获取。
 * @return xf_err_t 指示函数执行状态的状态代码。
 */
xf_err_t xf_osal_queue_delete(xf_osal_queue_t queue);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * End of defgroup group_xf_osal_queue queue
 * @}
 */

#endif // __XF_OSAL_QUEUE_H__

#endif // XF_OSAL_QUEUE_IS_ENABLE
