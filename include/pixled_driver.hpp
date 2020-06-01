#ifndef PIXLED_DRIVER_H
#define PIXLED_DRIVER_H

#include <stdint.h>

#include <driver/rmt.h>
#include <driver/gpio.h>
#include "esp_log.h"
#include "constants.hpp"
#include "pixel.hpp"
#include "output.hpp"
#include "converters.hpp"
#include "strip_config.hpp"

#define RGB_TO_RGBW_CONVERTER ComplexRgbToRgbwConverter

static const char* PIXLED_LOG_TAG = "PIXLED_DRIVER";

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
 * @brief General and abstract led Strip class.
 */
class Strip {
	public:
		Strip(gpio_num_t gpio_num, uint16_t pixel_count, int channel, StripConfig& strip_config);
		virtual void show() = 0;

		virtual void setPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue) = 0;
		virtual void setPixel(uint16_t index, uint32_t pixel) = 0;
		virtual void setPixel(uint16_t index, rgb_pixel pixel) = 0;

		virtual void setHsbPixel(uint16_t index, float hue, float saturation, float brightness) = 0;
		virtual void setHsbPixel(uint16_t index, hsb_pixel pixel) = 0;

		virtual void clear() = 0;
		virtual ~Strip();

	protected:
		uint16_t pixel_count;
		char* color_order;

		rmt_channel_t  channel;
		rmt_item32_t*  items;

		StripConfig& strip_config;
/*
 *
 *        uint8_t t0h;
 *        uint8_t t0l;
 *        uint8_t t1h;
 *        uint8_t t1l;
 */

		void setItem1(rmt_item32_t* pItem);
		void setItem0(rmt_item32_t* pItem);
};

/**
 * @brief RGB strip class.
 */
class RgbStrip: public Strip {
	protected:
		RgbStripConfig rgb_strip_config;
		HsbToRgbConverter hsb_to_rgb;

	public:
		RgbStrip(gpio_num_t gpio_num, uint16_t pixel_count, int channel, RgbStripConfig& config);
		void show();

		void setPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue);
		void setPixel(uint16_t index, rgb_pixel pixel);
		void setPixel(uint16_t index, uint32_t pixel);

		void setHsbPixel(uint16_t index, float hue, float saturation, float brightness);
		void setHsbPixel(uint16_t index, hsb_pixel pixel);

		void clear();

		virtual ~RgbStrip();
		rgb_pixel* pixels;
};

/**
 * RGBW strip class.
 */
class RgbwStrip:  public Strip {
public:
	RgbwStrip(gpio_num_t gpio_num, uint16_t pixel_count, int channel, RgbwStripConfig& config);

	void setPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue);
	void setPixel(uint16_t index, rgb_pixel pixel);
	void setPixel(uint16_t index, rgbw_pixel pixel);
	void setPixel(uint16_t index, uint32_t pixel);

	void setHsbPixel(uint16_t index, float hue, float saturation, float brightness);
	void setHsbPixel(uint16_t index, hsb_pixel pixel);

	void setPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white);

	void clear();
	void show();
	//void setRgbToRgbwConverter(rgbw_pixel (*converter) (uint8_t, uint8_t, uint8_t));
	virtual ~RgbwStrip();

private:
	RgbwStripConfig rgbw_strip_config;
	HsbToRgbConverter hsb_to_rgb;
	RGB_TO_RGBW_CONVERTER rgb_to_rgbw;

	rgbw_pixel* pixels;
};

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

#endif /* MAIN_LEDSTRIP_H_ */
