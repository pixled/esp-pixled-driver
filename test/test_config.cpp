#include "test_config.hpp"
#include "unity.h"

#include "strip_config.hpp"
#include "strip.hpp"

using namespace pixled;

template<typename Config>
void test_rgb(RgbSerializer serializer, uint32_t t0h, uint32_t t0l, uint32_t t1h, uint32_t t1l) {
	RgbStrip rgb_strip(GPIO_NUM_12, 50, RMT_CHANNEL_0, Config());

	const StripConfig& strip_config = rgb_strip.stripConfig();
	const RgbStripConfig& config = rgb_strip.rgbStripConfig();

	TEST_ASSERT(config.serializer == serializer);
	TEST_ASSERT_EQUAL_UINT32(config.t0h, t0h * RMT_RATIO);
	TEST_ASSERT_EQUAL_UINT32(config.t0l, t0l * RMT_RATIO);
	TEST_ASSERT_EQUAL_UINT32(config.t1h, t1h * RMT_RATIO);
	TEST_ASSERT_EQUAL_UINT32(config.t1l, t1l * RMT_RATIO);

	TEST_ASSERT_EQUAL_UINT32(strip_config.t0h, t0h * RMT_RATIO);
	TEST_ASSERT_EQUAL_UINT32(strip_config.t0l, t0l * RMT_RATIO);
	TEST_ASSERT_EQUAL_UINT32(strip_config.t1h, t1h * RMT_RATIO);
	TEST_ASSERT_EQUAL_UINT32(strip_config.t1l, t1l * RMT_RATIO);
}

void test_ws2812() {
	test_rgb<WS2812>(GRB, WS2812_T0H, WS2812_T0L, WS2812_T1H, WS2812_T1L);
}

void test_ws2815() {
	test_rgb<WS2815>(GRB, WS2815_T0H, WS2815_T0L, WS2815_T1H, WS2815_T1L);
}

void test_sk6812() {
	test_rgb<SK6812>(GRB, SK6812_T0H, SK6812_T0L, SK6812_T1H, SK6812_T1L);
}

template<typename Config>
void test_rgbw(RgbwSerializer serializer, uint32_t t0h, uint32_t t0l, uint32_t t1h, uint32_t t1l) {
	RgbwStrip rgbw_strip(GPIO_NUM_12, 50, RMT_CHANNEL_0, Config());

	const StripConfig& strip_config = rgbw_strip.stripConfig();
	const RgbwStripConfig& config = rgbw_strip.rgbwStripConfig();

	TEST_ASSERT(config.serializer == serializer);
	TEST_ASSERT_EQUAL_UINT32(config.t0h, t0h * RMT_RATIO);
	TEST_ASSERT_EQUAL_UINT32(config.t0l, t0l * RMT_RATIO);
	TEST_ASSERT_EQUAL_UINT32(config.t1h, t1h * RMT_RATIO);
	TEST_ASSERT_EQUAL_UINT32(config.t1l, t1l * RMT_RATIO);

	TEST_ASSERT_EQUAL_UINT32(strip_config.t0h, t0h * RMT_RATIO);
	TEST_ASSERT_EQUAL_UINT32(strip_config.t0l, t0l * RMT_RATIO);
	TEST_ASSERT_EQUAL_UINT32(strip_config.t1h, t1h * RMT_RATIO);
	TEST_ASSERT_EQUAL_UINT32(strip_config.t1l, t1l * RMT_RATIO);
}

void test_sk6812w() {
	test_rgbw<SK6812W>(GRBW, SK6812_T0H, SK6812_T0L, SK6812_T1H, SK6812_T1L);
}
