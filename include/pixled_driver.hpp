#ifndef PIXLED_DRIVER_H
#define PIXLED_DRIVER_H

#include <stdint.h>

#include <driver/rmt.h>

//#include <stdint.h>
//#include <stdlib.h>
//#include <stdexcept>
//#include <algorithm>

/**
 * @mainpage ESP32 Led Strip Driver (RGB and RGBW)
 *
 * @author PaulBreugnot
 * @brief Driver for Strip/NeoPixel data.
 * @see GitHub : https://github.com/PixLed/ESP32_RGBW_Led_Driver/
 *
 *
 * NeoPixels or Strips are LED devices that can illuminate in arbitrary colors with
 * 8 bits of data for each of the red, green and blue channels.  These devices can be
 * daisy chained together to produce a string of LEDs.  If we call each LED instance
 * a pixel, then when we want to set the value of a string of pixels, we need to supply
 * the data for all the pixels.  This class encapsulates setting the color value for
 * an individual pixel within the string and, once you have set up all the desired colors,
 * you can then set all the pixels in a show() operation.  The class hides from you
 * the underlying details needed to drive the devices.
 *
 * Currently, RGB Strips (WS2812 or NeoPixels, SK6812) and RGBW strips are
 * supported. For RGBW strips, you can either drive manually the 4 leds (be
 * careful to energy consumption if you set the 4 to high value) or drive them
 * normally in RGB. If you do so, the color will be converted to RGBW for a
 * "smart" white composant usage (from https://stackoverflow.com/questions/40312216/converting-rgb-to-rgbw).
 * A simpler function is also available :
 * @code
 *   w = min(r, g, b)
 *   r, g, b = r - w, g - w, b - w
 * @endcode
 *
 *
 * The choice between strip models is made instanciating the corresponding class.
 * Then, functions to set pixels color and show them are equal.
 *
 * Example code to drive RGBW and RGB strips with the same commands :
 *
 * @code{.cpp}
 * #include <stdio.h>
 * #include "LedStrip.h"
 * #include "freertos/task.h"
 *
 * #define WS2812_PIN 14
 * #define SK6812_PIN 12
 * #define NUM_LED 5
 *
 * extern "C" {
 *    void app_main();
 * }
 *
 * void app_main()
 * {
 *   SK6812W sk6812 = SK6812W((gpio_num_t) SK6812_PIN, NUM_LED, RMT_CHANNEL_0);
 *   WS2812 ws2812 = WS2812((gpio_num_t) WS2812_PIN, NUM_LED, RMT_CHANNEL_1);
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

/**
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



/**
 * @brief WS2812 RGB strip implementation.
 */
/*
 *class WS2812: public RgbStrip {
 *public:
 *    WS2812(gpio_num_t gpioNum, uint16_t pixelCount, int channel);
 *};
 */

/**
 * @brief WS2815 RGB strip implementation.
 */
/*
 *class WS2815: public RgbStrip {
 *public:
 *    WS2815(gpio_num_t gpioNum, uint16_t pixelCount, int channel);
 *};
 */

/**
 * @brief SK6812 RGB strip implementation.
 */
/*
 *class SK6812: public RgbStrip {
 *public:
 *    SK6812(gpio_num_t gpioNum, uint16_t pixelCount, int channel);
 *};
 */

/**
 * @brief SK2812 RGBW strip implementation.
 */
/*
 *class SK6812W: public RgbwStrip {
 *public:
 *    SK6812W(gpio_num_t gpioNum, uint16_t pixelCount, int channel);
 *};
 */

namespace pixled_driver {
	void init(rmt_channel_t channel);

}
#endif /* MAIN_LEDSTRIP_H_ */
