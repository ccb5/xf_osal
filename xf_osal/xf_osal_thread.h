/**
 * @file xf_osal_thread.h
 * @author cangyu(sky.kirto@qq.com)
 * @brief 定义、创建和控制线程函数。
 * @version 0.1
 * @date 2023-12-06
 *
 * @copyright Copyright(c) 2023, CorAL. All rights reserved.
 *
 */

#if XF_OSAL_THREAD_IS_ENABLE || defined(__DOXYGEN__)

#ifndef __XF_OSAL_THREAD_H__
#define __XF_OSAL_THREAD_H__

/* ==================== [Includes] ========================================== */

#include "xf_osal_def.h"
#include "xf_osal_kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#ifndef __NO_RETURN
#   if defined(__CC_ARM)
#       define __NO_RETURN __declspec(noreturn)
#   elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#       define __NO_RETURN __attribute__((__noreturn__))
#   elif defined(__GNUC__)
#       define __NO_RETURN __attribute__((__noreturn__))
#   elif defined(__ICCARM__)
#       define __NO_RETURN __noreturn
#   else
#       define __NO_RETURN
#   endif
#endif

#define XF_OSAL_DETACHED       0x00000000U /*!< 以分离模式创建线程（默认） */
#define XF_OSAL_JOINABLE       0x00000001U /*!< 以可加入模式创建的线程 */
#define XF_OSAL_UNPRIVILEGED   0x00000002U /*!< 线程以非特权模式运行 */
#define XF_OSAL_PRIVIEGED      0x00000004U /*!< 线程在特权模式下运行 */

/**
 * @brief FreeRTOS 对接中，用户可用的线程通知位数(bit0 ~ bit30)。
 */
#define MAX_BITS_TASK_NOTIFY        31U

/**
 * @brief FreeRTOS 对接里，无效（保留）事件位掩码(bit31)。
 */
#define THREAD_FLAGS_INVALID_BITS   (~((1UL << MAX_BITS_TASK_NOTIFY)  - 1U))

/* ==================== [Typedefs] ========================================== */

/**
 * @brief 线程优先级。
 *
 * 默认线程优先级是 XF_OSAL_PRIORITY_NORMOL.
 *
 * 如果一个活动线程准备就绪，且其优先级高于当前正在运行的线程，
 * 则立即发生线程切换。系统继续执行优先级较高的线程。
 */
