#include "sdkconfig.h"
#include "pixled_driver.hpp"

#include <algorithm>
#include <cstring>

#if CONFIG_CXX_EXCEPTIONS != 1
#error "C++ exception handling must be enabled within make menuconfig. See Compiler Options > Enable C++ Exceptions."
#endif


/*#ifdef CONFIG_RGB_TO_RGBW_1*/
	//#define RGB_TO_RGBW_1 CONFIG_RGB_TO_RGBW_1
//#else
	//#define RGB_TO_RGBW ComplexRgbToRgbwConverter
//#endif

//#ifdef CONFIG_RGB_TO_RGBW_2
	//#define RGB_TO_RGBW_2 CONFIG_RGB_TO_RGBW_2
//#else
	//#define RGB_TO_RGBW 
/*#endif*/

/**
* @brief rgb_pixel constructor.
*
* @param red	between 0 and 255
* @param green	between 0 and 255
* @param blue	between 0 and 255
*
*/
 rgb_pixel::rgb_pixel(uint8_t red, uint8_t green, uint8_t blue){
	 this->red = red;
	 this->green = green;
	 this->blue = blue;
 }

 /**
 * @brief rgb_pixel default constructor.
 *
 */
 rgb_pixel::rgb_pixel(){
	 rgb_pixel(0, 0, 0);
 }

 /**
 * @brief rgbw_pixel constructor.
 *
 * @param red between 0 and 255
 * @param green between 0 and 255
 * @param blue between 0 and 255
 * @param white between 0 and 255
 *
 */
 rgbw_pixel::rgbw_pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t white){
	 this->red = red;
	 this->green = green;
	 this->blue = blue;
	 this->white = white;
 }

 /**
 * @brief rgbw_pixel default constructor.
 */
 rgbw_pixel::rgbw_pixel(){
	 rgbw_pixel(0, 0, 0, 0);
 }

/**
 * @brief hsb_pixel constructor
 *
 * @param hue between 0 and 360
 * @param saturation between 0 and 1
 * @param brightness between 0 and 1
 */
hsb_pixel::hsb_pixel(float hue, float saturation, float brightness)
{
	this->hue = hue;
	this->saturation = saturation;
	this->brightness = brightness;
}

/**
 * @brief hsb_pixel default constructor.
 */
hsb_pixel::hsb_pixel()
{
	hsb_pixel(0, 0, 0);
}

/**
 * @brief Set up RMT strip value 1.
 *
 * Set two levels of RMT output to the Neopixel value for a "1".
 * This is:
 * 	- a logic 1 during t1h
 * 	- a logic 0 during t1l
 *
 * t1h and t1l are retrieved from the current strip, depending
 * on its model.
 *
 * @param pItem rmt item to set to 1
 */
void Strip::setItem1(rmt_item32_t* pItem) {
	pItem->level0    = 1;
	pItem->duration0 = strip_config.t1h;
	pItem->level1    = 0;
	pItem->duration1 = strip_config.t1l;
} // setItem1



/**
 * @brief Set up RMT strip value 0.
 *
 * Set two levels of RMT output to the Neopixel value for a "0".
 * This is:
 * a logic 1 for t0h
 * a logic 0 for t0l
 *
 * t1h and t1l are retrieved from the current strip, depending
 * on its model.
 *
 * @param pItem rmt item to set to 0
 */
void Strip::setItem0(rmt_item32_t* pItem) {
	pItem->level0    = 1;
	pItem->duration0 = strip_config.t0h;
	pItem->level1    = 0;
	pItem->duration1 = strip_config.t0l;
} // setItem0


/*
 * Add an RMT terminator into the RMT data.
 */
static void setTerminator(rmt_item32_t* pItem) {
	pItem->level0    = 0;
	pItem->duration0 = 0;
	pItem->level1    = 0;
	pItem->duration1 = 0;
} // setTerminator


