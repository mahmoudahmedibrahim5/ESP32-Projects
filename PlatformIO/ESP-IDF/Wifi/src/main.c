#include <unistd.h>
#include <driver/gpio.h>

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#define WIFI_SSID "Alien"
#define WIFI_PASS "alien'spassword"
#define LED_PIN     16

char* TAG = "MyApp";
bool WifiConnected = false;

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) 
{
    if (event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
        ESP_LOGI(TAG, "WiFi connecting...");
    } 
    else if (event_id == WIFI_EVENT_STA_CONNECTED) {
        WifiConnected = true;
        ESP_LOGI(TAG, "WiFi connected.");
    } 
    else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
        WifiConnected = false;
        ESP_LOGI(TAG, "WiFi lost connection. Reconnecting...");
        esp_wifi_connect();
    } 
}

void wifi_init(void) 
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));

    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL);
    //esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void app_main() 
{
    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_init();
    while(!WifiConnected);

    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    
    while (1)
    {
        gpio_set_level(LED_PIN, 1);
        ESP_LOGI(TAG, "LED ON");
        usleep(500000);
        
        gpio_set_level(LED_PIN, 0);
        ESP_LOGI(TAG, "LED OFF");
        usleep(500000);
    }
    
}
