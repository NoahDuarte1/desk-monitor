#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "lcd.h"

typedef struct{
    float temperature;
    float humidity;
    int noise_level;
} env_data_t;

QueueHandle_t env_queue;

void sensor_task(void *pvParameters){
    env_data_t data;
    while(1){
        data.temperature = 22.5;
        data.humidity = 55.0;
        xQueueOverwrite(env_queue, &data);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void display_task(void *pvParameters){
    env_data_t data;
    char buf[16];
    while(1){
        if(xQueuePeek(env_queue, &data, pdMS_TO_TICKS(100)) == pdTRUE){
            lcd_set_cursor(0,0);
            snprintf(buf, sizeof(buf), "Temp: %.1fC     ", data.temperature);
            lcd_print(buf);
            lcd_set_cursor(0, 1);
            snprintf(buf, sizeof(buf), "Hum:  %.1f%%     ", data.humidity);
            lcd_print(buf);
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main(void) {
    lcd_init();
    
    env_queue = xQueueCreate(1, sizeof(env_data_t));

    xTaskCreate(sensor_task, "sensor", 2048, NULL, 1, NULL);
    xTaskCreate(display_task, "display", 2048, NULL, 1, NULL);
}