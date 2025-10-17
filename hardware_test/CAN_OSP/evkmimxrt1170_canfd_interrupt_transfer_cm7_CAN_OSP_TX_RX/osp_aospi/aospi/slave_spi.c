#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "slave_spi.h"
#include "fsl_gpio.h"
#include "fsl_lpspi.h"
#include "fsl_flexcan.h"
#include "fsl_lpuart.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)

/* module-local queues (kept private to .c) */
static QueueHandle_t s_trans_queue_handle = NULL;
static QueueHandle_t s_trans_result_handle = NULL;
static QueueHandle_t s_trans_error_handle = NULL;
static QueueHandle_t s_in_flight_mailbox_handle = NULL;
static volatile bool isTransferCompleted = false;

/* helper: map spi_bus number to spi_host_device_t (basic mapping; you can adapt) */
//static LPSPI_Type hostFromBusNumber(uint8_t spi_bus)
//{
//    /* Common mapping used in Arduino-ESP32 wrappers:
//       FSPI -> SPI1_HOST, HSPI -> SPI2_HOST, VSPI -> SPI3_HOST.
//       If platform differs, adapt this function. */
//#if defined(SPI1_HOST)
//    if (spi_bus == 1) return LPSPI1;
//#endif
//#if defined(SPI2_HOST)
//    if (spi_bus == 2) return LPSPI2;
//#endif
//#if defined(SPI3_HOST)
//    if (spi_bus == 3) return LPSPI3;
//#endif
//#if defined(SPI1_HOST)
//    if (spi_bus == 4) return LPSPI4;
//#endif
//#if defined(SPI1_HOST)
//    if (spi_bus == 5) return LPSPI5;
//#endif
//#if defined(SPI1_HOST)
//    if (spi_bus == 6) return LPSPI6;
//#else
//    return LPSPI4;
//#endif
//}

///* Internal ISR callbacks that call user callbacks stored in transaction->user */
//static void IRAM_ATTR spi_slave_post_setup_cb_internal(spi_slave_transaction_t* trans)
//{
//    if (!trans || !trans->user) return;
//    spi_slave_cb_user_context_t *user_ctx = (spi_slave_cb_user_context_t*)trans->user;
//    if (user_ctx->post_setup.user_cb) {
//        user_ctx->post_setup.user_cb(trans, user_ctx->post_setup.user_arg);
//    }
//}
//
//static void IRAM_ATTR spi_slave_post_trans_cb_internal(spi_slave_transaction_t* trans)
//{
//    if (!trans || !trans->user) return;
//    spi_slave_cb_user_context_t *user_ctx = (spi_slave_cb_user_context_t*)trans->user;
//    if (user_ctx->post_trans.user_cb) {
//        user_ctx->post_trans.user_cb(trans, user_ctx->post_trans.user_arg);
//    }
//}

void LPSPI_SlaveUserCallback(LPSPI_Type *base, lpspi_slave_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        PRINTF("This is LPSPI slave transfer completed callback. \r\n");
        PRINTF("It's a successful transfer. \r\n\r\n");
    }

    if (status == kStatus_LPSPI_Error)
    {
        PRINTF("This is LPSPI slave transfer completed callback. \r\n");
        PRINTF("Error occurred in this transfer. \r\n\r\n");
        vTaskDelete(NULL);
    }

    isTransferCompleted = true;
}

#define EXAMPLE_LPSPI_SLAVE_BASEADDR LPSPI4
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     (kLPSPI_Pcs0)
static lpspi_slave_handle_t g_s_handle;

