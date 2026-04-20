#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_spp_api.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "ctype.h"

#define IN1 25
#define IN2 26
#define IN3 32
#define IN4 33
#define DEVICE_NAME "MyRobot"
#define SPP_SERVER_NAME "SPP_SERVER"

static const char *TAG = "BT_ROBOT";
uint32_t spp_handle = 0;
void motor_init()
{
    gpio_set_direction(IN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(IN2, GPIO_MODE_OUTPUT);
    gpio_set_direction(IN3, GPIO_MODE_OUTPUT);
    gpio_set_direction(IN4, GPIO_MODE_OUTPUT);

    gpio_set_level(IN1, 0);
    gpio_set_level(IN2, 0);
    gpio_set_level(IN3, 0);
    gpio_set_level(IN4, 0);
}
void stop()
{
    gpio_set_level(IN1, 0);
    gpio_set_level(IN2, 0);
    gpio_set_level(IN3, 0);
    gpio_set_level(IN4, 0);
}
void forward()
{
    gpio_set_level(IN1, 1);
    gpio_set_level(IN2, 0);
    gpio_set_level(IN3, 1);
    gpio_set_level(IN4, 0);
    ESP_LOGI(TAG, "FORWARD ACTION");

    char *msg = "Moving forward\n";
    esp_spp_write(spp_handle, strlen(msg), (uint8_t *)msg);
   // vTaskDelay(pdMS_TO_TICKS(3000));
    // stop();
}

void left()
{
    gpio_set_level(IN1, 0);
    gpio_set_level(IN2, 1);
    gpio_set_level(IN3, 1);
    gpio_set_level(IN4, 0);
    char *msg = "Turning Left\n";
    esp_spp_write(spp_handle, strlen(msg), (uint8_t *)msg);
    //vTaskDelay(pdMS_TO_TICK(1000));
    // stop();
}

void right()
{
    gpio_set_level(IN1, 1);
    gpio_set_level(IN2, 0);
    gpio_set_level(IN3, 0);
    gpio_set_level(IN4, 1);
    char *msg = "Turning Right\n";
    esp_spp_write(spp_handle, strlen(msg), (uint8_t *)msg);
    //vTaskDelay(pdMS_TO_TICK(1000));
    // stop();
}


void reverse()
{
    gpio_set_level(IN1, 0);
    gpio_set_level(IN2, 1);
    gpio_set_level(IN3, 0);
    gpio_set_level(IN4, 1);

    ESP_LOGI(TAG, "REVERSE for 2 sec");
char *msg = "Moving backward\n";
    esp_spp_write(spp_handle, strlen(msg), (uint8_t *)msg);
  // vTaskDelay(pdMS_TO_TICKS(3000));

    //stop();
}

/*void forward()
{

}
void left()
{
    ESP_LOGI(TAG, "LEFT ACTION");
}

void right()
{
    ESP_LOGI(TAG, "RIGHT ACTION");
}

void stop()
{
    ESP_LOGI(TAG, "STOP ACTION");
}

static void handle_command(char *cmd)

{
    // Remove \r and \n
    cmd[strcspn(cmd, "\r\n")] = 0;

    ESP_LOGI(TAG, "Cleaned Command: '%s'", cmd);

    if (strcmp(cmd, "forward") == 0)
    {
        forward();
    }
    else if (strcmp(cmd, "left") == 0)
    {
        left();
    }
    else if (strcmp(cmd, "right") == 0)
    {
        right();
    }
    else if (strcmp(cmd, "stop") == 0)
    {
        stop();
    }
    else
    {
        ESP_LOGI(TAG, "Unknown Command: %s", cmd);
    }
}*/
static void handle_command(char *cmd)
{
    // remove \n
    cmd[strcspn(cmd, "\r\n")] = 0;

    // convert to lowercase
    for (int i = 0; cmd[i]; i++)
    {
        cmd[i] = tolower(cmd[i]);
    }

    ESP_LOGI(TAG, "Cleaned Command: '%s'", cmd);

    //  HERE use strstr : if any word related to this then it will work
    if (strstr(cmd, "forward"))
    {
        forward();
    }
    else if (strstr(cmd, "left"))
    {
        left();
    }
    else if (strstr(cmd, "right"))
    {
        right();
    }
    else if (strstr(cmd, "stop"))
    {
        stop();
    }
    else if (strstr(cmd, "reverse"))
    {
        reverse();
    }
    else
    {
        ESP_LOGI(TAG, "Unknown Command");
    }
}
static void spp_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
    switch (event)
    {

    case ESP_SPP_INIT_EVT:
        ESP_LOGI(TAG, "SPP Initialized");
        esp_bt_dev_set_device_name(DEVICE_NAME);
        esp_spp_start_srv(ESP_SPP_SEC_NONE, ESP_SPP_ROLE_SLAVE, 0, SPP_SERVER_NAME);
        break;

    case ESP_SPP_START_EVT:
        ESP_LOGI(TAG, "SPP Server Started");
        esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
        break;

    case ESP_SPP_DATA_IND_EVT:
    {
        char data[100] = {0};

        int len = param->data_ind.len;

        if (len >= sizeof(data))
        {
            len = sizeof(data) - 1;
        }

        memcpy(data, param->data_ind.data, len);
        data[len] = '\0';

        ESP_LOGI(TAG, "Received: %s", data);

        // Remove \r and \n
        data[strcspn(data, "\r\n")] = 0;

        handle_command(data);
        break;
    } //  newly added
    case ESP_SPP_SRV_OPEN_EVT:
    {
        spp_handle = param->srv_open.handle;
        ESP_LOGI(TAG, "Client Connected");
        break;
    }
        //  till here
    default:
        break;
    }
}

void blue_user(void *pv)
{
    esp_err_t ret;
    motor_init();

    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        nvs_flash_erase();
        nvs_flash_init();
    }

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT));

    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_ERROR_CHECK(esp_bluedroid_enable());

    ESP_ERROR_CHECK(esp_spp_register_callback(spp_callback));
    ESP_ERROR_CHECK(esp_spp_init(ESP_SPP_MODE_CB));

    //  Keep task alive
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
