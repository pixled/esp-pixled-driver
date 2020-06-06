#ifndef STRIP_CONFIG_H
#define STRIP_CONFIG_H

#include "output.hpp"
#include "constants.hpp"

struct StripConfig {
	StripConfig(uint8_t t0h,  uint8_t t0l,  uint8_t t1h,  uint8_t t1l)
		: t0h(t0h), t0l(t0l), t1h(t1h), t1l(t1l) {}
	uint8_t t0h;
	uint8_t t0l;
	uint8_t t1h;
	uint8_t t1l;
};

struct RgbStripConfig : public StripConfig {
	RgbStripConfig(RgbSerializer serializer, uint8_t t0h,  uint8_t t0l,  uint8_t t1h,  uint8_t t1l)
		: StripConfig(t0h, t0l, t1h, t1l), serializer(serializer) {}

	RgbSerializer serializer;
};

struct RgbwStripConfig : public StripConfig {

	RgbwStripConfig(RgbwSerializer serializer, uint8_t t0h,  uint8_t t0l,  uint8_t t1h,  uint8_t t1l)
		: StripConfig(t0h, t0l, t1h, t1l), serializer(serializer) {}

	RgbwSerializer serializer;
};

struct WS2812 : public RgbStripConfig {
	WS2812()
		: RgbStripConfig(
				GRB,
				WS2812_T0H * RMT_RATIO,
				WS2812_T0L * RMT_RATIO,
				WS2812_T1H * RMT_RATIO,
				WS2812_T1L * RMT_RATIO)
	{}
};

struct WS2815 : public RgbStripConfig {
	WS2815()
		: RgbStripConfig(
				GRB,
				WS2815_T0H * RMT_RATIO,
				WS2815_T0L * RMT_RATIO,
				WS2815_T1H * RMT_RATIO,
				WS2815_T1L * RMT_RATIO)
	{}
};

struct SK6812 : public RgbStripConfig {
	SK6812()
		: RgbStripConfig(
				GRB,
				SK6812_T0H * RMT_RATIO,
				SK6812_T0L * RMT_RATIO,
				SK6812_T1H * RMT_RATIO,
				SK6812_T1L * RMT_RATIO)
	{}
};

struct SK6812W : public RgbwStripConfig {
	SK6812W()
		: RgbwStripConfig(
				GRBW,
				SK6812W_T0H * RMT_RATIO,
				SK6812W_T0L * RMT_RATIO,
				SK6812W_T1H * RMT_RATIO,
				SK6812W_T1L * RMT_RATIO)
	{}
};
#endif
