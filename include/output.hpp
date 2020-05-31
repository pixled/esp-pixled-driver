#ifndef PIXLED_DRIVER_OUTPUT_H
#define PIXLED_DRIVER_OUTPUT_H

#include <cstdint>
#include "pixel.hpp"

class RgbSerializerApi {
	public:
		virtual void serialize(const rgb_pixel& pixel, uint8_t* const&) const = 0;
};

template<int R, int G, int B>
class RgbSerializer : public RgbSerializerApi {
	public:
		void serialize(const rgb_pixel& pixel, uint8_t* const& output) const override {
			output[R] = pixel.red;
			output[G] = pixel.green;
			output[B] = pixel.blue;
		};
};

using RGB = RgbSerializer<0, 1, 2>;
using RBG = RgbSerializer<0, 2, 1>;
using GRB = RgbSerializer<1, 0, 2>;
using GBR = RgbSerializer<2, 0, 1>;
using BRG = RgbSerializer<1, 2, 0>;
using BGR = RgbSerializer<2, 1, 0>;

class RgbwSerializerApi {
	public:
		virtual void serialize(const rgbw_pixel& pixel, uint8_t* const&) const = 0;
};

template<int r, int g, int b, int w>
class RgbwSerializer : public RgbwSerializerApi {
	public:
		void serialize(const rgbw_pixel& pixel, uint8_t* const& output) const override {
			output[r] = pixel.red;
			output[g] = pixel.green;
			output[b] = pixel.blue;
			output[w] = pixel.white;
		}
};

using WRGB = RgbwSerializer<1, 2, 3, 0>;
using WRBG = RgbwSerializer<1, 3, 2, 0>;
using WGRB = RgbwSerializer<2, 1, 3, 0>;
using WGBR = RgbwSerializer<3, 1, 2, 0>;
using WBRG = RgbwSerializer<2, 3, 1, 0>;
using WBGR = RgbwSerializer<3, 2, 1, 0>;

using RWGB = RgbwSerializer<0, 2, 3, 1>;
using RWBG = RgbwSerializer<0, 3, 2, 1>;
using GWRB = RgbwSerializer<2, 0, 3, 1>;
using GWBR = RgbwSerializer<3, 0, 2, 1>;
using BWRG = RgbwSerializer<2, 3, 0, 1>;
using BWGR = RgbwSerializer<3, 2, 0, 1>;

using RGWB = RgbwSerializer<0, 1, 3, 2>;
using RBWG = RgbwSerializer<0, 3, 1, 2>;
using GRWB = RgbwSerializer<1, 0, 3, 2>;
using GBWR = RgbwSerializer<3, 0, 1, 2>;
using BRWG = RgbwSerializer<1, 3, 0, 2>;
using BGWR = RgbwSerializer<3, 1, 0, 2>;

using RGBW = RgbwSerializer<0, 1, 2, 3>;
using RBGW = RgbwSerializer<0, 2, 1, 3>;
using GRBW = RgbwSerializer<1, 0, 2, 3>;
using GBRW = RgbwSerializer<2, 0, 1, 3>;
using BRGW = RgbwSerializer<1, 2, 0, 3>;
using BGRW = RgbwSerializer<2, 1, 0, 3>;
#endif
