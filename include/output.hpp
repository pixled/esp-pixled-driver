#ifndef PIXLED_DRIVER_OUTPUT_H
#define PIXLED_DRIVER_OUTPUT_H

#include <cstdint>
#include "pixel.hpp"

class RgbSerializer {
	private:
		int R;
		int G;
		int B;
	public:
		RgbSerializer(int R, int G, int B)
			: R(R), G(G), B(B) {}

		void serialize(const rgb_pixel& pixel, uint8_t* const& output) const {
			output[R] = pixel.red;
			output[G] = pixel.green;
			output[B] = pixel.blue;
		};
};

class RgbwSerializer {
	private:
		int R;
		int G;
		int B;
		int W;
	public:
		RgbwSerializer(int R, int G, int B, int W)
			: R(R), G(G), B(B), W(W) {}

		void serialize(const rgbw_pixel& pixel, uint8_t* const& output) const {
			output[R] = pixel.red;
			output[G] = pixel.green;
			output[B] = pixel.blue;
			output[W] = pixel.white;
		}
};
#endif
