// /* WiFi station Example
//    This example code is in the Public Domain (or CC0 licensed, at your option.)
//    Unless required by applicable law or agreed to in writing, this
//    software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//    CONDITIONS OF ANY KIND, either express or implied.
// */
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/event_groups.h"
// #include "esp_system.h"
// #include "esp_wifi.h"
// #include "esp_event.h"
// #include "esp_log.h"
// #include "nvs_flash.h"

// #include "lwip/err.h"
// #include "lwip/sys.h"

// /* The examples use WiFi configuration that you can set via project configuration menu
//    If you'd rather not, just change the below entries to strings with
//    the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
// */
// #define EXAMPLE_ESP_WIFI_SSID      "Michal_Rz"
// #define EXAMPLE_ESP_WIFI_PASS      "6d6d6d6d72727a65706b613939"
// #define EXAMPLE_ESP_MAXIMUM_RETRY  5

// /* FreeRTOS event group to signal when we are connected*/
// static EventGroupHandle_t s_wifi_event_group;

// /* The event group allows multiple bits for each event, but we only care about two events:
//  * - we are connected to the AP with an IP
//  * - we failed to connect after the maximum amount of retries */
// #define WIFI_CONNECTED_BIT BIT0
// #define WIFI_FAIL_BIT      BIT1

// static const char *TAG = "wifi station";

// static int s_retry_num = 0;

// static void event_handler(void* arg, esp_event_base_t event_base,
//                                 int32_t event_id, void* event_data)
// {
//     if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
//         esp_wifi_connect();
//     } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
//         if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
//             esp_wifi_connect();
//             s_retry_num++;
//             ESP_LOGI(TAG, "retry to connect to the AP");
//         } else {
//             xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
//         }
//         ESP_LOGI(TAG,"connect to the AP fail");
//     } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
//         ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
//         ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
//         s_retry_num = 0;
//         xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
//     }
// }

// void wifi_init_sta(void)
// {
//     s_wifi_event_group = xEventGroupCreate();

//     ESP_ERROR_CHECK(esp_netif_init());

//     ESP_ERROR_CHECK(esp_event_loop_create_default());
//     esp_netif_create_default_wifi_sta();

//     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_wifi_init(&cfg));

//     ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
//     ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

//     wifi_config_t wifi_config = {
//         .sta = {
//             .ssid = EXAMPLE_ESP_WIFI_SSID,
//             .password = EXAMPLE_ESP_WIFI_PASS
//         },
//     };
//     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
//     ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
//     ESP_ERROR_CHECK(esp_wifi_start() );

//     ESP_LOGI(TAG, "wifi_init_sta finished.");

//     /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
//      * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
//     EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
//             WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
//             pdFALSE,
//             pdFALSE,
//             portMAX_DELAY);

//     /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
//      * happened. */
//     if (bits & WIFI_CONNECTED_BIT) {
//         ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
//                  EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
//     } else if (bits & WIFI_FAIL_BIT) {
//         ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
//                  EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
//     } else {
//         ESP_LOGE(TAG, "UNEXPECTED EVENT");
//     }

//     ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler));
//     ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler));
//     vEventGroupDelete(s_wifi_event_group);
// }

// void app_main(void)
// {
//     //Initialize NVS
//     esp_err_t ret = nvs_flash_init();
//     if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
//       ESP_ERROR_CHECK(nvs_flash_erase());
//       ret = nvs_flash_init();
//     }
//     ESP_ERROR_CHECK(ret);

//     ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
//     wifi_init_sta();
// }


// #include <stdio.h>
// #include <freertos/FreeRTOS.h>
// #include <freertos/task.h>
// #include <sys/time.h>
// #include <hd44780.h>


// static uint32_t get_time_sec()
// {
//     struct timeval tv;
//     gettimeofday(&tv, NULL);
//     return tv.tv_sec;
// }

// static const uint8_t char_data[] = {
//     0x04, 0x0e, 0x0e, 0x0e, 0x1f, 0x00, 0x04, 0x00,
//     0x1f, 0x11, 0x0a, 0x04, 0x0a, 0x11, 0x1f, 0x00
// };

// void lcd_test(void *pvParameters)
// {
//     hd44780_t lcd = {
//         .font = HD44780_FONT_5X8,
//         .lines = 2,
//         .pins = {
//             .rs = GPIO_NUM_19,
//             .e  = GPIO_NUM_18,
//             .d4 = GPIO_NUM_5,
//             .d5 = GPIO_NUM_17,
//             .d6 = GPIO_NUM_16,
//             .d7 = GPIO_NUM_4,
//             .bl = HD44780_NOT_USED
//         }
//     };

//     ESP_ERROR_CHECK(hd44780_init(&lcd));

//     hd44780_upload_character(&lcd, 0, char_data);
//     hd44780_upload_character(&lcd, 1, char_data + 8);

//     hd44780_gotoxy(&lcd, 0, 0);
//     hd44780_puts(&lcd, "\x08 Hello world");
//     hd44780_gotoxy(&lcd, 0, 1);
//     hd44780_puts(&lcd, "\x09 Its working xd");


// }

// void app_main()
// {
//     xTaskCreate(lcd_test, "lcd_test", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
// }

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ds1307.h>
#include <string.h>

#if defined(CONFIG_IDF_TARGET_ESP8266)
#define SDA_GPIO 4
#define SCL_GPIO 5
#else
#define SDA_GPIO 22
#define SCL_GPIO 23
#endif

#define CONFIG_FREERTOS_HZ 100

void ds1307_test(void *pvParameters)
{
    i2c_dev_t dev;
    memset(&dev, 0, sizeof(i2c_dev_t));

    ESP_ERROR_CHECK(ds1307_init_desc(&dev, 0, SDA_GPIO, SCL_GPIO));

    // setup datetime: 2018-04-11 00:52:10
    struct tm time = {
        .tm_year = 2018,
        .tm_mon  = 3,  // 0-based
        .tm_mday = 11,
        .tm_hour = 0,
        .tm_min  = 52,
        .tm_sec  = 10
    };
    ESP_ERROR_CHECK(ds1307_set_time(&dev, &time));

    uint8_t buff[2];
    buff[0] = 12;
    buff[1] = 15;

    ds1307_write_ram(&dev, 0, buff, 2);

    uint8_t buffer[2];

    while (1)
    {
        ds1307_get_time(&dev, &time);
        

        printf("%04d-%02d-%02d %02d:%02d:%02d\n", time.tm_year, time.tm_mon + 1,
            time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);

            ds1307_read_ram(&dev, 0, buffer, 2);
        
        printf("buffer:%d:%d\n", buffer[0], buffer[1]);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    ESP_ERROR_CHECK(i2cdev_init());

    xTaskCreate(ds1307_test, "ds1307_test", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL);
}
