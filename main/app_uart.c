#include <string.h>
#include <math.h>
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "app_uart.h"
#include "app_driver.h"

#define TXD_PIN (GPIO_NUM_23)
#define RXD_PIN (GPIO_NUM_18)

static const int RX_BUF_SIZE = 1024;

double power_read, current_read, voltage_read;

static int uart_send_data(const char *data, const int len)
{
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    return txBytes;
}

static int uart_write_data(uint16_t addr, uint32_t data)
{
    uint8_t tx_buf[8];
    tx_buf[0] = V9260S_HEAD;
    tx_buf[1] = (uint8_t)((addr & 0x0f00) >> 4) + RacWrite;
    tx_buf[2] = (uint8_t)(addr & 0x00ff);
    tx_buf[3] = (uint8_t)(data & 0x000000ff);
    tx_buf[4] = (uint8_t)((data & 0x0000ff00) >> 8);
    tx_buf[5] = (uint8_t)((data & 0x00ff0000) >> 16);
    tx_buf[6] = (uint8_t)((data & 0xff000000) >> 24);
    tx_buf[7] = 0;
    for (int i = 0; i < 7; i++)
    {
        tx_buf[7] += tx_buf[i];
    }
    tx_buf[7] = ~tx_buf[7] + 0x33;
    return uart_send_data((const char *)tx_buf, 8);
}

static void uart_receive_data(void)
{
    uint8_t *rxData = (uint8_t *)malloc(RX_BUF_SIZE + 1);
    const int rxBytes = uart_read_bytes(UART_NUM_1, rxData, RX_BUF_SIZE, 100 / portTICK_RATE_MS);
    if (rxBytes > 0)
    {
        if ((rxData[0] == V9260S_HEAD) && (rxData[1] == RacRead))
        {
            uint8_t len = rxData[2];
            int32_t meter_data[len];
            for (int i = 0; i < len; i++)
            {
                meter_data[i] = rxData[i * 4 + 6];
                meter_data[i] = (meter_data[i] << 8) | rxData[i * 4 + 5];
                meter_data[i] = (meter_data[i] << 8) | rxData[i * 4 + 4];
                meter_data[i] = (meter_data[i] << 8) | rxData[i * 4 + 3];

                if (i == 0)
                {
                    power_read = abs(meter_data[i]);
                    power_read = (power_read / pow(10, 5)) * 1.1;
                }
                else if (i == 3)
                {
                    current_read = meter_data[i];
                    current_read = (current_read / pow(10, 8)) * 1.1;
                    if (current_read > 10)
                    {
                        relay_set_state(false);
                    }
                }
                else if (i == 4)
                {
                    voltage_read = meter_data[i];
                    voltage_read = (voltage_read / pow(10, 6)) * 1.1;
                }
            }
        }
    }
    free(rxData);
}

static void uart_reset(void)
{
    uart_write_data(RegSFTRST, 0x4572BEAF);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    uart_write_data(RegSysCtrl, 0x28064008);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void uart_read_data(uint16_t addr, uint8_t num)
{
    uint8_t tx_buf[8];
    tx_buf[0] = V9260S_HEAD;
    tx_buf[1] = (uint8_t)((addr & 0x0f00) >> 4) + RacRead;
    tx_buf[2] = (uint8_t)(addr & 0x00ff);
    tx_buf[3] = num;
    tx_buf[4] = 0;
    tx_buf[5] = 0;
    tx_buf[6] = 0;
    tx_buf[7] = 0;
    for (int i = 0; i < 7; i++)
    {
        tx_buf[7] += tx_buf[i];
    }
    tx_buf[7] = ~tx_buf[7] + 0x33;
    uart_send_data((const char *)tx_buf, 8);
    uart_receive_data();
}

void uart_init(void)
{
    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_ODD,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0));

    uart_reset();
}