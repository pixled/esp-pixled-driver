#ifndef PIXLED_DRIVER_PIXEL_H
#define PIXLED_DRIVER_PIXEL_H

#include <cstdint>

namespace pixled {
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
}
#endif
