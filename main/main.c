#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "lcd.h"
#include "dht11.h"
#include "sound.h"
#include "alerts.h"
#include "buttons.h"

#define DHT11_PIN 32

typedef struct{
    float temperature;
    float humidity;
    int noise_level;
} env_data_t;

QueueHandle_t env_queue;

void sensor_task(void *pvParameters){
    env_data_t data;
    while(1){
        dht11_data_t reading = dht11_read(DHT11_PIN);
        printf("temp: %.1f, hum: %.1f\n", reading.temperature, reading.humidity);

        if(reading.temperature > 0){
            data.temperature = reading.temperature;
            data.humidity = reading.humidity;
            xQueueOverwrite(env_queue, &data);
        }
        
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void display_task(void *pvParameters){
    env_data_t data;
    int screen = 0;
    char buf[16];
    while(1){
        button_t btn = button_read();
        if (btn == BUTTON_RIGHT) screen = 1;
        else if (btn == BUTTON_LEFT) screen = 0;

        if (screen == 0){
            if(xQueuePeek(env_queue, &data, pdMS_TO_TICKS(100)) == pdTRUE){
                lcd_set_cursor(0,0);
                snprintf(buf, sizeof(buf), "Temp: %.1fC     ", data.temperature);
                lcd_print(buf);
                lcd_set_cursor(0, 1);
                snprintf(buf, sizeof(buf), "Hum:  %.1f%%     ", data.humidity);
                lcd_print(buf);
            }
        }
        if (screen == 1){
            if(xQueuePeek(env_queue, &data, pdMS_TO_TICKS(100)) == pdTRUE){
                lcd_set_cursor(0,0);
                snprintf(buf, sizeof(buf), "Noise:  %d    ", data.noise_level);
                lcd_print(buf);
                lcd_set_cursor(0, 1);
                lcd_print("                ");
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void audio_task(void *pvParameters){
    env_data_t data = {0, 0, 0};
    while(1){
        int noise = sound_read();
        printf("noise: %d\n", noise);

        xQueuePeek(env_queue, &data, 0);
        data.noise_level = noise;
        xQueueOverwrite(env_queue, &data);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
void alert_task(void *pvParameters){
    env_data_t data;
    while(1){
        xQueuePeek(env_queue, &data, 0);
        alerts_update(data.temperature, data.noise_level);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main(void) {
    button_init();
    alerts_init();
    lcd_init();
    
    env_queue = xQueueCreate(1, sizeof(env_data_t));

    xTaskCreate(sensor_task, "sensor", 2048, NULL, 1, NULL);
    xTaskCreate(display_task, "display", 2048, NULL, 1, NULL);
    xTaskCreate(audio_task, "audio", 2048, NULL, 1, NULL);
    xTaskCreate(alert_task, "alert", 2048, NULL, 1, NULL);
    
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}