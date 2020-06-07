#include "sdkconfig.h"
#include "strip.hpp"

static const char* PIXLED_LOG_TAG = "PIXLED_DRIVER";

/*********/
/* Strip */
/*********/

/**
 * Sets up RMT strip value 1.
 *
 * Sets two levels of RMT output for a "1".
 * This is:
 * 	- a logic 1 during t1h
 * 	- a logic 0 during t1l
 *
 * t1h and t1l are retrieved from the current strip, depending
 * on its type.
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
 * Sets up RMT strip value 0.
 *
 * Sets two levels of RMT output for a "0".
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

/**
 * Adds an RMT terminator into the RMT data.
 */
void Strip::setTerminator(rmt_item32_t* pItem) {
	pItem->level0    = 0;
	pItem->duration0 = 0;
	pItem->level1    = 0;
	pItem->duration1 = 0;
} // setTerminator

/**
 * Strip constructor.
 *
 * Sets up all the RMT driver and other common parameters for every strip types.
 * The RMT channel specified is configured for this strip instance, and should
 * not be used for other purpose (including an other led strip).
 *
 * Because Strip is abstract, this constructor should not be used directly, but
 * will be call by implementing classes (RgbStrip, RgbwStrip).
 *
 * @param gpio_num Led Strip GPIO. See https://docs.espressif.com/projects/esp-idf/en/stable/api-reference/peripherals/gpio.html#_CPPv410gpio_num_t
 * @param pixel_count Number of leds.
 * @param channel RMT channel to use. See https://docs.espressif.com/projects/esp-idf/en/stable/api-reference/peripherals/rmt.html#_CPPv413rmt_channel_t
 * @param rmt_items dynamically allocated rmt buffer, according to the led type
 * and the strip length
 * @param config strip config, defined t0h, t0l, t1h and t1l
 *
 */
Strip::Strip(
		gpio_num_t gpio_num, uint16_t pixel_count, uint8_t* _buffer,
		rmt_channel_t channel, rmt_item32_t* rmt_items, StripConfig config)
	: pixel_count(pixel_count), _buffer(_buffer), channel(channel), rmt_items(rmt_items), strip_config(config) {
		rmt_config_t _rmt_config;
		_rmt_config.rmt_mode                  = RMT_MODE_TX;
		_rmt_config.channel                   = channel;
		_rmt_config.gpio_num                  = gpio_num;
		//_rmt_config.mem_block_num             = 8 - channel;
		_rmt_config.mem_block_num             = 1;
		_rmt_config.clk_div                   = 8;
		_rmt_config.tx_config.loop_en         = 0;
		_rmt_config.tx_config.carrier_en      = 0;
		_rmt_config.tx_config.idle_output_en  = 1;
		_rmt_config.tx_config.idle_level      = (rmt_idle_level_t) 0;
		_rmt_config.tx_config.carrier_freq_hz = 10000;
		_rmt_config.tx_config.carrier_level   = (rmt_carrier_level_t)1;
		_rmt_config.tx_config.carrier_duty_percent = 50;

		ESP_ERROR_CHECK(rmt_config(&_rmt_config));
		ESP_ERROR_CHECK(rmt_driver_install(channel, 0, 0));
} // Strip

/**
 * Strip instance destructor.
 *
 * Uninstalls the RMT driver from the RMT channel currently in use, so that it
 * can be re-used.
 */
Strip::~Strip() {
	delete[] this->rmt_items;
	ESP_ERROR_CHECK(rmt_driver_uninstall(channel));
} // ~Strip()

/************/
/* RgbStrip */
/************/

/**
 * RgbStrip constructor.
 *
 * Class used to drive RGB led strips.
 *
 * The `config` is used to define all the hardware specific parameters :
 * - t0h, t0l, t1h and t1l. See the StripConfig doc to learn how to manually
 *   define those parameters.
 * - RGB serializer : defines the output order (RGB, GBR, etc...)
 *
 * Predefined configurations are available for the following RGB LED strip models :
 * - WS2812
 * - WS2815
 * - SK6812
 *
 * Example usage :
 * ```
 * RgbStrip strip {GPIO_NUM_12, 20, RMT_CHANNEL_0, WS2812()};
 * ```
 *
 * @param gpio_num Led Strip GPIO. See https://docs.espressif.com/projects/esp-idf/en/stable/api-reference/peripherals/gpio.html#_CPPv410gpio_num_t
 * @param pixel_count Number of leds.
 * @param channel RMT channel to use. See https://docs.espressif.com/projects/esp-idf/en/stable/api-reference/peripherals/rmt.html#_CPPv413rmt_channel_t
 * @param config RGB strip config
 */
