#ifndef PIXLED_DRIVER_H
#define PIXLED_DRIVER_H

#include <stdint.h>

#include <driver/rmt.h>
#include "constants.hpp"
#include "output.hpp"
#include "pixel.hpp"
#include "strip_config.hpp"
#include "strip.hpp"

/**
 * @mainpage ESP32 Led Strip Driver (RGB and RGBW)
 *
 * @author PaulBreugnot
 * @brief ESP-IDF led strip driver
 * @see GitHub : https://github.com/PixLed/ESP32_RGBW_Led_Driver/
 *
 * This esp-idf component allows you to control RGB / RGBW led strips.
 *
 * In addition to the base control algorithm, algorithms are also implemented
 * to convert HSB colors to RGB, and RGB colors to RGBW. This last feature
 * allows to take full advantage of the white led of RGBW strips, such as
 * SK6812 LEDs, but still working with RGB colors (that is used in practice to
 * compute animations for example).
 *
 * Moreover, the driver can be used to drive *any* type of led hardware, thanks
 * to a generic output implementation, for RGB and RGBW strips.
 *
 * The common interface allows user to write hardware independent code that
 * could work with any type of LED.
 *
 * Example code to drive RGBW and RGB strips with the same commands :
 *
 * @code{.cpp}
 * #include <stdio.h>
 * #include "pixled_driver.hpp"
 * #include "freertos/task.h"
 *
 * #define WS2812_PIN GPIO_NUM_14
 * #define SK6812_PIN GPIO_NUM_12
 * #define NUM_LED 5
 *
 * extern "C" {
 *    void app_main();
 * }
 *
 * void app_main()
 * {
 *   RgbwStrip sk6812 {SK6812_PIN, NUM_LED, RMT_CHANNEL_0, SK6812W()};
 *   RgbStrip ws2812 {WS2812_PIN, NUM_LED, RMT_CHANNEL_1, WS2812()};
 *
 *   while(1) {
 *     for (int h=0; h < 360; h++) {
 *       for (int i=0; i < NUM_LED; i++) {
 *           sk6812.setHsbPixel(i, h, 0.8, 0.7);
 *           ws2812.setHsbPixel(i, h, 0.8, 0.7);
 *       }
 *       sk6812.show();
 *       ws2812.show();
 *       vTaskDelay(50 / portTICK_PERIOD_MS);
 *     }
 * }
 * @endcode
 */

/*
 * @file LedStrip.h
 *
 * Two functions, RGBtoRGBW1 and RGBtoRGBW2, are available to convert RGB
 * components to RGBW components.
 *
 * By default, RGBtoRGBW2 is used. You can set up the function :
 *  - at compile time, with Make Menuconfig => Component config => Led Strip Config
 *  - at runtime, see RGBW_Strip::setRgbToRgbwConverter
 *
 */
#endif /* MAIN_LEDSTRIP_H_ */
