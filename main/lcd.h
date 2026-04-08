#pragma once
#include <stdint.h>

void lcd_init(void);
void lcd_set_cursor(int col, int row);
void lcd_print(const char *str);
void lcd_char(char c);
void lcd_command(uint8_t cmd);