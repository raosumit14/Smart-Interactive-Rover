#include "ssd1306.h"
#include <string.h>

static uint8_t buffer[WIDTH * HEIGHT / 8];
static i2c_port_t i2c_port;

static void ssd1306_write_cmd(uint8_t cmd)
{
    uint8_t data[2] = {0x00, cmd};
    i2c_master_write_to_device(i2c_port, SSD1306_ADDR, data, 2, 1000 / portTICK_PERIOD_MS);
}

static void ssd1306_write_data(uint8_t *data, size_t len)
{
    uint8_t temp[len + 1];
    temp[0] = 0x40;
    memcpy(&temp[1], data, len);

    i2c_master_write_to_device(i2c_port, SSD1306_ADDR, temp, len + 1, 1000 / portTICK_PERIOD_MS);
}

void ssd1306_init(i2c_port_t i2c_num)
{
    i2c_port = i2c_num;

    vTaskDelay(pdMS_TO_TICKS(100));

    ssd1306_write_cmd(0xAE); // display off
    ssd1306_write_cmd(0x20); // memory mode
    ssd1306_write_cmd(0x00); // horizontal

    ssd1306_write_cmd(0xB0);
    ssd1306_write_cmd(0xC8);
    ssd1306_write_cmd(0x00);
    ssd1306_write_cmd(0x10);

    ssd1306_write_cmd(0x40);
    ssd1306_write_cmd(0x81);
    ssd1306_write_cmd(0xFF);

    ssd1306_write_cmd(0xA1);
    ssd1306_write_cmd(0xA6);

    ssd1306_write_cmd(0xA8);
    ssd1306_write_cmd(0x3F);

    ssd1306_write_cmd(0xA4);
    ssd1306_write_cmd(0xD3);
    ssd1306_write_cmd(0x00);

    ssd1306_write_cmd(0xD5);
    ssd1306_write_cmd(0xF0);

    ssd1306_write_cmd(0xD9);
    ssd1306_write_cmd(0x22);

    ssd1306_write_cmd(0xDA);
    ssd1306_write_cmd(0x12);

    ssd1306_write_cmd(0xDB);
    ssd1306_write_cmd(0x20);

    ssd1306_write_cmd(0x8D);
    ssd1306_write_cmd(0x14);

    ssd1306_write_cmd(0xAF); // display ON

    ssd1306_clear();
    ssd1306_update();
}

void ssd1306_clear(void)
{
    memset(buffer, 0x00, sizeof(buffer));
}

void ssd1306_update(void)
{
    for (int page = 0; page < 8; page++)
    {
        ssd1306_write_cmd(0xB0 + page);
        ssd1306_write_cmd(0x00);
        ssd1306_write_cmd(0x10);

        ssd1306_write_data(&buffer[WIDTH * page], WIDTH);
    }
}

void ssd1306_draw_pixel(int x, int y, bool color)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;

    if (color)
        buffer[x + (y / 8) * WIDTH] |= (1 << (y % 8));
    else
        buffer[x + (y / 8) * WIDTH] &= ~(1 << (y % 8));
}

void ssd1306_fill_rect(int x, int y, int w, int h, bool color)
{
    for (int i = x; i < x + w; i++)
    {
        for (int j = y; j < y + h; j++)
        {
            ssd1306_draw_pixel(i, j, color);
        }
    }
}