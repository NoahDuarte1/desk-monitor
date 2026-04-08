#include "sound.h"
#include "driver/adc.h"

#define SOUND_CHANNEL ADC1_CHANNEL_5

void sound_init(void) {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(SOUND_CHANNEL, ADC_ATTEN_DB_12);
}

int sound_read(void){
    int sum = 0;
    for(int i = 0; i < 64; i++){
        sum += adc1_get_raw(SOUND_CHANNEL);
    }
    return sum/64;
}