typedef enum _xf_osal_priority_t {
    XF_OSAL_PRIORITY_NONE           =  0,         /*!< 无优先级（未初始化） */
    XF_OSAL_PRIORITY_IDLE           =  1,         /*!< 为空闲线程保留 */
    XF_OSAL_PRIORITY_LOW            =  8,         /*!< Priority: low */
    XF_OSAL_PRIORITY_LOW1           =  8 + 1,     /*!< Priority: low + 1 */
    XF_OSAL_PRIORITY_LOW2           =  8 + 2,     /*!< Priority: low + 2 */
    XF_OSAL_PRIORITY_LOW3           =  8 + 3,     /*!< Priority: low + 3 */
    XF_OSAL_PRIORITY_LOW4           =  8 + 4,     /*!< Priority: low + 4 */
    XF_OSAL_PRIORITY_LOW5           =  8 + 5,     /*!< Priority: low + 5 */
    XF_OSAL_PRIORITY_LOW6           =  8 + 6,     /*!< Priority: low + 6 */
    XF_OSAL_PRIORITY_LOW7           =  8 + 7,     /*!< Priority: low + 7 */
    XF_OSAL_PRIORITY_BELOW_NORMAL   = 16,         /*!< Priority: below normal */
    XF_OSAL_PRIORITY_BELOW_NORMAL1  = 16 + 1,     /*!< Priority: below normal + 1 */
    XF_OSAL_PRIORITY_BELOW_NORMAL2  = 16 + 2,     /*!< Priority: below normal + 2 */
    XF_OSAL_PRIORITY_BELOW_NORMAL3  = 16 + 3,     /*!< Priority: below normal + 3 */
    XF_OSAL_PRIORITY_BELOW_NORMAL4  = 16 + 4,     /*!< Priority: below normal + 4 */
    XF_OSAL_PRIORITY_BELOW_NORMAL5  = 16 + 5,     /*!< Priority: below normal + 5 */
    XF_OSAL_PRIORITY_BELOW_NORMAL6  = 16 + 6,     /*!< Priority: below normal + 6 */
    XF_OSAL_PRIORITY_BELOW_NORMAL7  = 16 + 7,     /*!< Priority: below normal + 7 */
    XF_OSAL_PRIORITY_NORMOL         = 24,         /*!< Priority: normal */
    XF_OSAL_PRIORITY_NORMOL1        = 24 + 1,     /*!< Priority: normal + 1 */
    XF_OSAL_PRIORITY_NORMOL2        = 24 + 2,     /*!< Priority: normal + 2 */
    XF_OSAL_PRIORITY_NORMOL3        = 24 + 3,     /*!< Priority: normal + 3 */
    XF_OSAL_PRIORITY_NORMOL4        = 24 + 4,     /*!< Priority: normal + 4 */
    XF_OSAL_PRIORITY_NORMOL5        = 24 + 5,     /*!< Priority: normal + 5 */
    XF_OSAL_PRIORITY_NORMOL6        = 24 + 6,     /*!< Priority: normal + 6 */
    XF_OSAL_PRIORITY_NORMOL7        = 24 + 7,     /*!< Priority: normal + 7 */
    XF_OSAL_PRIORITY_ABOVE_NORMAL   = 32,         /*!< Priority: above normal */
    XF_OSAL_PRIORITY_ABOVE_NORMAL1  = 32 + 1,     /*!< Priority: above normal + 1 */
    XF_OSAL_PRIORITY_ABOVE_NORMAL2  = 32 + 2,     /*!< Priority: above normal + 2 */
    XF_OSAL_PRIORITY_ABOVE_NORMAL3  = 32 + 3,     /*!< Priority: above normal + 3 */
    XF_OSAL_PRIORITY_ABOVE_NORMAL4  = 32 + 4,     /*!< Priority: above normal + 4 */
    XF_OSAL_PRIORITY_ABOVE_NORMAL5  = 32 + 5,     /*!< Priority: above normal + 5 */
    XF_OSAL_PRIORITY_ABOVE_NORMAL6  = 32 + 6,     /*!< Priority: above normal + 6 */
    XF_OSAL_PRIORITY_ABOVE_NORMAL7  = 32 + 7,     /*!< Priority: above normal + 7 */
    XF_OSAL_PRIORITY_HIGH           = 40,         /*!< Priority: high */
    XF_OSAL_PRIORITY_HIGH1          = 40 + 1,     /*!< Priority: high + 1 */
    XF_OSAL_PRIORITY_HIGH2          = 40 + 2,     /*!< Priority: high + 2 */
    XF_OSAL_PRIORITY_HIGH3          = 40 + 3,     /*!< Priority: high + 3 */
    XF_OSAL_PRIORITY_HIGH4          = 40 + 4,     /*!< Priority: high + 4 */
    XF_OSAL_PRIORITY_HIGH5          = 40 + 5,     /*!< Priority: high + 5 */
    XF_OSAL_PRIORITY_HIGH6          = 40 + 6,     /*!< Priority: high + 6 */
    XF_OSAL_PRIORITY_HIGH7          = 40 + 7,     /*!< Priority: high + 7 */
    XF_OSAL_PRIORITY_REALTIME       = 48,         /*!< Priority: realtime */
    XF_OSAL_PRIORITY_REALTIME1      = 48 + 1,     /*!< Priority: realtime + 1 */
    XF_OSAL_PRIORITY_REALTIME2      = 48 + 2,     /*!< Priority: realtime + 2 */
    XF_OSAL_PRIORITY_REALTIME3      = 48 + 3,     /*!< Priority: realtime + 3 */
    XF_OSAL_PRIORITY_REALTIME4      = 48 + 4,     /*!< Priority: realtime + 4 */
    XF_OSAL_PRIORITY_REALTIME5      = 48 + 5,     /*!< Priority: realtime + 5 */
    XF_OSAL_PRIORITY_REALTIME6      = 48 + 6,     /*!< Priority: realtime + 6 */
    XF_OSAL_PRIORITY_REALTIME7      = 48 + 7,     /*!< Priority: realtime + 7 */
    XF_OSAL_PRIORITY_ISR            = 56,         /*!< 为 ISR 延迟线程保留 */
    XF_OSAL_PRIORITY_ERROR          = -1,         /*!< 系统无法确定优先级或非法优先级 */
    XF_OSAL_PRIORITY_RESERVED       = 0x7FFFFFFF  /*!< 防止枚举缩小编译器优化 */
} xf_osal_priority_t;

