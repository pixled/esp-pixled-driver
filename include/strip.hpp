#ifndef STRIP_H
#define STRIP_H

#include <driver/rmt.h>
#include <driver/gpio.h>

#include "esp_log.h"
#include "constants.hpp"
#include "pixel.hpp"
#include "output.hpp"
#include "converters.hpp"
#include "strip_config.hpp"

#define RGB_TO_RGBW_CONVERTER ComplexRgbToRgbwConverter

/**
 * @brief General and abstract led Strip class.
 */
class Strip {
	public:
		
		Strip(
			gpio_num_t gpio_num, uint16_t pixel_count, uint8_t* _buffer,
			rmt_channel_t channel, rmt_item32_t* rmt_items, StripConfig strip_config);

		uint16_t length() {return pixel_count;}
		virtual void show() = 0;

		virtual void setPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue) = 0;

		virtual void setHsbPixel(uint16_t index, float hue, float saturation, float brightness) = 0;
		virtual void setHsbPixel(uint16_t index, hsb_pixel pixel) = 0;

		uint8_t* const& buffer() {return _buffer;}
		virtual void clear() = 0;

		const StripConfig& stripConfig() const {return strip_config;}
		virtual ~Strip();

	protected:
		uint16_t pixel_count;
		uint8_t* _buffer;

		rmt_channel_t  channel;
		rmt_item32_t*  rmt_items;

		StripConfig strip_config;

		void setItem1(rmt_item32_t* pItem);
		void setItem0(rmt_item32_t* pItem);

		/*
		 * Add an RMT terminator into the RMT data.
		 */
		static void setTerminator(rmt_item32_t* pItem);
};

/**
 * @brief RGB strip class.
 */
class RgbStrip: public Strip {
	protected:
		RgbStripConfig rgb_strip_config;
		HsbToRgbConverter hsb_to_rgb;

	public:
		RgbStrip(gpio_num_t gpio_num, uint16_t pixel_count, rmt_channel_t channel, RgbStripConfig config);
		RgbStrip(const RgbStrip&) = delete;
		RgbStrip(RgbStrip&&) = delete;
		RgbStrip& operator=(const RgbStrip&) = delete;
		RgbStrip& operator=(RgbStrip&&) = delete;

		void show() override;

		void setPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue) override;

		void setHsbPixel(uint16_t index, float hue, float saturation, float brightness) override;
		void setHsbPixel(uint16_t index, hsb_pixel pixel) override;

		void clear() override;

		const RgbStripConfig& rgbStripConfig() const {return rgb_strip_config;}

		virtual ~RgbStrip();
};

/**
 * RGBW strip class.
 */
class RgbwStrip:  public Strip {
public:
	RgbwStrip(gpio_num_t gpio_num, uint16_t pixel_count, rmt_channel_t channel, RgbwStripConfig config);
	RgbwStrip(const RgbwStrip&) = delete;
	RgbwStrip(RgbwStrip&&) = delete;
	RgbwStrip& operator=(const RgbwStrip&) = delete;
	RgbwStrip& operator=(RgbwStrip&&) = delete;

	void show() override;

	void setPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue) override;
	void setPixel(uint16_t index, rgbw_pixel pixel);

	void setHsbPixel(uint16_t index, float hue, float saturation, float brightness) override;
	void setHsbPixel(uint16_t index, hsb_pixel pixel) override;

	void setPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white);

	void clear() override;

	const RgbwStripConfig& rgbwStripConfig() const {return rgbw_strip_config;}

	virtual ~RgbwStrip();

private:
	RgbwStripConfig rgbw_strip_config;
	HsbToRgbConverter hsb_to_rgb;
	RGB_TO_RGBW_CONVERTER rgb_to_rgbw;
};
#endif