/**
 * @brief RgbStrip constructor.
 *
 * Notice that several strips can be connected, but they must be connected on different RMT channels.
 * You shouldn't need to call this class directly, but you can use it to drive
 * other led strips not currently supported, with custom duration values.
 *
 * @param gpioNum Led Strip GPIO.
 * @param pixel_count Number of leds.
 * @param channel RMT channel to use. See https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/peripherals/rmt.html#enumerations
 * @param t0h high value duration for bit 0, expressed in RMT ticks.
 * @param t0l low value duration for bit 0, expressed in RMT ticks.
 * @param t1h high value duration for bit 1, expressed in RMT ticks.
 * @param t1l low value duration for bit 0, expressed in RMT ticks.
 */
RgbStrip::RgbStrip(gpio_num_t gpioNum, uint16_t pixel_count, int channel, RgbStripConfig& config):
	Strip(gpioNum, pixel_count, channel, config), rgb_strip_config(config), pixels(new rgb_pixel[pixel_count])  {
		this->items      = new rmt_item32_t[pixel_count * 24 + 1];
		clear();
	};


/**
* @brief RgbwStrip constructor.
*
* Notice that several strips can be connected, but they must be connected on different RMT channels.
* You shouldn't need to call this class directly, but you can use it to drive
* other led strips not currently supported, with custom duration values.
*
* @param gpioNum Led Strip GPIO.
* @param pixel_count Number of leds.
* @param channel RMT channel to use. See https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/peripherals/rmt.html#enumerations
* @param t0h high value duration for bit 0, expressed in RMT ticks.
* @param t0l low value duration for bit 0, expressed in RMT ticks.
* @param t1h high value duration for bit 1, expressed in RMT ticks.
* @param t1l low value duration for bit 0, expressed in RMT ticks.
*/
RgbwStrip::RgbwStrip(gpio_num_t gpioNum, uint16_t pixel_count, int channel, RgbwStripConfig& config):
	Strip(gpioNum, pixel_count, channel, config), rgbw_strip_config(config),
	rgb_to_rgbw(), pixels(new rgbw_pixel[pixel_count])
{
	this->items = new rmt_item32_t[pixel_count * 32 + 1];
		/*
		 *if (RGB_TO_RGBW_1) {
		 *    ESP_LOGI(PIXLED_LOG_TAG, "RGB_TO_RGBW_1 selected");
		 *    this->rgb2rgbwConverter = RGBtoRGBW1;
		 *}
		 *else if (RGB_TO_RGBW_2) {
		 *    ESP_LOGI(PIXLED_LOG_TAG, "RGB_TO_RGBW_2 selected");
		 *    this->rgb2rgbwConverter = RGBtoRGBW2;
		 *}
		 */
		clear();
	};


/**
 * @brief Construct a wrapper for the pixels.
 *
 * Set up all the RMT driver and other common parameters for every strip types.

 * Because Strip is abstract, this constructor should not be used directly, but
 * will be call by implementing classes (WS2812, SK6812W...) with correct time
 * values.
 *
 * @param gpioNum Led Strip GPIO.
 * @param pixel_count Number of leds.
 * @param channel RMT channel to use. See https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/peripherals/rmt.html#enumerations
 * @param t0h high value duration for bit 0, expressed in RMT ticks.
 * @param t0l low value duration for bit 0, expressed in RMT ticks.
 * @param t1h high value duration for bit 1, expressed in RMT ticks.
 * @param t1l low value duration for bit 0, expressed in RMT ticks.
 *
 */