/* ===== spi task ===== */
void spi_slave_task(void* arg)
{
	lpspi_slave_config_t slaveConfig;
	lpspi_slave_handle_t g_s_handle;
    ESP32SPISlave_C* dev = (ESP32SPISlave_C*)arg;
    spi_slave_context_t *ctx = &dev->ctx;
    status_t r;


    LPSPI_SlaveGetDefaultConfig(&slaveConfig);
	slaveConfig.whichPcs = EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT;
	slaveConfig.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;
	slaveConfig.pinCfg = kLPSPI_SdoInSdoOut;

	LPSPI_SlaveInit(EXAMPLE_LPSPI_SLAVE_BASEADDR, &slaveConfig);

	LPSPI_SlaveTransferCreateHandle(EXAMPLE_LPSPI_SLAVE_BASEADDR, &g_s_handle, LPSPI_SlaveUserCallback, NULL);

//    esp_err_t err = spi_slave_initialize(ctx->host, &ctx->bus_cfg, &ctx->if_cfg, SPI_DMA_DISABLED);
//    if (err != ESP_OK) {
//    	PRINTF("spi_slave_initialize failed: 0x%X", err);
//        vTaskDelete(NULL);
//        return;
//    }

    /* create internal queues */
    s_trans_queue_handle = xQueueCreate(1, sizeof(spi_transaction_context_t));
    s_trans_result_handle = xQueueCreate(1, sizeof(size_t));
    s_trans_error_handle = xQueueCreate(1, sizeof(status_t));
    s_in_flight_mailbox_handle = xQueueCreate(1, sizeof(size_t));

    if (!s_trans_queue_handle || !s_trans_result_handle || !s_trans_error_handle || !s_in_flight_mailbox_handle) {
    	PRINTF("failed to create internal queues");
        if (s_trans_queue_handle) vQueueDelete(s_trans_queue_handle);
        if (s_trans_result_handle) vQueueDelete(s_trans_result_handle);
        if (s_trans_error_handle) vQueueDelete(s_trans_error_handle);
        if (s_in_flight_mailbox_handle) vQueueDelete(s_in_flight_mailbox_handle);
//        spi_slave_free(ctx->host);
        vTaskDelete(NULL);
        return;
    }

    PRINTF("spi_slave_task ready");

    for (;;) {
        spi_transaction_context_t trans_ctx;
        if (xQueueReceive(s_trans_queue_handle, &trans_ctx, RECV_TRANS_QUEUE_TIMEOUT_TICKS) == pdTRUE) {
            if (!trans_ctx.trans || trans_ctx.size == 0) {
                continue;
            }

            /* update in-flight count mailbox */
            xQueueOverwrite(s_in_flight_mailbox_handle, &trans_ctx.size);

            /* queue the transactions to driver */
            status_t *errs = (status_t*)malloc(sizeof(status_t) * trans_ctx.size);
            if (!errs) {
            	PRINTF("malloc failed for errs");
                free(trans_ctx.trans); /* free caller-allocated array to avoid leak */
                continue;
            }

            for (size_t i = 0; i < trans_ctx.size; ++i) {
//                errs[i] = spi_slave_queue_trans(ctx->host, &trans_ctx.trans[i], trans_ctx.timeout_ticks);
                if (errs[i] != kStatus_Success) {
                	PRINTF("spi_slave_queue_trans failed: 0x%X (idx=%u)", errs[i], (unsigned)i);
                }
            }

            /* wait for completion of queued transactions and forward results/errors */
            for (size_t i = 0; i < trans_ctx.size; ++i) {
                size_t num_received_bytes = 0;
                if (errs[i] == kStatus_Success) {
                	lpspi_transfer_t* rtrans = NULL;
//                	r = spi_slave_get_trans_result(ctx->host, &rtrans, trans_ctx.timeout_ticks);
                    if (r == kStatus_Success && rtrans) {
//                        num_received_bytes = (rtrans->trans_len ? (rtrans->trans_len / 8) : (rtrans->length / 8));
                        PRINTF("transaction complete: %u bytes received", (unsigned)rtrans->dataSize);
                    } else {
                    	PRINTF("spi_slave_get_trans_result failed: 0x%X", r);
                    }
                } else {
                	PRINTF("skipped spi_slave_get_trans_result because queueing failed (idx=%u)", (unsigned)i);
                }

                if (xQueueSend(s_trans_result_handle, &num_received_bytes, SEND_TRANS_RESULT_TIMEOUT_TICKS) != pdTRUE) {
                	PRINTF("failed to send result to main task");
                }
                if (xQueueSend(s_trans_error_handle, &errs[i], SEND_TRANS_ERROR_TIMEOUT_TICKS) != pdTRUE) {
                	PRINTF("failed to send error to main task");
                }

                size_t rest = trans_ctx.size - (i + 1);
                xQueueOverwrite(s_in_flight_mailbox_handle, &rest);
            }

            free(errs);
            /* free transaction array (ownership transferred from caller) */
            free(trans_ctx.trans);
        }

        /* terminate task if requested */
        if (xTaskNotifyWait(0, 0, NULL, 0) == pdTRUE) {
            break;
        }
    }

    /* cleanup */
    vQueueDelete(s_in_flight_mailbox_handle);
    vQueueDelete(s_trans_result_handle);
    vQueueDelete(s_trans_error_handle);
    vQueueDelete(s_trans_queue_handle);

//    spi_slave_free(ctx->host);

    /* notify main task that spi task finished */
    xTaskNotifyGive(ctx->main_task_handle);

    vTaskDelete(NULL);
}

/* ===== Public API implementation ===== */

