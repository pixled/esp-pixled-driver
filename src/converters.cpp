#include "converters.hpp"
#include <algorithm>

namespace pixled {
	rgb_pixel HsbToRgbConverter::operator()(const hsb_pixel& pixel) const {
		double      hh, p, q, t, ff;
		long        i;
		double      r_out;
		double      g_out;
		double 	    b_out;

		float hue = pixel.hue;
		float saturation = pixel.saturation;
		float brightness = pixel.brightness;

		if(saturation <= 0.0) {       // < is bogus, just shuts up warnings
			r_out = brightness;
			g_out = brightness;
			b_out = brightness;
			return rgb_pixel(r_out * 255, g_out * 255, b_out * 255);
		}
		hh = hue;
		if(hh >= 360.0) hh = 0.0;
		hh /= 60.0;
		i = (long)hh;
		ff = hh - i;
		p = brightness * (1.0 - saturation);
		q = brightness * (1.0 - (saturation * ff));
		t = brightness * (1.0 - (saturation * (1.0 - ff)));

		switch(i) {
			case 0:
				r_out = brightness;
				g_out = t;
				b_out = p;
				break;
			case 1:
				r_out = q;
				g_out = brightness;
				b_out = p;
				break;
			case 2:
				r_out = p;
				g_out = brightness;
				b_out = t;
				break;

			case 3:
				r_out = p;
				g_out = q;
				b_out = brightness;
				break;
			case 4:
				r_out = t;
				g_out = p;
				b_out = brightness;
				break;
			case 5:
			default:
				r_out = brightness;
				g_out = p;
				b_out = q;
				break;
		}

		return rgb_pixel(r_out * 255, g_out * 255, b_out * 255);
	}

	rgbw_pixel SimpleRgbToRgbwConverter::operator()(const rgb_pixel& pixel) const {
		uint8_t white = std::min({pixel.red, pixel.green, pixel.blue});
		return rgbw_pixel(
				pixel.red - white,
				pixel.green - white,
				pixel.blue - white,
				white
				);
	}

	rgbw_pixel ComplexRgbToRgbwConverter::operator()(const rgb_pixel& pixel) const {
		//Get the maximum between R, G, and B
		float tM = std::max({pixel.red, pixel.green, pixel.blue});

		//If the maximum value is 0, immediately return pure black.
		if(tM == 0)
		{ return rgbw_pixel(0, 0, 0, 0); }

		//This section serves to figure out what the color with 100% hue is
		float multiplier = 255.0f / tM;
		float hR = pixel.red * multiplier;
		float hG = pixel.green * multiplier;
		float hB = pixel.blue * multiplier;

		//This calculates the Whiteness (not strictly speaking Luminance) of the color
		float M = std::max(hR, std::max(hG, hB));
		float m = std::min(hR, std::min(hG, hB));
		float Luminance = ((M + m) / 2.0f - 127.5f) * (255.0f/127.5f) / multiplier;

		//Calculate the output values
		uint8_t Wo = Luminance;
		uint8_t  Bo = pixel.blue - Luminance;
		uint8_t  Ro = pixel.red - Luminance;
		uint8_t  Go = pixel.green - Luminance;

		return rgbw_pixel(Ro, Go, Bo, Wo);

	}
}
