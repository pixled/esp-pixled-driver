#include "sdkconfig.h"
#include "strip.hpp"

static const char* PIXLED_LOG_TAG = "PIXLED_DRIVER";

/*********/
/* Strip */
/*********/

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
void Strip::setTerminator(rmt_item32_t* pItem) {
	pItem->level0    = 0;
	pItem->duration0 = 0;
	pItem->level1    = 0;
	pItem->duration1 = 0;
} // setTerminator

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
Strip::Strip(
		gpio_num_t gpio_num, uint16_t pixel_count, uint8_t* _buffer,
		rmt_channel_t channel, rmt_item32_t* rmt_items, StripConfig& config)
	: pixel_count(pixel_count), _buffer(_buffer), channel(channel), rmt_items(rmt_items), strip_config(config) {

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
} // Strip

/**
 * @brief Strip instance destructor.
 */
Strip::~Strip() {
	delete[] this->rmt_items;
} // ~Strip()

/************/
/* RgbStrip */
/************/

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
RgbStrip::RgbStrip(gpio_num_t gpio_num, uint16_t pixel_count, rmt_channel_t channel, RgbStripConfig config) :
	Strip(
		gpio_num, pixel_count, new uint8_t[pixel_count*3],
		channel, new rmt_item32_t[pixel_count * 24 + 1], config),
	rgb_strip_config(config)  {
		clear();
	};

/**
 * @brief Show the current pixel data for RGB strips.
 *
 * Drive the LEDs with the values that were previously set.
 */
void RgbStrip::show() {
	auto pCurrentItem = this->rmt_items;

	for (uint16_t i = 0; i < this->pixel_count; i++) {
		//uint8_t rgb[3];
		//rgb_strip_config.serializer.serialize(this->pixels[i], rgb);
		uint32_t currentPixel =
				(_buffer[3*i] << 16) |
				(_buffer[3*i+1] << 8)  |
				_buffer[3*i+2];

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
	ESP_ERROR_CHECK(rmt_write_items(this->channel, this->rmt_items, this->pixel_count * 24, 1 /* wait till done */));
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
	rgb_strip_config.serializer.serialize({red, green, blue}, &_buffer[index*3]);
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
	rgb_strip_config.serializer.serialize(
			hsb_to_rgb({hue, saturation, brightness}),
			&_buffer[3*index]);
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
	rgb_strip_config.serializer.serialize(hsb_to_rgb(hsb_pixel), &_buffer[index*3]);
} // setHsbPixel

/**
 * @brief Clear all the pixel colors for an RGB strip.
 *
 * This sets all the pixels to off which is no brightness for all of the color channels.
 * The LEDs are not actually updated until a call to show().
 */
void RgbStrip::clear() {
	for (uint16_t i = 0; i < this->pixel_count; i++) {
		_buffer[3*i] = 0;
		_buffer[3*i+1] = 0;
		_buffer[3*i+2] = 0;
	}
} // clear

/**
 * @brief RgbStrip instance destructor.
 */
RgbStrip::~RgbStrip() {
	delete[] this->_buffer;
}

/*************/
/* RgbwStrip */
/*************/

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
RgbwStrip::RgbwStrip(gpio_num_t gpio_num, uint16_t pixel_count, rmt_channel_t channel, RgbwStripConfig config):
	Strip(
			gpio_num, pixel_count, new uint8_t[pixel_count*4],
			channel, new rmt_item32_t[pixel_count * 32 + 1], config),
	rgbw_strip_config(config),
	rgb_to_rgbw()
{
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
 * @brief Show the current pixel data for RGBW strips.
 *
 * Drive the LEDs with the values that were previously set.
 */
void RgbwStrip::show() {
	auto pCurrentItem = this->rmt_items;

	for (uint16_t i = 0; i < this->pixel_count; i++) {
		//uint8_t rgbw[4];
		//rgbw_strip_config.serializer.serialize(pixels[i], rgbw);
		uint32_t currentPixel =
				(_buffer[4*i] << 24) |
				(_buffer[4*i+1] << 16) |
				(_buffer[4*i+2] << 8) |
				_buffer[4*i+3];

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
	ESP_ERROR_CHECK(rmt_write_items(this->channel, this->rmt_items, this->pixel_count * 32, 1 /* wait till done */));
} // show

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
	rgbw_strip_config.serializer.serialize(rgb_to_rgbw({red, green, blue}), &_buffer[index*4]);
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
	rgbw_strip_config.serializer.serialize({red, green, blue, white}, &_buffer[index*4]);
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
	rgbw_strip_config.serializer.serialize(pixel, &_buffer[index*4]);
} // setPixel

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
	rgbw_strip_config.serializer.serialize(rgb_to_rgbw(
			hsb_to_rgb(hsb_pixel)
			),
			&_buffer[index*4]
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
	rgbw_strip_config.serializer.serialize(
			rgb_to_rgbw(hsb_to_rgb({hue, saturation, brightness})),
			&_buffer[index*4]
			);
} // setHsbPixel

/**
 * @brief Clear all the pixel colors for an RGBW strip.
 *
 * This sets all the pixels to off which is no brightness for all of the color channels.
 * The LEDs are not actually updated until a call to show().
 */
void RgbwStrip::clear() {
	for (uint16_t i = 0; i < this->pixel_count; i++) {
		_buffer[4*i] = 0;
		_buffer[4*i+1] = 0;
		_buffer[4*i+2] = 0;
		_buffer[4*i+3] = 0;
	}
} // clear

/**
 * @brief RgbwStrip instance destructor.
 */
RgbwStrip::~RgbwStrip() {
	delete[] this->_buffer;
}
