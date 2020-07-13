#ifndef PIXLED_DRIVER_OUTPUT_H
#define PIXLED_DRIVER_OUTPUT_H

#include <cstdint>
#include "pixel.hpp"

namespace pixled {
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

			bool operator==(const RgbSerializer& other) const {
				return (R == other.R) && (G == other.G) && (B == other.B);
			}
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

			bool operator==(const RgbwSerializer& other) const {
				return (R == other.R) && (G == other.G) && (B == other.B) && (W == other.W);
			}
	};
}
#endif
