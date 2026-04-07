#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LCD_RS  GPIO_NUM_19
#define LCD_EN  GPIO_NUM_18
#define LCD_D4  GPIO_NUM_5
#define LCD_D5  GPIO_NUM_13
#define LCD_D6  GPIO_NUM_25
#define LCD_D7  GPIO_NUM_4

static void lcd_pulse_enable(void){//falling edge
    gpio_set_level(LCD_EN,1);
    vTaskDelay(pdMS_TO_TICKS(1));
    gpio_set_level(LCD_EN,0);
    vTaskDelay(pdMS_TO_TICKS(1));
}
static void lcd_send_nibble(uint8_t nibble){
    gpio_set_level(LCD_D4,(nibble>>0)&1);
    gpio_set_level(LCD_D5,(nibble>>1)&1);
    gpio_set_level(LCD_D6,(nibble>>2)&1);
    gpio_set_level(LCD_D7,(nibble>>3)&1);
    lcd_pulse_enable();//reads on falling edge
}
static void lcd_send_byte(uint8_t byte, int is_data){
    gpio_set_level(LCD_RS, is_data);
    lcd_send_nibble(byte >> 4);
    lcd_send_nibble(byte & 0x0F);
}

static void lcd_command(uint8_t cmd){
    lcd_send_byte(cmd, 0);
    vTaskDelay(pdMS_TO_TICKS(2));
}

static void lcd_char(char c){
    lcd_send_byte(c, 1);
}