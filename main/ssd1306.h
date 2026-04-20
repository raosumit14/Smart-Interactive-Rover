#ifndef SSD1306_H
#define SSD1306_H

#include "driver/i2c.h"
#include <stdint.h>
#include <stdbool.h>

#define SSD1306_ADDR 0x3C

#define WIDTH 128
#define HEIGHT 64

void ssd1306_init(i2c_port_t i2c_num);
void ssd1306_clear(void);
void ssd1306_update(void);

void ssd1306_draw_pixel(int x, int y, bool color);
void ssd1306_fill_rect(int x, int y, int w, int h, bool color);

#endif