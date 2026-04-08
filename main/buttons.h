#pragma once

typedef enum {
    BUTTON_NONE,
    BUTTON_RIGHT,
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_SELECT
} button_t;

void button_init(void);

button_t button_read(void);