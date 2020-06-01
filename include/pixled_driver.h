#ifndef PIXLED_DRIVER_H
#define PIXLED_DRIVER_H

#include <stdint.h>

#include <driver/rmt.h>
#include <driver/gpio.h>
#include "esp_log.h"
#include "constants.h"

#ifdef CONFIG_RGB_TO_RGBW_1
	#define RGB_TO_RGBW_1 CONFIG_RGB_TO_RGBW_1
#else
	#define RGB_TO_RGBW_1 false
#endif

#ifdef CONFIG_RGB_TO_RGBW_2
	#define RGB_TO_RGBW_2 CONFIG_RGB_TO_RGBW_2
#else
	#define RGB_TO_RGBW_2 true
#endif

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
 * @brief A data type representing an RGB pixel.
 */
struct rgb_pixel {
	rgb_pixel();
	rgb_pixel(uint8_t red, uint8_t green, uint8_t blue);
	/**
	 * @brief The red component of the pixel.
	 */
	uint8_t red;
	/**
	 * @brief The green component of the pixel.
	 */
	uint8_t green;
	/**
	 * @brief The blue component of the pixel.
	 */
	uint8_t blue;
};

/**
 * @brief A data type representing an RGBW pixel.
 */
struct rgbw_pixel : rgb_pixel {
	rgbw_pixel();
	rgbw_pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
	/**
	 * @brief The white component of the pixel.
	 */
	uint8_t white;
};

/**
 * @brief A data type representing an HSB pixel.
 */
struct hsb_pixel {
	hsb_pixel();
	hsb_pixel(float hue, float saturation, float brightness);
	/**
	 * @brief Color hue, in [0;360]
	 */
	float hue;

	/**
	 * @brief Color saturation, in [0;1]
	 */
	float saturation;

	/**
	 * @brief Color brightness, in [0;1]
	 */
	float brightness;
};

/**
 * @fn static rgb_pixel HSBtoRGB(float hue, float saturation, float brightness)
 *
 * @brief Converts the specified HSB color to an RGB color.
 *
 * @param hue between 0 and 360
 * @param saturation between 0 and 1
 * @param brightness between 0 and 1
 * @return converted rgb pixel
 */
rgb_pixel HSBtoRGB(float hue, float saturation, float brightness);

/**
 * @fn static rgbw_pixel RGBtoRGBW1(uint8_t red, uint8_t green, uint8_t blue)
 *
 * @brief First method to convert RGB to RGBW.
 *
 * @code
 *    w = min(r, g, b)
 *    r, g, b = r - w, g - w, b - w
 * @endcode
 *
 * @param red between 0 and 255
 * @param green between 0 and 255
 * @param blue between 0 and 255
 *
 * @return converted rgbw pixel
 */
rgbw_pixel RGBtoRGBW1(uint8_t red, uint8_t green, uint8_t blue); 

/**
 * @fn static rgbw_pixel RGBtoRGBW2(uint8_t red, uint8_t green, uint8_t blue)
 *
 * @brief Second method to convert RGB to RGBW.
 *
 * Based on https://stackoverflow.com/questions/40312216/converting-rgb-to-rgbw
 *
 * @param red between 0 and 255
 * @param green between 0 and 255
 * @param blue between 0 and 255
 *
 * @return converted rgbw pixel
 */
rgbw_pixel RGBtoRGBW2(uint8_t red, uint8_t green, uint8_t blue);

/**
 * @brief General and abstract led Strip class.
 */
class Strip {
public:
	Strip(gpio_num_t gpio_num, uint16_t pixel_count, int channel, uint8_t t0h, uint8_t t0l, uint8_t t1h, uint8_t t1l);
	virtual void show() = 0;
	void setColorOrder(char* order);
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

	uint8_t t0h;
	uint8_t t0l;
	uint8_t t1h;
	uint8_t t1l;

	void setItem1(rmt_item32_t* pItem);
	void setItem0(rmt_item32_t* pItem);
};

/**
 * @brief RGB strip class.
 */
class RgbStrip: public Strip {
public:
	RgbStrip(gpio_num_t gpio_num, uint16_t pixel_count, int channel, uint8_t t0h, uint8_t t0l, uint8_t t1h, uint8_t t1l);
	void setPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue);
	void setPixel(uint16_t index, rgb_pixel pixel);
	void setPixel(uint16_t index, uint32_t pixel);
	void setHsbPixel(uint16_t index, float hue, float saturation, float brightness);
	void setHsbPixel(uint16_t index, hsb_pixel pixel);
	void clear();
	void show();
	virtual ~RgbStrip();
	rgb_pixel* pixels;
};

/**
 * RGBW strip class.
 */
class RgbwStrip:  public Strip {
public:
	RgbwStrip(gpio_num_t gpio_num, uint16_t pixel_count, int channel, uint8_t t0h, uint8_t t0l, uint8_t t1h, uint8_t t1l);
	void setPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue);
	void setPixel(uint16_t index, rgb_pixel pixel);
	void setPixel(uint16_t index, rgbw_pixel pixel);
	void setPixel(uint16_t index, uint32_t pixel);
	void setHsbPixel(uint16_t index, float hue, float saturation, float brightness);
	void setHsbPixel(uint16_t index, hsb_pixel pixel);
	void setPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
	void clear();
	void show();
	void setRgbToRgbwConverter(rgbw_pixel (*converter) (uint8_t, uint8_t, uint8_t));
	virtual ~RgbwStrip();
	rgbw_pixel* pixels;

private:
	rgbw_pixel (*rgb2rgbwConverter) (uint8_t, uint8_t, uint8_t);
};

/**
 * @brief WS2812 RGB strip implementation.
 */
class WS2812: public RgbStrip {
public:
	WS2812(gpio_num_t gpioNum, uint16_t pixelCount, int channel);
};

/**
 * @brief WS2815 RGB strip implementation.
 */
class WS2815: public RgbStrip {
public:
	WS2815(gpio_num_t gpioNum, uint16_t pixelCount, int channel);
};

/**
 * @brief SK6812 RGB strip implementation.
 */
class SK6812: public RgbStrip {
public:
	SK6812(gpio_num_t gpioNum, uint16_t pixelCount, int channel);
};

/**
 * @brief SK2812 RGBW strip implementation.
 */
class SK6812W: public RgbwStrip {
public:
	SK6812W(gpio_num_t gpioNum, uint16_t pixelCount, int channel);
};

#endif /* MAIN_LEDSTRIP_H_ */