Strip::Strip(gpio_num_t gpio_num, uint16_t pixel_count, int channel, StripConfig& config)
	: strip_config(config) {
	this->pixel_count = pixel_count;
	this->channel    = (rmt_channel_t) channel;

	rmt_config_t _rmt_config;
	_rmt_config.rmt_mode                  = RMT_MODE_TX;
	_rmt_config.channel                   = this->channel;
	_rmt_config.gpio_num                  = gpio_num;
	_rmt_config.mem_block_num             = 8 - this->channel;
	_rmt_config.clk_div                   = 8;
	_rmt_config.tx_config.loop_en         = 0;
	_rmt_config.tx_config.carrier_en      = 0;
	_rmt_config.tx_config.idle_output_en  = 1;
	_rmt_config.tx_config.idle_level      = (rmt_idle_level_t) 0;
	_rmt_config.tx_config.carrier_freq_hz = 10000;
	_rmt_config.tx_config.carrier_level   = (rmt_carrier_level_t)1;
	_rmt_config.tx_config.carrier_duty_percent = 50;

	ESP_ERROR_CHECK(rmt_config(&_rmt_config));
	ESP_ERROR_CHECK(rmt_driver_install(this->channel, 0, 0));
} // Strip


/**
 * @brief Show the current pixel data for RGB strips.
 *
 * Drive the LEDs with the values that were previously set.
 */
void RgbStrip::show() {
	auto pCurrentItem = this->items;

	for (uint16_t i = 0; i < this->pixel_count; i++) {
		uint8_t rgb[3];
		rgb_strip_config.serializer.serialize(this->pixels[i], rgb);
		uint32_t currentPixel =
				(rgb[0] << 16) |
				(rgb[1] << 8)  |
				rgb[2];

		ESP_LOGD(PIXLED_LOG_TAG, "Pixel value: %x", currentPixel);
		for (int8_t j = 23; j >= 0; j--) {
			// We have 24 bits of data representing the red, green amd blue channels. The value of the
			// 24 bits to output is in the variable current_pixel.  We now need to stream this value
			// through RMT in most significant bit first.  To do this, we iterate through each of the 24
			// bits from MSB to LSB.
			if (currentPixel & (1 << j)) {
				setItem1(pCurrentItem);
			} else {
				setItem0(pCurrentItem);
			}
			pCurrentItem++;
		}
	}
	setTerminator(pCurrentItem); // Write the RMT terminator.

	// Show the pixels.
	ESP_ERROR_CHECK(rmt_write_items(this->channel, this->items, this->pixel_count * 24, 1 /* wait till done */));
} // show

/**
 * @brief Show the current pixel data for RGBW strips.
 *
 * Drive the LEDs with the values that were previously set.
 */
void RgbwStrip::show() {
	auto pCurrentItem = this->items;

	for (uint16_t i = 0; i < this->pixel_count; i++) {
		uint8_t rgbw[4];
		rgbw_strip_config.serializer.serialize(pixels[i], rgbw);
		uint32_t currentPixel =
				(rgbw[0] << 24) |
				(rgbw[1] << 16) |
				(rgbw[2] << 8) |
				rgbw[3];

		ESP_LOGD(PIXLED_LOG_TAG, "Pixel value: %x", currentPixel);
		for (int8_t j = 31; j >= 0; j--) {
			// We have 32 bits of data representing the red, green, blue and white channels. The value of the
			// 32 bits to output is in the variable current_pixel.  We now need to stream this value
			// through RMT in most significant bit first.  To do this, we iterate through each of the 32
			// bits from MSB to LSB.
			if (currentPixel & (1 << j)) {
				setItem1(pCurrentItem);
			} else {
				setItem0(pCurrentItem);
			}
			pCurrentItem++;
		}
	}
	setTerminator(pCurrentItem); // Write the RMT terminator.

	// Show the pixels.
	ESP_ERROR_CHECK(rmt_write_items(this->channel, this->items, this->pixel_count * 32, 1 /* wait till done */));
} // show


/**
 * @brief Set the given pixel to the specified color for RGB strips.
 *
 * The LEDs are not actually updated until a call to show().
 *
 * @param index The pixel that is to have its color set.
 * @param red The amount of red in the pixel, between 0 and 255.
 * @param green The amount of green in the pixel, between 0 and 255.
 * @param blue The amount of blue in the pixel, between 0 and 255.
 *
 */
