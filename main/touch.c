                                      // For LCD Display and touch sensor

/*#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#define BUTTON GPIO_NUM_5
#define LED    GPIO_NUM_23

int press = 0;
int buttonState = 0;
int lastButtonState = 0;

int64_t lastDebounceTime = 0;
int64_t time_start = 0;

const int debounceDelay = 50;
const int Delaytime = 3500;

void touch_user(void *arg)
{
    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    while (1)
    {
        int64_t current_time = esp_timer_get_time() / 1000;
        int reading = gpio_get_level(BUTTON);

        // Debounce logic
        if (reading != lastButtonState) {
            lastDebounceTime = current_time;
        }

        if ((current_time - lastDebounceTime) > debounceDelay) {

            if (reading != buttonState) {
                buttonState = reading;


                    press++;

                    if (press == 1) {
                        time_start = current_time;
                    }

                    printf("Press count: %d\n", press);
                }
            }
        }

        lastButtonState = reading;

        // Only check timer if at least 1 press happened
        if ( (current_time - time_start) > Delaytime)
        {
            if (press == 4) {
                gpio_set_level(LED, 1);
                vTaskDelay(pdMS_TO_TICKS(3000));
                gpio_set_level(LED, 0);
            }

            // reset
            press = 0;
            time_start = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}*/

/*#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "lcd_i2c.h"

#define BUTTON GPIO_NUM_5
#define LED    GPIO_NUM_23

int press = 0;
int buttonState = 1;
int lastButtonState = 1;

int64_t lastDebounceTime = 0;
int64_t timein = 0;

const int debounceDelay = 50;
const int Delaytime = 2000;

void touch_user(void *arg)
{
    // GPIO setup
    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON, GPIO_PULLUP_ONLY);

    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    // LCD init
    lcd_init();
    lcd_backlight_on();

    lcd_set_cursor(0, 0);
    lcd_print("Ready...");

    while (1)
    {
        int64_t current_time = esp_timer_get_time() / 1000;
        int reading = gpio_get_level(BUTTON);

        // detect press (falling edge)
        if (reading == 0 && lastButtonState == 1) {
            lastDebounceTime = current_time;
        }

        // debounce
        if ((current_time - lastDebounceTime) > debounceDelay) {
            if (reading == 0 && buttonState == 1) {

                press++;
                timein = current_time;

                //  Update 2nd row with press count
                lcd_set_cursor(0, 1);
                lcd_print("                ");  // clear row

                lcd_set_cursor(0, 1);
                char buf[16];
                sprintf(buf, "Press:%d", press);
                lcd_print(buf);
            }
            buttonState = reading;
        }

        lastButtonState = reading;

        // after delay → show result
        if ((current_time - timein > Delaytime) && press > 0)
        {
            lcd_clear();

            if (press == 1) {
                lcd_set_cursor(0, 0);
                lcd_print("Hello Sir");

            } else if (press == 2) {
                lcd_set_cursor(0, 0);
                lcd_print("Sad Face :(");

            } else if (press == 3) {
                lcd_set_cursor(0, 0);
                lcd_print("I am Micro");

            } else if (press == 4) {
                gpio_set_level(LED, 1);

                lcd_set_cursor(0, 0);
                lcd_print("Smiley :)");

            } else {
                lcd_set_cursor(0, 0);
                lcd_print("Too many press");
            }

            vTaskDelay(pdMS_TO_TICKS(2000));

            gpio_set_level(LED, 0);

            // reset
            press = 0;
            timein = current_time;

            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_print("Ready...");
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}*/
/*#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "ssd1306.h"
#include "lcd_i2c.h"

#define SDA 21
#define SCL 22

void i2c_init()
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA,
        .scl_io_num = SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000};
    i2c_param_config(I2C_NUM_0, &conf);
    i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
}

void drawEyes(int blink)
{
    ssd1306_clear();

    int y = 15 + blink;
    int h = 30 - blink * 2;

    ssd1306_fill_rect(20, y, 30, h, 1);
    ssd1306_fill_rect(78, y, 30, h, 1);

    ssd1306_update();
}

void touch_user(void *pv)
{
    i2c_master_init();                 // MUST be first
    vTaskDelay(pdMS_TO_TICKS(100));   // give system time

    ssd1306_init(I2C_NUM_0);          // THEN init display

    int blink = 0;
    int dir = 1;

    while (1)
    {
        drawEyes(blink);

        blink += dir;

        if (blink >= 10 || blink <= 0)
            dir = -dir;

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}*/
                     // without interface of touch sensorfor oled 

