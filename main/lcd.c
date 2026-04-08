#include "lcd.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LCD_RS  GPIO_NUM_19
#define LCD_EN  GPIO_NUM_18
#define LCD_D4  GPIO_NUM_23
#define LCD_D5  GPIO_NUM_21
#define LCD_D6  GPIO_NUM_25
#define LCD_D7  GPIO_NUM_22

void lcd_pulse_enable(void){//falling edge
    gpio_set_level(LCD_EN,1);
    vTaskDelay(pdMS_TO_TICKS(1));
    gpio_set_level(LCD_EN,0);
    vTaskDelay(pdMS_TO_TICKS(1));
}
void lcd_send_nibble(uint8_t nibble){
    gpio_set_level(LCD_D4,(nibble>>0)&1);
    gpio_set_level(LCD_D5,(nibble>>1)&1);
    gpio_set_level(LCD_D6,(nibble>>2)&1);
    gpio_set_level(LCD_D7,(nibble>>3)&1);
    lcd_pulse_enable();//reads on falling edge
}
void lcd_send_byte(uint8_t byte, int is_data){
    gpio_set_level(LCD_RS, is_data);
    lcd_send_nibble(byte >> 4);
    lcd_send_nibble(byte & 0x0F);
}

void lcd_command(uint8_t cmd){
    lcd_send_byte(cmd, 0);
    vTaskDelay(pdMS_TO_TICKS(2));
}

void lcd_char(char c){
    lcd_send_byte(c, 1);
    vTaskDelay(pdMS_TO_TICKS(5));
}

void lcd_init(void){
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LCD_RS) | (1ULL << LCD_EN) |
                        (1ULL << LCD_D4) | (1ULL << LCD_D5) |
                        (1ULL << LCD_D6) | (1ULL << LCD_D7),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    vTaskDelay(pdMS_TO_TICKS(20));

    gpio_set_level(LCD_RS, 0);
    gpio_set_level(LCD_EN, 0);

    vTaskDelay(pdMS_TO_TICKS(20));
    lcd_send_nibble(0x03);
    vTaskDelay(pdMS_TO_TICKS(20));
    lcd_send_nibble(0x03);
    vTaskDelay(pdMS_TO_TICKS(20));
    lcd_send_nibble(0x03);
    vTaskDelay(pdMS_TO_TICKS(20));
    lcd_send_nibble(0x02);
    vTaskDelay(pdMS_TO_TICKS(5));

    lcd_command(0x28);
    vTaskDelay(pdMS_TO_TICKS(20));
    lcd_command(0x0C);
    vTaskDelay(pdMS_TO_TICKS(20));
    lcd_command(0x06);
    vTaskDelay(pdMS_TO_TICKS(20));
    lcd_command(0x01);
    vTaskDelay(pdMS_TO_TICKS(20));
}

void lcd_set_cursor(int col, int row){
    uint8_t row_offsets[] = {0x00, 0x40};
    lcd_command(0x80 | (col + row_offsets[row]));
}

void lcd_print(const char *str){
    while (*str){
        lcd_char(*str++);
    }
}