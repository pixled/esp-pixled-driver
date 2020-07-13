#include "pixel.hpp"

namespace pixled {
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
}
