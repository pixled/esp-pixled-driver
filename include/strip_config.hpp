#ifndef STRIP_CONFIG_H
#define STRIP_CONFIG_H

#include "output.hpp"
#include "constants.hpp"

struct StripConfig {
	/**
	 * Base StripConfig constructor.
	 *
	 * Time constants are given in *nS*.
	 * Example for WS2812 :
	 * - t0h = 350
	 * - t0l = 800
	 * - t1h = 700
	 * - t1l = 600
	 *
	 * Each constant is then automatically converted to RMT ticks.
	 *
	 * @param t0h t0h in nS
	 * @param t0l t0l in nS
	 * @param t1h t1h in nS
	 * @param t1l t1l in nS
	 */
	StripConfig(uint16_t t0h,  uint16_t t0l,  uint16_t t1h,  uint16_t t1l) :
		t0h(NS_TO_RMT_TICKS(t0h)),
		t0l(NS_TO_RMT_TICKS(t0l)),
		t1h(NS_TO_RMT_TICKS(t1h)),
		t1l(NS_TO_RMT_TICKS(t1l)) {}
	/**
	 * t0h, in *RMT ticks*.
	 */
	uint16_t t0h;
	/**
	 * t0l, in *RMT ticks*.
	 */
	uint16_t t0l;
	/**
	 * t1h, in *RMT ticks*.
	 */
	uint16_t t1h;
	/**
	 * t1l, in *RMT ticks*.
	 */
	uint16_t t1l;
};

struct RgbStripConfig : public StripConfig {
	/**
	 * Base StripConfig constructor.
	 *
	 * Time constants are given in *nS*.
	 * Each constant is then automatically converted to RMT ticks.
	 *
	 * Predefined constants defined in constants.hpp can be used as
	 * RgbSerializer. (eg RGB, GBR, etc...)
	 *
	 * @param serializer defines output color order
	 * @param t0h t0h in nS
	 * @param t0l t0l in nS
	 * @param t1h t1h in nS
	 * @param t1l t1l in nS
	 */
	RgbStripConfig(RgbSerializer serializer, uint16_t t0h,  uint16_t t0l,  uint16_t t1h,  uint16_t t1l)
		: StripConfig(t0h, t0l, t1h, t1l), serializer(serializer) {}

	RgbSerializer serializer;
};

struct RgbwStripConfig : public StripConfig {
	/**
	 * Base StripConfig constructor.
	 *
	 * Time constants are given in *nS*.
	 * Each constant is then automatically converted to RMT ticks.
	 *
	 * Predefined constants defined in constants.hpp can be used as
	 * RgbSerializer. (eg RGBW, GBRW, etc...)
	 *
	 * @param serializer defines output color order
	 * @param t0h t0h in nS
	 * @param t0l t0l in nS
	 * @param t1h t1h in nS
	 * @param t1l t1l in nS
	 */
	RgbwStripConfig(RgbwSerializer serializer, uint16_t t0h,  uint16_t t0l,  uint16_t t1h,  uint16_t t1l)
		: StripConfig(t0h, t0l, t1h, t1l), serializer(serializer) {}

	RgbwSerializer serializer;
};

struct WS2812 : public RgbStripConfig {
	WS2812()
		: RgbStripConfig(
				GRB,
				WS2812_T0H,
				WS2812_T0L,
				WS2812_T1H,
				WS2812_T1L)
	{}
};

struct WS2815 : public RgbStripConfig {
	WS2815()
		: RgbStripConfig(
				GRB,
				WS2815_T0H,
				WS2815_T0L,
				WS2815_T1H,
				WS2815_T1L)
	{}
};

struct SK6812 : public RgbStripConfig {
	SK6812()
		: RgbStripConfig(
				GRB,
				SK6812_T0H,
				SK6812_T0L,
				SK6812_T1H,
				SK6812_T1L)
	{}
};

struct SK6812W : public RgbwStripConfig {
	SK6812W()
		: RgbwStripConfig(
				GRBW,
				SK6812W_T0H,
				SK6812W_T0L,
				SK6812W_T1H,
				SK6812W_T1L)
	{}
};
#endif
