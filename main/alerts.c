#include "alerts.h"
#include "driver/gpio.h"

#define GREEN GPIO_NUM_14
#define YELLOW GPIO_NUM_27
#define RED GPIO_NUM_26

void alerts_init(void){
    gpio_set_direction(GREEN, GPIO_MODE_OUTPUT);
    gpio_set_level(GREEN, 0);
    gpio_set_direction(YELLOW, GPIO_MODE_OUTPUT);
    gpio_set_level(YELLOW, 0);
    gpio_set_direction(RED, GPIO_MODE_OUTPUT);
    gpio_set_level(RED, 0);
}

void alerts_update(float temperature, int noise_level){
    if(temperature > 32 || noise_level > 2500){
        gpio_set_level(RED, 1);
        gpio_set_level(YELLOW, 0);
        gpio_set_level(GREEN, 0);
    }
    else if(temperature > 28 || noise_level > 1500){
        gpio_set_level(RED, 0);
        gpio_set_level(YELLOW, 1);
        gpio_set_level(GREEN, 0);
    }else{
        gpio_set_level(RED, 0);
        gpio_set_level(YELLOW, 0);
        gpio_set_level(GREEN, 1);
    }
}