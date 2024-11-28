/**
 * @file xf_osal_mutex.h
 * @author cangyu(sky.kirto@qq.com)
 * @brief 互斥 (Mutex) 同步资源访问
 * @version 0.1
 * @date 2023-12-06
 *
 * @copyright Copyright(c) 2023, CorAL. All rights reserved.
 *
 */

#if XF_OSAL_MUTEX_IS_ENABLE || defined(__DOXYGEN__)

#ifndef __XF_OSAL_MUTEX_H__
#define __XF_OSAL_MUTEX_H__

/* ==================== [Includes] ========================================== */

#include "xf_osal_def.h"
#include "xf_osal_thread.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/**
 * @brief 互斥锁递归属性。
 *
 * @details
 *
 * - **同一线程** 可以多次获取递归互斥锁，而不会锁定自身线程。
 * - 每次成功获取递归互斥锁时，锁计数都会增加。
 * - 递归互斥锁也必须被释放相同次数，锁定计数达到零时互斥锁被释放，其他线程可以获取。
 *
 * @note 最大递归锁定次数由实现决定，如果达到最大递归锁定次数则可能获取失败。
 * @note 并非所有平台都支持所有属性，需见具体实现。
 */
#define XF_OSAL_MUTEX_RECURSIVE        0x00000001U

/**
 * @brief 互斥锁继承属性。
 *
 * @details
 *
 * 对于带有优先级继承属性的互斥锁：
 *
 * - 会临时提升持有互斥锁的线程的优先级。提升到互斥锁等待列表中最高优先级线程的相同等级。
 * - 可以避免在多线程的情况下，因为锁的持有线程的的优先级较低，长时间无法调度，
 *   导致低优先级线程"锁住"高优先级线程（优先级反转）。
 * - 当持有互斥锁的线程释放锁，该线程优先级会恢复到原来等级。
 * - 即，带有优先级继承属性的互斥锁 **可以降低高优先级线程的等待时间。**
 *
 * 例如：
 *
 * - 有三个线程，他们的优先级分别是低、中、高，其中低优先级和高优先级都需要获取锁。
 * - 低优先级线程先获取了锁。高优先级线程后获取锁，但因互斥锁已经锁定而阻塞等待。
 * - 中优先级线程执行阻塞任务，一直不释放 CPU, 依赖调度器打断执行.
 * - 如果互斥锁 **不带有** 优先级继承属性：
 *   低优先级线程一直无法执行，导致高优先级线程无法执行（优先级反转）。
 * - 如果互斥锁 **带有** 优先级继承属性：
 *   低优先级线程临时提升至高优先级，打断中优先级线程，执行完毕后释放锁，
 *   使高优先级线程能够及时执行。
 *
 * @note 并非所有平台都支持所有属性，需见具体实现。
 */
#define XF_OSAL_MUTEX_PRIO_INHERIT      0x00000002U

/**
 * @brief 互斥锁健壮属性。
 *
 * 当锁的持有者线程终止时，互斥锁会自动释放。
 *
 * @note 并非所有平台都支持所有属性，需见具体实现。
 */
#define XF_OSAL_MUTEX_ROBUST           0x00000008U

/* ==================== [Typedefs] ========================================== */

/**
 * @brief 互斥锁句柄。
 */
typedef void *xf_osal_mutex_t;

/**
 * @brief 互斥锁的属性结构。
 */
typedef struct _xf_osal_mutex_attr_t {
    const char *name;       /*!< 互斥锁的名称，指向可读字符串。默认值: NULL. */
    uint32_t    attr_bits;  /*!< 属性位，默认值: 0.
                             *   可用属性：
                             *   - @ref XF_OSAL_MUTEX_RECURSIVE.
                             *   - @ref XF_OSAL_MUTEX_PRIO_INHERIT.
                             *   - @ref XF_OSAL_MUTEX_ROBUST.
                             *   默认值(0)情况下，互斥锁的属性是：
                             *   - 非递归互斥锁：线程不能多次使用互斥锁。
                             *   - 非优先级提升：拥有线程的优先级不会改变。
                             *   - 互斥锁不会自动释放：当所有者线程终止时，互斥锁对象必须始终自动释放。
                             */
    void       *cb_mem;     /*!< 控制块的内存，默认值: NULL, 即自动动态分配内存。 */
    uint32_t    cb_size;    /*!< 控制块内存大小（单位字节），不使用静态分配时设为默认值: 0. */
} xf_osal_mutex_attr_t;

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief 创建并初始化互斥锁。
 *
 * @param attr 互斥锁属性。填入 NULL 时使用默认属性。
 * @return xf_osal_mutex_t
 *      - NULL                  创建失败
 *      - (OTHER)               互斥锁句柄
 */
xf_osal_mutex_t xf_osal_mutex_create(const xf_osal_mutex_attr_t *attr);

/**
 * @brief 获取互斥锁。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param mutex 互斥锁句柄。获取自 @ref xf_osal_mutex_create().
 * @param timeout 超时时间，单位 tick.
 *      - 如需以 ms 为单位，请配合 @ref xf_osal_kernel_ms_to_ticks() 使用。
 *      - 一直等待，直到获取到锁（等待语义）：填入 @ref XF_OSAL_WAIT_FOREVER.
 *      - 尝试获取锁（尝试语义），无论成功与否都立刻返回：填入 0.
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_TIMEOUT        超时
 *      - XF_ERR_RESOURCE       未指定超时时无法获取互斥锁
 *      - XF_ERR_INVALID_ARG    无效参数
 *      - XF_ERR_ISR            禁止在中断服务函数中调用
 */
xf_err_t xf_osal_mutex_acquire(xf_osal_mutex_t mutex, uint32_t timeout);

/**
 * @brief 释放互斥锁。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param mutex 互斥锁句柄。获取自 @ref xf_osal_mutex_create().
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_RESOURCE       无法释放（未获取互斥锁，或当前线程不是锁持有者）
 *      - XF_ERR_INVALID_ARG    无效参数
 *      - XF_ERR_ISR            禁止在中断服务函数中调用
 */
xf_err_t xf_osal_mutex_release(xf_osal_mutex_t mutex);

/**
 * @brief 获取互斥锁持有者线程句柄。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param mutex 互斥锁句柄。获取自 @ref xf_osal_mutex_create().
 * @return xf_err_t
 *      - NULL                  没有线程已获取该锁，或发生错误
 *      - (OTHER)               线程句柄
 */
xf_osal_thread_t xf_osal_mutex_get_owner(xf_osal_mutex_t mutex);

/**
 * @brief 删除互斥锁。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param mutex 互斥锁句柄。获取自 @ref xf_osal_mutex_create().
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_RESOURCE       互斥锁处于无效状态
 *      - XF_ERR_INVALID_ARG    无效参数
 *      - XF_ERR_ISR            禁止在中断服务函数中调用
 */
xf_err_t xf_osal_mutex_delete(xf_osal_mutex_t mutex);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_OSAL_MUTEX_H__

#endif // XF_OSAL_MUTEX_IS_ENABLE
