#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "esp_err.h"

#include <stdio.h>

uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .source_clk = UART_SCLK_DEFAULT,
};

void app_main(void) {
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_2, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install (UART_NUM_2, 1024, 0, 0, NULL, 0));

    uint8_t expected = 0;
    uint8_t buf[64];
    int len = 0;

    while(1) {
        len = uart_read_bytes(UART_NUM_2, buf, sizeof(buf), pdMS_TO_TICKS(200));

        for (int i = 0; i < len; i++) {
            printf("rx: %u\n", buf[i]);
            if(buf[i] != expected) {
                printf("Missed expected: %u | received: %u \n", expected, buf[i]);
            }

            expected = buf[i] + 1;
        }
    }
}
 