#include "pixled_driver.h"
#include "freertos/task.h"

#define SK6812_PIN 12
#define NUM_LED 30

extern "C" void app_main() {
	//SK6812W sk6812 = SK6812W((gpio_num_t) SK6812_PIN, NUM_LED, RMT_CHANNEL_0);
	WS2812 sk6812 = {(gpio_num_t) SK6812_PIN, NUM_LED, RMT_CHANNEL_0};
	while(1) {
		for (int h=0; h < 360; h++) {
			for (int i=0; i < NUM_LED; i++) {
				sk6812.setHsbPixel(i, h, 0.8, 0.7);
			}
			sk6812.show();
			vTaskDelay(50 / portTICK_PERIOD_MS);
		}
	}
}