bool esp32_spi_slave_begin(ESP32SPISlave_C* dev, uint8_t spi_bus)
{
    if (!dev) return false;
    memset(dev, 0, sizeof(*dev));

//    /* defaults */
//    dev->ctx.if_cfg.spics_io_num = -1;
//    dev->ctx.if_cfg.flags = 0;
//    dev->ctx.if_cfg.queue_size = 1;
//    dev->ctx.if_cfg.mode = SPI_MODE0;
//    dev->ctx.if_cfg.post_setup_cb = spi_slave_post_setup_cb_internal;
//    dev->ctx.if_cfg.post_trans_cb = spi_slave_post_trans_cb_internal;
//
//    /* bus defaults */
//    dev->ctx.bus_cfg.mosi_io_num = -1;
//    dev->ctx.bus_cfg.miso_io_num = -1;
//    dev->ctx.bus_cfg.sclk_io_num = -1;
//    dev->ctx.bus_cfg.max_transfer_sz = 4096; /* fallback, adapt if you need larger */
//    dev->ctx.bus_cfg.flags = SPICOMMON_BUSFLAG_SLAVE;
//
//    dev->ctx.host = hostFromBusNumber(spi_bus);
    dev->ctx.main_task_handle = xTaskGetCurrentTaskHandle();
//    dev->queue_capacity = dev->ctx.if_cfg.queue_size;

    BaseType_t ret = xTaskCreate(
            spi_slave_task,
            "spi_slave_task",
            SPI_SLAVE_TASK_STACK_SIZE, // Kích thước Stack
            dev,                       // Tham số truyền vào Task
            SPI_SLAVE_TASK_PRIORITY,   // Độ ưu tiên
            &dev->spi_task_handle      // Con trỏ nhận Task Handle
    );

    if (ret != pdPASS) {
    	PRINTF("failed to create spi_slave_task: %d", (int)ret);
        return false;
    }

    /* wait briefly until spi task created internal queues (avoid race) */
    int wait_ms = 200; /* wait up to 200ms */
    while (!s_trans_queue_handle && wait_ms > 0) {
        vTaskDelay(pdMS_TO_TICKS(10));
        wait_ms -= 10;
    }
    if (!s_trans_queue_handle) {
        PRINTF("spi task did not initialize internal queues in time");
        /* still return true because task exists; user may retry later */
    }

    return true;
}

bool esp32_spi_slave_begin_pins(ESP32SPISlave_C* dev, uint8_t spi_bus, int sck, int miso, int mosi, int ss)
{
    if (!dev) return false;
    /* set pins first */
//    dev->ctx.if_cfg.spics_io_num = ss;
//    dev->ctx.bus_cfg.sclk_io_num = sck;
//    dev->ctx.bus_cfg.mosi_io_num = mosi;
//    dev->ctx.bus_cfg.miso_io_num = miso;
    return esp32_spi_slave_begin(dev, spi_bus);
}

void esp32_spi_slave_end(ESP32SPISlave_C* dev)
{
    if (!dev || !dev->spi_task_handle) return;
    xTaskNotifyGive(dev->spi_task_handle);
    if (xTaskNotifyWait(0, 0, NULL, pdMS_TO_TICKS(5000)) != pdTRUE) {
    	PRINTF("timeout waiting for spi_slave_task termination");
    }
    dev->spi_task_handle = NULL;
}