void RgbStrip::setPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue) {
	this->pixels[index].red   = red;
	this->pixels[index].green = green;
	this->pixels[index].blue  = blue;
} // setPixel

/**
 * @brief Set the given pixel to the specified color for RGBW strips.
 *
 * The RGBtoRGBW converter is used to convert from RGB to RGBW.
 *
 * The LEDs are not actually updated until a call to show().
 *
 * @param index The pixel that is to have its color set.
 * @param red The amount of red in the pixel, between 0 and 255.
 * @param green The amount of green in the pixel, between 0 and 255.
 * @param blue The amount of blue in the pixel, between 0 and 255.
 *
 */
void RgbwStrip::setPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue) {
	this->pixels[index] = this->rgb_to_rgbw({red, green, blue});
} // setPixel

/**
 * @brief Set the given pixel to the specified color for RGBW strips.
 *
 * The RGBtoRGBW converter is used to convert from RGB to RGBW.
 *
 * The LEDs are not actually updated until a call to show().
 *
 * @param index The pixel that is to have its color set.
 * @param pixel rgb pixel
 */
void RgbwStrip::setPixel(uint16_t index, rgb_pixel pixel) {
	this->pixels[index] = this->rgb_to_rgbw(pixel);
} // setPixel

/**
 * @brief Set the given pixel to the specified color for RGBW strips.
 *
 * The LEDs are not actually updated until a call to show().
 *
 * @param index The pixel that is to have its color set.
 * @param red The amount of red in the pixel, between 0 and 255.
 * @param green The amount of green in the pixel, between 0 and 255.
 * @param blue The amount of blue in the pixel, between 0 and 255.
 * @param white The amount of white in the pixel, between 0 and 255.
 */
void RgbwStrip::setPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
	this->pixels[index].red   = red;
	this->pixels[index].green = green;
	this->pixels[index].blue  = blue;
	this->pixels[index].white = white;
} // setPixel



/**
 * @brief Set the given pixel to the specified color for RGB strip.
 *
 * The LEDs are not actually updated until a call to show().
 *
 * @param index The pixel that is to have its color set.
 * @param pixel The RGB color value of the pixel.
 */
void RgbStrip::setPixel(uint16_t index, rgb_pixel pixel) {
	this->pixels[index] = pixel;
} // setPixel

/**
 * @brief Set the given pixel to the specified color for RGBW strip.
 *
 * The LEDs are not actually updated until a call to show().
 *
 * @param index The pixel that is to have its color set.
 * @param pixel The RGBW color value of the pixel.
 */
void RgbwStrip::setPixel(uint16_t index, rgbw_pixel pixel) {
	this->pixels[index] = pixel;
} // setPixel


/**
 * @brief Set the given pixel to the specified color for an RGB strip.
 *
 * The LEDs are not actually updated until a call to show().
 *
 * @param index The pixel that is to have its color set.
 * @param pixel The RGB color value of the pixel.
 */
void RgbStrip::setPixel(uint16_t index, uint32_t pixel) {
	this->pixels[index].red   = pixel & 0xff;
	this->pixels[index].green = (pixel & 0xff00) >> 8;
	this->pixels[index].blue  = (pixel & 0xff0000) >> 16;
} // setPixel

/**
 * @brief Set the given pixel to the specified color for an RGBW strip.
 *
 * The LEDs are not actually updated until a call to show().
 *
 * @param index The pixel that is to have its color set.
 * @param pixel The RGBW color value of the pixel.
 */
void RgbwStrip::setPixel(uint16_t index, uint32_t pixel) {
	this->setPixel(index, rgbw_pixel(
		pixel & 0xff,
		(pixel & 0xff00) >> 8,
		(pixel & 0xff0000) >> 16,
		(pixel & 0xff000000) >> 24
	));
} // setPixel

