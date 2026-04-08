#include "dht11.h"
#include "driver/gpio.h"
#include "rom/ets_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

dht11_data_t dht11_read(int gpio_num){
    dht11_data_t result = {-1, -1};

    gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT);
    gpio_set_level(gpio_num, 0);
    vTaskDelay(pdMS_TO_TICKS(18));

    gpio_set_level(gpio_num, 1);
    ets_delay_us(40);
    gpio_set_direction(gpio_num, GPIO_MODE_INPUT);

    ets_delay_us(80);
    if (gpio_get_level(gpio_num) == 0){
        result.temperature = -2;
        return result;
    }
    ets_delay_us(80);

    uint8_t data[5] = {0};

    for (int i = 0; i < 40; i++) {
        while (gpio_get_level(gpio_num) == 1);

        while (gpio_get_level(gpio_num) == 0);
    
        ets_delay_us(50);

        if (gpio_get_level(gpio_num) == 1) {
            data[i / 8] |= (1 << (7 - (i % 8)));
        }
    }

    if (data[4] != ((data[0] + data[1] + data[2] + data[3]) & 0xFF)){
        result.temperature = -3;
        return result;
    }

    result.humidity = data[0] + data[1] * 0.1;
    result.temperature = data[2] + data[3] * 0.1;

    return result;
}