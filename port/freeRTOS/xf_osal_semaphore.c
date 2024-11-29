/**
 * @file xf_osal_semaphore.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-09-10
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_osal_internal.h"

#if XF_OSAL_SEMAPHORE_IS_ENABLE

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

xf_osal_semaphore_t xf_osal_semaphore_create(uint32_t max_count, uint32_t initial_count,
        const xf_osal_semaphore_attr_t *attr)
{
    SemaphoreHandle_t hSemaphore;
    int32_t mem;

    hSemaphore = NULL;

    if ((IRQ_Context() == 0U) && (max_count > 0U) && (initial_count <= max_count)) {
        mem = -1;

        if (attr != NULL) {
            if ((attr->cb_mem != NULL) && (attr->cb_size >= sizeof(StaticSemaphore_t))) {
                /* The memory for control block is provided, use static object */
                mem = 1;
            } else {
                if ((attr->cb_mem == NULL) && (attr->cb_size == 0U)) {
                    /* Control block will be allocated from the dynamic pool */
                    mem = 0;
                }
            }
        } else {
            mem = 0;
        }

        if (mem != -1) {
            if (max_count == 1U) {
                if (mem == 1) {
#if (configSUPPORT_STATIC_ALLOCATION == 1)
                    hSemaphore = xSemaphoreCreateBinaryStatic((StaticSemaphore_t *)attr->cb_mem);
#endif
                } else {
#if (configSUPPORT_DYNAMIC_ALLOCATION == 1)
                    hSemaphore = xSemaphoreCreateBinary();
#endif
                }

                if ((hSemaphore != NULL) && (initial_count != 0U)) {
                    if (xSemaphoreGive(hSemaphore) != pdPASS) {
                        vSemaphoreDelete(hSemaphore);
                        hSemaphore = NULL;
                    }
                }
            } else {
                if (mem == 1) {
#if (configSUPPORT_STATIC_ALLOCATION == 1)
                    hSemaphore = xSemaphoreCreateCountingStatic(max_count, initial_count, (StaticSemaphore_t *)attr->cb_mem);
#endif
                } else {
#if (configSUPPORT_DYNAMIC_ALLOCATION == 1)
                    hSemaphore = xSemaphoreCreateCounting(max_count, initial_count);
#endif
                }
            }

#if (configQUEUE_REGISTRY_SIZE > 0)
            if (hSemaphore != NULL) {
                if ((attr != NULL) && (attr->name != NULL)) {
                    /* Only non-NULL name objects are added to the Queue Registry */
                    vQueueAddToRegistry(hSemaphore, attr->name);
                }
            }
#endif
        }
    }

    /* Return semaphore ID */
    return ((xf_osal_semaphore_t)hSemaphore);
}

xf_err_t xf_osal_semaphore_acquire(xf_osal_semaphore_t semaphore, uint32_t timeout)
{
    SemaphoreHandle_t hSemaphore = (SemaphoreHandle_t)semaphore;
    xf_err_t stat;
    BaseType_t yield;

    stat = XF_OK;

    if (hSemaphore == NULL) {
        stat = XF_ERR_INVALID_ARG;
    } else if (IRQ_Context() != 0U) {
        if (timeout != 0U) {
            stat = XF_ERR_INVALID_ARG;
        } else {
            yield = pdFALSE;

            if (xSemaphoreTakeFromISR(hSemaphore, &yield) != pdPASS) {
                stat = XF_ERR_RESOURCE;
            } else {
                portYIELD_FROM_ISR(yield);
            }
        }
    } else {
        if (xSemaphoreTake(hSemaphore, (TickType_t)timeout) != pdPASS) {
            if (timeout != 0U) {
                stat = XF_ERR_TIMEOUT;
            } else {
                stat = XF_ERR_RESOURCE;
            }
        }
    }

    /* Return execution status */
    return (stat);
}

xf_err_t xf_osal_semaphore_release(xf_osal_semaphore_t semaphore)
{
    SemaphoreHandle_t hSemaphore = (SemaphoreHandle_t)semaphore;
    xf_err_t stat;
    BaseType_t yield;

    stat = XF_OK;

    if (hSemaphore == NULL) {
        stat = XF_ERR_INVALID_ARG;
    } else if (IRQ_Context() != 0U) {
        yield = pdFALSE;

        if (xSemaphoreGiveFromISR(hSemaphore, &yield) != pdTRUE) {
            stat = XF_ERR_RESOURCE;
        } else {
            portYIELD_FROM_ISR(yield);
        }
    } else {
        if (xSemaphoreGive(hSemaphore) != pdPASS) {
            stat = XF_ERR_RESOURCE;
        }
    }

    /* Return execution status */
    return (stat);
}

uint32_t xf_osal_semaphore_get_count(xf_osal_semaphore_t semaphore)
{
    SemaphoreHandle_t hSemaphore = (SemaphoreHandle_t)semaphore;
    uint32_t count;

    if (hSemaphore == NULL) {
        count = 0U;
    } else if (IRQ_Context() != 0U) {
        count = 0U;
    } else {
        count = (uint32_t)uxSemaphoreGetCount(hSemaphore);
    }

    /* Return number of tokens */
    return (count);
}

xf_err_t xf_osal_semaphore_delete(xf_osal_semaphore_t semaphore)
{
    SemaphoreHandle_t hSemaphore = (SemaphoreHandle_t)semaphore;
    xf_err_t stat;

#ifndef USE_FreeRTOS_HEAP_1
    if (IRQ_Context() != 0U) {
        stat = XF_ERR_ISR;
    } else if (hSemaphore == NULL) {
        stat = XF_ERR_INVALID_ARG;
    } else {
#if (configQUEUE_REGISTRY_SIZE > 0)
        vQueueUnregisterQueue(hSemaphore);
#endif

        stat = XF_OK;
        vSemaphoreDelete(hSemaphore);
    }
#else
    stat = XF_FAIL;
#endif

    /* Return execution status */
    return (stat);
}

/* ==================== [Static Functions] ================================== */

#endif