/**
 * @brief Set the given pixel to the specified HSB color for an RGB strip.
 *
 * The LEDs are not actually updated until a call to show().
 *
 * @param index The pixel that is to have its color set.
 * @param hue The amount of hue in the pixel (0-360).
 * @param saturation The amount of saturation in the pixel (0-1).
 * @param brightness The amount of brightness in the pixel (0-1).
 */
void RgbStrip::setHsbPixel(uint16_t index, float hue, float saturation, float brightness) {
	this->pixels[index] = hsb_to_rgb(hue, saturation, brightness);
} // setHsbPixel

/**
 * @brief Set the given pixel to the specified HSB color for an RGB strip.
 *
 * The LEDs are not actually updated until a call to show().
 *
 * @param index The pixel that is to have its color set.
 * @param hsb_pixel HSB colors.
 */
void RgbStrip::setHsbPixel(uint16_t index, hsb_pixel hsb_pixel) {
	this->pixels[index] = hsb_to_rgb(hsb_pixel.hue, hsb_pixel.saturation, hsb_pixel.brightness);
} // setHsbPixel

/**
 * @brief Set the given pixel to the specified HSB color for an RGBW strip.
 *
 * Compute the RGB value from HSB, and then the RGBW value from RGB.
 *
 * The LEDs are not actually updated until a call to show().
 *
 * @param index The pixel that is to have its color set.
 * @param hsb_pixel HSB colors.
 */
void RgbwStrip::setHsbPixel(uint16_t index, hsb_pixel hsb_pixel) {
	assert(index < pixel_count);

	this->pixels[index] = rgb_to_rgbw(
			hsb_to_rgb(hsb_pixel.hue, hsb_pixel.saturation, hsb_pixel.brightness)
			);
} // setHsbPixel


/**
 * @brief Set the given pixel to the specified HSB color for an RGBW strip.
 *
 * Compute the RGB value from HSB, and then the RGBW value from RGB.
 *
 * The LEDs are not actually updated until a call to show().
 *
 * @param index The pixel that is to have its color set.
 * @param hue The amount of hue in the pixel (0-360).
 * @param saturation The amount of saturation in the pixel (0-1).
 * @param brightness The amount of brightness in the pixel (0-1).
 */
void RgbwStrip::setHsbPixel(uint16_t index, float hue, float saturation, float brightness) {
	assert(index < pixel_count);

	this->pixels[index] = this->rgb_to_rgbw(hsb_to_rgb(hue, saturation, brightness));
} // setHsbPixel


/**
 * @brief Clear all the pixel colors for an RGB strip.
 *
 * This sets all the pixels to off which is no brightness for all of the color channels.
 * The LEDs are not actually updated until a call to show().
 */
void RgbStrip::clear() {
	for (uint16_t i = 0; i < this->pixel_count; i++) {
		this->pixels[i].red   = 0;
		this->pixels[i].green = 0;
		this->pixels[i].blue  = 0;
	}
} // clear

/**
 * @brief Clear all the pixel colors for an RGBW strip.
 *
 * This sets all the pixels to off which is no brightness for all of the color channels.
 * The LEDs are not actually updated until a call to show().
 */
void RgbwStrip::clear() {
	for (uint16_t i = 0; i < this->pixel_count; i++) {
		this->pixels[i].red   = 0;
		this->pixels[i].green = 0;
		this->pixels[i].blue  = 0;
		this->pixels[i].white = 0;
	}
} // clear


/**
 * @brief Strip instance destructor.
 */
Strip::~Strip() {
	delete this->items;
} // ~Strip()

/**
 * @brief RgbStrip instance destructor.
 */
RgbStrip::~RgbStrip() {
	delete[] this->pixels;
}

/**
 * @brief RgbwStrip instance destructor.
 */
RgbwStrip::~RgbwStrip() {
	delete[] this->pixels;
}