/**
 * @brief 线程的属性结构。
 */
typedef struct _xf_osal_thread_attr_t {
    const char         *name;       /*!< 线程的名称，指向可读字符串。默认值: NULL. */
    uint32_t            attr_bits;  /*!< 属性位，默认值: 0.
                                     *   可用属性：
                                     *   - @ref XF_OSAL_DETACHED.
                                     *   - @ref XF_OSAL_JOINABLE.
                                     *   - @ref XF_OSAL_UNPRIVILEGED.
                                     *   - @ref XF_OSAL_PRIVIEGED.
                                     */
    void               *cb_mem;     /*!< 控制块的内存，默认值: NULL, 即自动动态分配内存。 */
    uint32_t            cb_size;    /*!< 控制块内存大小（单位字节），
                                     *   不使用静态分配时设为默认值: 0.
                                     */
    void               *stack_mem;  /*!< 栈内存（需 64 位对齐）。
                                     *   默认值: NULL, 即自动动态分配内存。
                                     */
    uint32_t            stack_size; /*!< 栈内存大小（单位字节），不使用静态分配时设为默认值: 0. */
    xf_osal_priority_t  priority;   /*!< 线程优先级，默认值: XF_OSAL_PRIORITY_NORMOL. */
    uint32_t            reserved;   /*!< 保留，必须为 0. */
} xf_osal_thread_attr_t;

/**
 * @brief 线程句柄。
 */
typedef void *xf_osal_thread_t;

/**
 * @brief 线程的入口函数。
 */
typedef void (*xf_osal_thread_func_t)(void *argument);

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief 创建一个线程并将其添加到活动线程中。
 *
 * - 函数 xf_osal_thread_create() 通过将线程函数添加到活动线程列表，
 *   并将其设置为状态 READY 来启动线程函数。
 * - 当创建的线程函数的优先级高于当前 RUNNING 线程时，立刻发生一次线程切换，
 *   创建的线程函数立即启动，并成为新的 RUNNING 线程。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param func          线程函数。
 * @param argument      线程函数的参数。
 * @param attr          线程属性。填入 NULL 时使用默认属性。
 * @return xf_osal_thread_t
 *      - NULL                  创建失败
 *      - (OTHER)               线程句柄
 */
xf_osal_thread_t xf_osal_thread_create(
    xf_osal_thread_func_t func, void *argument, const xf_osal_thread_attr_t *attr);

/**
 * @brief 获取线程的名称。
 *
 * @note @b 可以 在中断服务函数中调用。
 *
 * @param thread 线程句柄。
 * @return const char* 线程名称字符串（以 '\0' 结尾）。
 */
const char *xf_osal_thread_get_name(xf_osal_thread_t thread);

/**
 * @brief 返回当前正在运行的线程的线程句柄。
 *
 * @note @b 可以 在中断服务函数中调用。
 *
 * @return xf_osal_thread_t
 *      - NULL                  错误
 *      - (OTHER)               当前线程句柄
 */
xf_osal_thread_t xf_osal_thread_get_current(void);

/**
 * @brief 获取线程的当前线程状态。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param thread 线程句柄。
 * @return xf_osal_state_t 指定线程的当前线程状态，见 @ref xf_osal_state_t.
 */
xf_osal_state_t xf_osal_thread_get_state(xf_osal_thread_t thread);

