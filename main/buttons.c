#include "buttons.h"
#include "driver/adc.h"

#define BUTTON_CHANNEL ADC1_CHANNEL_6

void button_init(void) {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(BUTTON_CHANNEL, ADC_ATTEN_DB_12);
}

button_t button_read(void) {
    int val = adc1_get_raw(BUTTON_CHANNEL);
    printf("btn: %d\n", val);

    if (val < 100) return BUTTON_RIGHT;
    else if (val < 1500) return BUTTON_UP;
    else if (val < 2500) return BUTTON_DOWN;
    else if (val < 3500) return BUTTON_LEFT;
    else if (val < 3900) return BUTTON_SELECT;
    else return BUTTON_NONE;
}