#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "touch.h"
#include "bluetooth.h"

void app_main(void)
{
  xTaskCreate(touch_user, "lcd", 8192, NULL, 1, NULL);
xTaskCreate(blue_user, "bt", 8192, NULL, 3, NULL);
}