/**
 * @brief 根据执行过程中的栈记录获取线程的可用堆栈空间。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param thread 线程句柄。
 * @return uint32_t 剩余的堆栈空间（单位字节）。
 */
uint32_t xf_osal_thread_get_stack_space(xf_osal_thread_t thread);

/**
 * @brief 更改线程的优先级。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param thread 线程句柄。
 * @param priority 线程函数的新优先级值。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_RESOURCE       线程处于无效状态
 *      - XF_ERR_ISR            禁止在中断服务函数中调用
 *      - XF_ERR_INVALID_ARG    无效参数
 */
xf_err_t xf_osal_thread_set_priority(
    xf_osal_thread_t thread, xf_osal_priority_t priority);

/**
 * @brief 获取线程的当前优先级。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param thread 线程句柄。
 * @return xf_osal_priority_t 指定线程的优先级。
 *      - (OTHER)                   指定线程的优先级，见 @ref xf_osal_priority_t
 *      - XF_OSAL_PRIORITY_ERROR    无法确定优先级或优先级非法，或在中断中调用该函数
 */
xf_osal_priority_t xf_osal_thread_get_priority(xf_osal_thread_t thread);

/**
 * @brief 将控制权传递给处于状态 @b READY 的下一个线程。
 *
 * - xf_osal_thread_yield() 不会将线程设置为状态 BLOCKED。
 *   因此，即使处于 READY 状态的线程可用，也不会调度优先级较低的线程。
 *
 * @note @b 禁止 在中断服务函数中调用。
 * @note 当内核锁定时此函数无作用。
 *
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_ISR            禁止在中断服务函数中调用
 */
xf_err_t xf_osal_thread_yield(void);

/**
 * @brief 暂停指定线程的执行，使其进入 @b BLOCKED 状态。
 *
 * - 暂停正在运行的线程将导致上下文立即切换到另一个处于 READY 状态的线程。
 * - 在使用函数 xf_osal_thread_resume() 显式恢复之前，不会执行挂起的线程。
 *
 * @note @b 禁止 在中断服务函数中调用。
 * @note 当内核被锁定时，不得调用此函数来挂起正在运行的线程。
 *
 * @param thread 线程句柄。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_RESOURCE       线程处于无效状态
 *      - XF_ERR_ISR            禁止在中断服务函数中调用
 *      - XF_ERR_INVALID_ARG    无效参数
 */
xf_err_t xf_osal_thread_suspend(xf_osal_thread_t thread);

/**
 * @brief 恢复线程的执行。
 *
 * - 被恢复的线程必须处于 @b BLOCKED 状态。
 * - 如果恢复的线程的优先级高于正在运行的线程，则会立即进行上下文切换。
 * - @b 不建议 恢复 @b 不是 被 xf_osal_thread_suspend() 暂停的线程。 @n
 *     以下函数均可能使线程阻塞：
 *     - xf_osal_event_wait()
 *     - xf_osal_thread_notify_wait()
 *     - xf_osal_delay(), xf_osal_delay_until(), xf_osal_delay_ms()
 *     - xf_osal_mutex_acquire()
 *     - xf_osal_semaphore_acquire()
 *     - xf_osal_queue_get()
 *
 * @note @b 禁止 在中断服务函数中调用。
 * @note 当内核被锁定时， @b 可以 调用此函数。
 *       此时潜在的上下文切换会被延迟，直到内核解锁。
 *
 * @param thread 线程句柄。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_RESOURCE       线程处于无效状态
 *      - XF_ERR_ISR            禁止在中断服务函数中调用
 *      - XF_ERR_INVALID_ARG    无效参数
 */
xf_err_t xf_osal_thread_resume(xf_osal_thread_t thread);

/**
 * @brief 终止线程的执行。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param thread 线程句柄。
 *      允许为 NULL, 等价于 `xf_osal_thread_delete(xf_osal_thread_get_current())`。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_RESOURCE       线程处于无效状态或不存在其他 READY 线程
 *      - XF_ERR_ISR            禁止在中断服务函数中调用
 *      - XF_ERR_INVALID_ARG    无效参数
 */