size_t esp32_spi_slave_transfer(ESP32SPISlave_C* dev,
                                const uint8_t* tx_buf,
                                uint8_t* rx_buf,
                                size_t size,
                                uint32_t timeout_ms)
{
    if (!dev) return 0;
    if (size % 4 != 0) {
    	PRINTF("buffer size must be multiple of 4");
        return 0;
    }
    if (!s_trans_queue_handle) {
    	PRINTF("spi task not ready (no trans queue)");
        return 0;
    }

    lpspi_transfer_t* t = (lpspi_transfer_t*)calloc(1, sizeof(lpspi_transfer_t));
    if (!t) return 0;

//    t->length = (int)(size * 8);
//    t->trans_len = 0;
//    t->tx_buffer = (const void*)tx_buf;
//    t->rx_buffer = (void*)rx_buf;
//    t->user = &dev->cb_user_ctx;

    t->dataSize = (size * 8);
    t->rxData = rx_buf;
    t->txData = tx_buf;
    t->configFlags = EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER | kLPSPI_SlaveByteSwap;

    spi_transaction_context_t ctx = {
        .trans = t,
        .size = 1,
        .timeout_ticks = (timeout_ms == 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms),
    };

    if (xQueueSend(s_trans_queue_handle, &ctx, SEND_TRANS_QUEUE_TIMEOUT_TICKS) != pdTRUE) {
    	PRINTF("failed to queue transaction to spi task");
        free(t);
        return 0;
    }

    /* wait for result */
    size_t received = 0;
    TickType_t wait_ticks = (timeout_ms == 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    if (xQueueReceive(s_trans_result_handle, &received, wait_ticks) != pdTRUE) {
    	PRINTF("timeout waiting for transaction result");
        return 0;
    }
    return received;
}

bool esp32_spi_slave_queue(ESP32SPISlave_C* dev,
                           const uint8_t* tx_buf,
                           uint8_t* rx_buf,
                           size_t size)
{
    if (!dev) return false;
    if (size % 4 != 0) {
    	PRINTF("buffer size must be multiple of 4");
        return false;
    }

    if (dev->num_transactions >= dev->queue_capacity) {
    	PRINTF("queue is full (capacity=%u)", (unsigned)dev->queue_capacity);
        return false;
    }

    /* append a transaction object to dev->transactions array */
//    spi_slave_transaction_t tmp;
    lpspi_transfer_t slaveXfer;
    memset(&slaveXfer, 0, sizeof(slaveXfer));
//    tmp.length = (int)(size * 8);
//    tmp.trans_len = 0;
//    tmp.tx_buffer = (const void*)tx_buf;
//    tmp.rx_buffer = (void*)rx_buf;
//    tmp.user = &dev->cb_user_ctx;

    slaveXfer.dataSize = (int)(size * 8);
    slaveXfer.txData = tx_buf;
    slaveXfer.rxData = rx_buf;
    slaveXfer.configFlags = EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER | kLPSPI_SlaveByteSwap;

    lpspi_transfer_t* newptr = (lpspi_transfer_t*)realloc(dev->transactions, (dev->num_transactions + 1) * sizeof(lpspi_transfer_t));
    if (!newptr) {
    	PRINTF("realloc failed");
        return false;
    }
    dev->transactions = newptr;
    dev->transactions[dev->num_transactions++] = slaveXfer;
    return true;
}

bool esp32_spi_slave_trigger(ESP32SPISlave_C* dev, uint32_t timeout_ms)
{
    if (!dev) return false;
    if (dev->num_transactions == 0) return false;
    if (!s_trans_queue_handle) {
        PRINTF("spi task not ready (no trans queue)");
        return false;
    }

    lpspi_transfer_t* arr = (lpspi_transfer_t*)malloc(dev->num_transactions * sizeof(lpspi_transfer_t));
    if (!arr) {
    	PRINTF("malloc failed");
        return false;
    }
    memcpy(arr, dev->transactions, dev->num_transactions * sizeof(lpspi_transfer_t));

    spi_transaction_context_t ctx = {
        .trans = arr,
        .size = dev->num_transactions,
        .timeout_ticks = (timeout_ms == 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms),
    };

    BaseType_t ret = xQueueSend(s_trans_queue_handle, &ctx, SEND_TRANS_QUEUE_TIMEOUT_TICKS);
    free(dev->transactions);
    dev->transactions = NULL;
    dev->num_transactions = 0;

    if (ret != pdTRUE) {
        free(arr);
        PRINTF("failed to send transactions to spi task");
        return false;
    }
    return true;
}

/* status helpers use module-global queues */
size_t esp32_spi_slave_num_in_flight(void)
{
    size_t num = 0;
    if (s_in_flight_mailbox_handle) {
        xQueuePeek(s_in_flight_mailbox_handle, &num, 0);
    }
    return num;
}

size_t esp32_spi_slave_num_completed(void)
{
    if (!s_trans_result_handle) return 0;
    return (size_t)uxQueueMessagesWaiting(s_trans_result_handle);
}

size_t esp32_spi_slave_num_errors(void)
{
    if (!s_trans_error_handle) return 0;
    return (size_t)uxQueueMessagesWaiting(s_trans_error_handle);
}

size_t esp32_spi_slave_num_bytes_received(void)
{
    size_t val = 0;
    if (s_trans_result_handle) {
        if (xQueueReceive(s_trans_result_handle, &val, RECV_TRANS_RESULT_TIMEOUT_TICKS) == pdTRUE) {
            return val;
        }
    }
    return 0;
}

//esp_err_t esp32_spi_slave_error(void)
//{
//    esp_err_t e = ESP_OK;
//    if (s_trans_error_handle) {
//        if (xQueueReceive(s_trans_error_handle, &e, RECV_TRANS_ERROR_TIMEOUT_TICKS) == pdTRUE) {
//            return e;
//        }
//    }
//    return ESP_OK;
//}

/* set user callbacks (will be called from ISR contexts) */
void esp32_spi_slave_set_user_post_setup_cb(ESP32SPISlave_C* dev, spi_slave_user_cb_t cb, void* arg)
{
    if (!dev) return;
    dev->cb_user_ctx.post_setup.user_cb = cb;
    dev->cb_user_ctx.post_setup.user_arg = arg;
}

void esp32_spi_slave_set_user_post_trans_cb(ESP32SPISlave_C* dev, spi_slave_user_cb_t cb, void* arg)
{
    if (!dev) return;
    dev->cb_user_ctx.post_trans.user_cb = cb;
    dev->cb_user_ctx.post_trans.user_arg = arg;
}

