#pragma once
#include <stdint.h>

typedef struct {
    float temperature;
    float humidity;
} dht11_data_t;

dht11_data_t dht11_read(int gpio_num);