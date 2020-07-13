#include "pixel.hpp"

namespace pixled {
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
	class HsbToRgbConverter {
		public:
			rgb_pixel operator()(const hsb_pixel&) const;
	};

	class RgbToRgbwConverter {
		public:
			virtual rgbw_pixel operator()(const rgb_pixel&) const = 0;
	};

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

	class SimpleRgbToRgbwConverter : public RgbToRgbwConverter {
		public:
			rgbw_pixel operator()(const rgb_pixel&) const;
	};

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
	class ComplexRgbToRgbwConverter : public RgbToRgbwConverter {
		public:
			rgbw_pixel operator()(const rgb_pixel&) const;
	};
}