RgbStrip::RgbStrip(gpio_num_t gpio_num, uint16_t pixel_count, rmt_channel_t channel, RgbStripConfig config) :
	Strip(
		gpio_num, pixel_count, new uint8_t[pixel_count*3],
		channel, new rmt_item32_t[pixel_count * 24 + 1], config),
	rgb_strip_config(config)  {
		clear();
	};

/**
 * RgbStrip constructor with a default RMT channel set to RMT_CHANNEL_0.
 *
 * Same as RgbStrip(gpio_num, pixel_count, RMT_CHANNEL_0, config).
 * 
 * @param gpio_num Led Strip GPIO. See https://docs.espressif.com/projects/esp-idf/en/stable/api-reference/peripherals/gpio.html#_CPPv410gpio_num_t
 * @param pixel_count Number of leds.
 * @param config RGB strip config
 */
RgbStrip::RgbStrip(gpio_num_t gpio_num, uint16_t pixel_count, RgbStripConfig config) :
	RgbStrip(gpio_num, pixel_count, RMT_CHANNEL_0, config) {
};

/**
 * Transmits the current buffer to the RGB strip.
 */
void RgbStrip::show() {
	rmt_item32_t* pCurrentItem = this->rmt_items;

	for (uint16_t i = 0; i < this->pixel_count; i++) {
		uint32_t current_pixel =
				(_buffer[3*i] << 16) |
				(_buffer[3*i+1] << 8)  |
				_buffer[3*i+2];

		ESP_LOGD(PIXLED_LOG_TAG, "Pixel value: %x", current_pixel);
		for (int8_t j = 23; j >= 0; j--) {
			// We have 24 bits of data representing the red, green amd blue channels. The value of the
			// 24 bits to output is in the variable current_pixel.  We now need to stream this value
			// through RMT in most significant bit first.  To do this, we iterate through each of the 24
			// bits from MSB to LSB.
			if (current_pixel & (1 << j)) {
				setItem1(pCurrentItem);
			} else {
				setItem0(pCurrentItem);
			}
			pCurrentItem++;
		}
	}
	setTerminator(pCurrentItem); // Write the RMT terminator.

	// Show the pixels.
	ESP_ERROR_CHECK(rmt_write_items(this->channel, this->rmt_items, this->pixel_count * 24, 1 /*[> wait till done <]*/));
} // show

/**
 * Sets the value of the led at position `index` with the specified RGB values.
 *
 * The LEDs are not actually updated until a call to show().
 *
 * Data is directly written into the internal buffer, according to the current
 * output order.
 *
 * @param index position of the led
 * @param red red value, between 0 and 255.
 * @param green green value, between 0 and 255.
 * @param blue blue value, between 0 and 255.
 *
 */
void RgbStrip::setRgbPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue) {
	rgb_strip_config.serializer.serialize({red, green, blue}, &_buffer[index*3]);
} // setRgbPixel

/**
 * Sets the value of the led at position `index` with the specified HSB values.
 *
 * The LEDs are not actually updated until a call to show().
 *
 * Data is directly converted to RGB and written into the internal buffer,
 * according to the current output order.
 *
 * @param index The pixel that is to have its color set.
 * @param hue hue value between 0 and 360.
 * @param saturation saturation value, between 0 and 1.
 * @param brightness brightness value, between 0 and 1.
 */
void RgbStrip::setHsbPixel(uint16_t index, float hue, float saturation, float brightness) {
	rgb_strip_config.serializer.serialize(
			hsb_to_rgb({hue, saturation, brightness}),
			&_buffer[3*index]);
} // setHsbPixel

/**
 * Clears all the pixel colors.
 *
 * This sets all the pixels to off which is no brightness for all of the color channels.
 *
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
 * RgbStrip destructor.
 *
 * The internal buffer is deleted, and should not be accessed any more.
 */
RgbStrip::~RgbStrip() {
	delete[] this->_buffer;
}

/*************/
/* RgbwStrip */
/*************/

