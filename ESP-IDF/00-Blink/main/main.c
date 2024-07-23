#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "driver/gpio.h"

void app_main(void)
{
	gpio_config_t led;
	led.mode = GPIO_MODE_OUTPUT;
	led.pin_bit_mask = (1 << 4);
	gpio_config(&led);

    while (1)
    {
    	gpio_set_level(4, 0);
    	sleep(1);
    	gpio_set_level(4, 1);
    	sleep(1);
    }
}