xf_err_t xf_osal_thread_delete(xf_osal_thread_t thread);

/**
 * @brief 获取活动线程的数量。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @return uint32_t
 *      - 0                     错误，可能在中断服务函数中调用
 *      - (OTHER)               活动线程的数量
 */
uint32_t xf_osal_thread_get_count(void);

/**
 * @brief 列出活动线程。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param[out] thread_array 指向用于检索线程句柄的数组的指针。
 * @param array_items       用于检索线程句柄的数组中的最大项目数。
 * @return uint32_t
 *      - 0                     错误，可能在中断服务函数中调用，或接口未实现，
 *                              或参数错误
 *      - (OTHER)               已枚举线程的数量
 */
uint32_t xf_osal_thread_enumerate(
    xf_osal_thread_t *thread_array, uint32_t array_items);

/**
 * @brief 设置线程的指定线程标志。
 *
 * @note @b 可以 在中断服务函数中调用。
 *
 * @param thread    线程句柄。
 * @param notify    指定应设置的线程标志。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_TIMEOUT        超时
 *      - XF_ERR_RESOURCE       资源不可用
 *      - XF_ERR_INVALID_ARG    无效参数，参数标志设置了最高位
 *      - XF_ERR_ISR            中断调用错误，禁止在中断中调用
 */
xf_err_t xf_osal_thread_notify_set(xf_osal_thread_t thread, uint32_t notify);

/**
 * @brief 清除当前运行线程的指定线程标志。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param notify 指定应清除的线程标志。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_INVALID_ARG    无效参数，参数标志设置了最高位
 *      - XF_ERR_ISR            中断调用错误，禁止在中断中调用
 */
xf_err_t xf_osal_thread_notify_clear(uint32_t notify);

/**
 * @brief 获取当前正在运行的线程的当前线程标志。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @return uint32_t 当前线程标志。
 */
uint32_t xf_osal_thread_notify_get(void);

/**
 * @brief 当前运行线程等待一个或多个线程标志变为有信号状态。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param notify    指定要等待的标志。
 * @param options   指定标志选项。
 *      - 见 @ref XF_OSAL_WAIT_ANY.
 *      - 见 @ref XF_OSAL_WAIT_ALL.
 *      - 见 @ref XF_OSAL_NO_CLEAR.
 * @param timeout 超时值，单位 ticks.
 *                可以为 0 或 @ref XF_OSAL_WAIT_FOREVER.
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_TIMEOUT        超时
 *      - XF_ERR_RESOURCE       在未指定超时的情况下，未设置等待标记
 *      - XF_ERR_INVALID_ARG    无效参数，句柄无效或 flags 设置了最高位
 */
xf_err_t xf_osal_thread_notify_wait(
    uint32_t notify, uint32_t options, uint32_t timeout);

/**
 * @brief （睡眠）等待超时。
 *
 * xf_osal_delay() 从调用时刻起睡眠等待指定时间刻度。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param ticks 需要等待的时间刻度值。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_INVALID_ARG    无效参数
 *      - XF_ERR_ISR            禁止在中断服务函数中调用
 */
xf_err_t xf_osal_delay(uint32_t ticks);

/**
 * @brief （睡眠）等到指定时间刻度。
 *
 * xf_osal_delay_until() 会等待直到绝对时间刻度。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param ticks 等待到指定的时间刻度值。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_INVALID_ARG    无效参数
 *      - XF_ERR_ISR            禁止在中断服务函数中调用
 */
xf_err_t xf_osal_delay_until(uint32_t ticks);

/**
 * @brief （睡眠）等待超时，以 ms 为单位。
 *
 * xf_osal_delay_ms() 从调用时刻起睡眠等待指定 ms 数。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @param ms 需要等待的 ms 数。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_INVALID_ARG    无效参数
 *      - XF_ERR_ISR            禁止在中断服务函数中调用
 */
xf_err_t xf_osal_delay_ms(uint32_t ms);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_OSAL_THREAD_H__

#endif // XF_OSAL_THREAD_IS_ENABLE