/**
 * RgbwStrip constructor.
 *
 * Class used to drive RGB led strips.
 *
 * The `config` is used to define all the hardware specific parameters :
 * - t0h, t0l, t1h and t1l. See the StripConfig doc to learn how to manually
 *   define those parameters.
 * - RGBW serializer : defines the output order (RGBW, GBRW, etc...)
 *
 * Predefined configurations are available for the following RGBW LED strip models :
 * - SK6812W
 *
 * Notice that the class inherits from the Strip functions, so does the
 * RgbStrip class. In consequence, the same code might be used to drive RGB or
 * RGBW strips using the common strip interface. The difference is that when
 * setRgbPixel is used on a RGBW strip, the RGB color is efficiently converted
 * to an RGBW color to take advantage of the additional white LED.
 *
 * Additionally, the setRgbwPixel can be used to manually control the white
 * LED.
 *
 * Example usage :
 * ```
 * RgbwStrip strip {GPIO_NUM_12, 20, RMT_CHANNEL_0, SK6812W()};
 * ```
 *
 * @param gpio_num Led Strip GPIO. See https://docs.espressif.com/projects/esp-idf/en/stable/api-reference/peripherals/gpio.html#_CPPv410gpio_num_t
 * @param pixel_count Number of leds.
 * @param channel RMT channel to use. See https://docs.espressif.com/projects/esp-idf/en/stable/api-reference/peripherals/rmt.html#_CPPv413rmt_channel_t
 * @param config RGBW strip config
 *
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
 * RgbwStrip constructor with a default RMT channel set to RMT_CHANNEL_0.
 *
 * Same as RgbwStrip(gpio_num, pixel_count, RMT_CHANNEL_0, config).
 * 
 * @param gpio_num Led Strip GPIO. See https://docs.espressif.com/projects/esp-idf/en/stable/api-reference/peripherals/gpio.html#_CPPv410gpio_num_t
 * @param pixel_count Number of leds.
 * @param config RGBW strip config
 */
RgbwStrip::RgbwStrip(gpio_num_t gpio_num, uint16_t pixel_count, RgbwStripConfig config)
	: RgbwStrip(gpio_num, pixel_count, RMT_CHANNEL_0, config) {}

/**
 * Transmits the current buffer to the RGBW strip.
 */
void RgbwStrip::show() {
	rmt_item32_t* pCurrentItem = this->rmt_items;

	for (uint16_t i = 0; i < this->pixel_count; i++) {
		uint32_t current_pixel =
				(_buffer[4*i] << 24) |
				(_buffer[4*i+1] << 16) |
				(_buffer[4*i+2] << 8) |
				_buffer[4*i+3];

		ESP_LOGD(PIXLED_LOG_TAG, "RGBW Pixel value: %x", current_pixel);
		for (int8_t j = 31; j >= 0; j--) {
			// We have 32 bits of data representing the red, green, blue and white channels. The value of the
			// 32 bits to output is in the variable current_pixel.  We now need to stream this value
			// through RMT in most significant bit first.  To do this, we iterate through each of the 32
			// bits from MSB to LSB.
			if (current_pixel & (1 << j)) {
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
 * Sets the value of the led at position `index` with the specified RGB values.
 *
 * The LEDs are not actually updated until a call to show().
 *
 * Data is directly converted to RGBW and written into the internal buffer,
 * according to the current output order.
 *
 * @param index position of the led
 * @param red red value, between 0 and 255.
 * @param green green value, between 0 and 255.
 * @param blue blue value, between 0 and 255.
 *
 */
void RgbwStrip::setRgbPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue) {
	rgbw_strip_config.serializer.serialize(rgb_to_rgbw({red, green, blue}), &_buffer[index*4]);
} // setRgbPixel

/**
 * Sets the value of the led at position `index` with the specified RGBW values.
 *
 * The LEDs are not actually updated until a call to show().
 *
 * Data is directly written into the internal buffer, according to the current
 * output order.
 *
 * @param index position of the led
 * @param red red value, between 0 and 255.
 * @param green green value, between 0 and 255.
 * @param blue blue value, between 0 and 255.
 * @param white white value, between 0 and 255.
 */
void RgbwStrip::setRgbwPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
	rgbw_strip_config.serializer.serialize({red, green, blue, white}, &_buffer[index*4]);
} // setRgbPixel

/**
 * Sets the value of the led at position `index` with the specified HSB values.
 *
 * The LEDs are not actually updated until a call to show().
 *
 * Data is directly converted from HSB to RGB, from RGB to RGBW, and written
 * into the internal buffer, according to the current output order.
 *
 * @param index The pixel that is to have its color set.
 * @param hue hue value between 0 and 360.
 * @param saturation saturation value, between 0 and 1.
 * @param brightness brightness value, between 0 and 1.
 */
void RgbwStrip::setHsbPixel(uint16_t index, float hue, float saturation, float brightness) {
	rgbw_strip_config.serializer.serialize(
			rgb_to_rgbw(hsb_to_rgb({hue, saturation, brightness})),
			&_buffer[index*4]
			);
} // setHsbPixel

/**
 * Clears all the pixel colors.
 *
 * This sets all the pixels to off which is no brightness for all of the color channels.
 *
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
 * RgbStrip destructor.
 *
 * The internal buffer is deleted, and should not be accessed any more.
 */
RgbwStrip::~RgbwStrip() {
	delete[] this->_buffer;
}
