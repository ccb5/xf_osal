/**
 * @file xf_osal_kernel.h
 * @author cangyu(sky.kirto@qq.com)
 * @brief 调度器内核控制。
 * @version 0.1
 * @date 2023-12-06
 *
 * @copyright Copyright(c) 2023, CorAL. All rights reserved.
 *
 */

#if XF_OSAL_KERNEL_IS_ENABLE || defined(__DOXYGEN__)

#ifndef __XF_OSAL_KERNEL_H__
#define __XF_OSAL_KERNEL_H__

/* ==================== [Includes] ========================================== */

#include "xf_osal_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief api 与内核版本信息。
 */
typedef struct _xf_osal_version_t {
    uint32_t                api;        /*!< API 版本(major.minor.rev: mmnnnrrrr dec). */
    uint32_t                kernel;     /*!< 内核版本(major.minor.rev: mmnnnrrrr dec). */
} xf_osal_version_t;

/**
 * @brief 线程状态或内核状态。
 */
typedef enum _xf_osal_state_t {
    XF_OSAL_INACTIVE        =  0,         /*!< 不活动. */
    XF_OSAL_READY           =  1,         /*!< 就绪. */
    XF_OSAL_RUNNING         =  2,         /*!< 运行. */
    XF_OSAL_BLOCKED         =  3,         /*!< 线程阻塞或内核锁定. */
    XF_OSAL_TERMINATED      =  4,         /*!< 线程终止或内核终止. */
    XF_OSAL_ERROR           = -1,         /*!< 错误. */
    XF_OSAL_RESERVED        = 0x7FFFFFFF, /*!< 防止枚举缩小编译器优化. */
} xf_osal_state_t;

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief 获取 RTOS 内核信息。
 *
 * @note @b 可以 在中断服务函数中调用。
 *
 * @param version 指向用于检索版本信息的缓冲区的指针。
 * @param id_buf 指向用于检索内核标识字符串的缓冲区的指针。
 * @param id_size 内核标识字符串缓冲区的大小。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 */
xf_err_t xf_osal_kernel_get_info(
    xf_osal_version_t *version, char *id_buf, uint32_t id_size);

/**
 * @brief 获取当前 RTOS 内核状态。
 *
 * @note @b 可以 在中断服务函数中调用。
 *
 * @return xf_osal_state_t 当前 RTOS 内核状态，见 @ref xf_osal_state_t.
 */
xf_osal_state_t xf_osal_kernel_get_state(void);

/**
 * @brief 锁定 RTOS 内核调度程序。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_ISR            禁止在中断服务函数中调用
 */
xf_err_t xf_osal_kernel_lock(void);

/**
 * @brief 解锁 RTOS 内核调度程序。
 *
 * @note @b 禁止 在中断服务函数中调用。
 *
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               通用错误
 *      - XF_ERR_ISR            禁止在中断服务函数中调用
 */
xf_err_t xf_osal_kernel_unlock(void);

/**
 * @addtogroup group_xf_osal
 * @todo - 添加挂起与恢复 kernel 相关 API，以供低功耗设备使用无滴答操作。
 */

/**
 * @brief 获取 RTOS 内核滴答计数。
 *
 * @note @b 可以 在中断服务函数中调用。
 *
 * @return uint32_t RTOS 内核当前滴答计数。
 */
uint32_t xf_osal_kernel_get_tick_count(void);

/**
 * @brief 获取 RTOS 内核滴答频率。
 *
 * @note @b 可以 在中断服务函数中调用。
 *
 * @return uint32_t 内核滴答频率（以赫兹为单位），即每秒内核滴答数。
 */
uint32_t xf_osal_kernel_get_tick_freq(void);

/**
 * @brief 将滴答数转为 ms.
 *
 * @note @b 可以 在中断服务函数中调用。
 *
 * @param ticks 要转换的滴答数。
 * @return uint32_t 滴答计数对应的 ms 数。
 */
uint32_t xf_osal_kernel_ticks_to_ms(uint32_t ticks);

/**
 * @brief 将 ms 数转为滴答数.
 *
 * @note @b 可以 在中断服务函数中调用。
 *
 * @param ms 要转换的 ms 数。
 * @return uint32_t ms 数对应的滴答数。
 */
uint32_t xf_osal_kernel_ms_to_ticks(uint32_t ms);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_OSAL_KERNEL_H__

#endif // XF_OSAL_KERNEL_IS_ENABLE
