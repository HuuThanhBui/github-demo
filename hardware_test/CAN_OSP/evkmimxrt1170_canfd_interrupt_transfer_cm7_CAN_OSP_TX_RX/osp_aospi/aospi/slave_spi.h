#ifndef _SLAVE_SPI_H_
#define _SLAVE_SPI_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "fsl_gpio.h"
#include "fsl_lpspi.h"
#include "fsl_flexcan.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Task / priority */
#define SPI_SLAVE_TASK_STACK_SIZE   (1024 * 2)
#define SPI_SLAVE_TASK_PRIORITY     5

/* Timeouts */
#define SEND_TRANS_QUEUE_TIMEOUT_TICKS   pdMS_TO_TICKS(5000)
#define RECV_TRANS_QUEUE_TIMEOUT_TICKS   pdMS_TO_TICKS(5000)
#define SEND_TRANS_RESULT_TIMEOUT_TICKS  pdMS_TO_TICKS(5000)
#define RECV_TRANS_RESULT_TIMEOUT_TICKS  0
#define SEND_TRANS_ERROR_TIMEOUT_TICKS   pdMS_TO_TICKS(5000)
#define RECV_TRANS_ERROR_TIMEOUT_TICKS   0

/* User callback type (C version) */
typedef void (*spi_slave_user_cb_t)(lpspi_transfer_t* trans, void* user_arg);

/* Internal context structures (mirrors original design) */
typedef struct {
//    spi_slave_interface_config_t if_cfg;
//    spi_bus_config_t bus_cfg;
//    spi_host_device_t host;
    TaskHandle_t main_task_handle;
} spi_slave_context_t;

typedef struct {
	lpspi_transfer_t* trans; /* pointer to an array allocated by caller (freed by spi task) */
    size_t size;
    TickType_t timeout_ticks;
} spi_transaction_context_t;

typedef struct {
    struct {
        spi_slave_user_cb_t user_cb;
        void* user_arg;
    } post_setup;
    struct {
        spi_slave_user_cb_t user_cb;
        void* user_arg;
    } post_trans;
} spi_slave_cb_user_context_t;

/* Public C "object" */
typedef struct {
    spi_slave_context_t ctx;
    spi_slave_cb_user_context_t cb_user_ctx;
    TaskHandle_t spi_task_handle;
    /* transaction buffer for queueing multiple transactions before trigger() */
    lpspi_transfer_t* transactions;
    size_t num_transactions;
    size_t queue_capacity;
} ESP32SPISlave_C;

/* === Public API === */

/* initialize with default pin mapping for a given spi_bus number (implementation maps to a host) */
bool esp32_spi_slave_begin(ESP32SPISlave_C* dev, uint8_t spi_bus);

/* initialize specifying pins (sck, miso, mosi, ss) */
bool esp32_spi_slave_begin_pins(ESP32SPISlave_C* dev, uint8_t spi_bus, int sck, int miso, int mosi, int ss);

/* stop spi slave and terminate task */
void esp32_spi_slave_end(ESP32SPISlave_C* dev);

/* blocking transfer (performs one transaction and waits) */
size_t esp32_spi_slave_transfer(ESP32SPISlave_C* dev,
                                const uint8_t* tx_buf,
                                uint8_t* rx_buf,
                                size_t size,
                                uint32_t timeout_ms);

/* queue a transaction to internal buffer (must be multiple of 4 bytes) */
bool esp32_spi_slave_queue(ESP32SPISlave_C* dev,
                           const uint8_t* tx_buf,
                           uint8_t* rx_buf,
                           size_t size);

/* trigger queued transactions to execute in background (non-blocking) */
bool esp32_spi_slave_trigger(ESP32SPISlave_C* dev, uint32_t timeout_ms);

/* status helpers (module-level; note: this implementation uses module-global queues, so these are global) */
size_t esp32_spi_slave_num_in_flight(void);
size_t esp32_spi_slave_num_completed(void);
size_t esp32_spi_slave_num_errors(void);

/* pop one completed result (oldest) - returns received bytes */
size_t esp32_spi_slave_num_bytes_received(void);
/* pop one error (oldest) */
//esp_err_t esp32_spi_slave_error(void);

/* user callbacks: set ISR-time user callbacks (post_setup/post_trans) */
void esp32_spi_slave_set_user_post_setup_cb(ESP32SPISlave_C* dev, spi_slave_user_cb_t cb, void* arg);
void esp32_spi_slave_set_user_post_trans_cb(ESP32SPISlave_C* dev, spi_slave_user_cb_t cb, void* arg);

#ifdef __cplusplus
}
#endif

#endif /* ESP32SPI_SLAVE_C_H */