#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include <string.h>  

#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 100000

#define OLED_ADDR 0x3C

uint8_t buffer[1024]; // 128x64 / 8

// ---------------- I2C ----------------
void i2c_master_init() {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

void oled_cmd(uint8_t cmd) {
    uint8_t data[2] = {0x00, cmd};
    i2c_master_write_to_device(I2C_MASTER_NUM, OLED_ADDR, data, 2, 1000 / portTICK_PERIOD_MS);
}

void oled_data(uint8_t *data, size_t len) {
    uint8_t temp[len + 1];
    temp[0] = 0x40;
    memcpy(&temp[1], data, len);
    i2c_master_write_to_device(I2C_MASTER_NUM, OLED_ADDR, temp, len + 1, 1000 / portTICK_PERIOD_MS);
}

// ---------------- OLED INIT ----------------
void oled_init() {
    oled_cmd(0xAE); // OFF
    oled_cmd(0x20); oled_cmd(0x00);
    oled_cmd(0xB0);
    oled_cmd(0xC8);
    oled_cmd(0x00);
    oled_cmd(0x10);
    oled_cmd(0x40);
    oled_cmd(0x81); oled_cmd(0xFF);
    oled_cmd(0xA1);
    oled_cmd(0xA6);
    oled_cmd(0xA8); oled_cmd(0x3F);
    oled_cmd(0xA4);
    oled_cmd(0xD3); oled_cmd(0x00);
    oled_cmd(0xD5); oled_cmd(0xF0);
    oled_cmd(0xD9); oled_cmd(0x22);
    oled_cmd(0xDA); oled_cmd(0x12);
    oled_cmd(0xDB); oled_cmd(0x20);
    oled_cmd(0x8D); oled_cmd(0x14);
    oled_cmd(0xAF); // ON
}

// ---------------- DRAW ----------------
void clear_buffer() {
    memset(buffer, 0, sizeof(buffer));
}

void draw_pixel(int x, int y) {
    buffer[x + (y / 8) * 128] |= (1 << (y % 8));
}

void draw_rect(int x, int y, int w, int h) {
    for (int i = x; i < x + w; i++)
        for (int j = y; j < y + h; j++)
            draw_pixel(i, j);
}

void oled_update() {
    for (int i = 0; i < 8; i++) {
        oled_cmd(0xB0 + i);
        oled_cmd(0x00);
        oled_cmd(0x10);
        oled_data(&buffer[128 * i], 128);
    }
}

// ---------------- FACE ----------------
void draw_face(int offset, int blink) {
    clear_buffer();

    int eyeY = 20;

    if (blink) {
        draw_rect(30 + offset, eyeY + 8, 16, 2);
        draw_rect(80 + offset, eyeY + 8, 16, 2);
    } else {
        draw_rect(30 + offset, eyeY, 16, 20);
        draw_rect(80 + offset, eyeY, 16, 20);
    }

    // Smirk
    draw_rect(55, 45, 18, 4);

    oled_update();
}

// ---------------- MAIN ----------------
void touch_user(void *pv) {
    i2c_master_init();
    oled_init();

    while (1) {
        draw_face(0, 0); // normal
        vTaskDelay(1500 / portTICK_PERIOD_MS);

        draw_face(-5, 0); // left
        vTaskDelay(800 / portTICK_PERIOD_MS);

        draw_face(5, 0); // right
        vTaskDelay(800 / portTICK_PERIOD_MS);

        draw_face(0, 1); // blink
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}
