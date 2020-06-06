#include "test_strip.hpp"
#include "unity.h"

#include "strip.hpp"
#include "strip_config.hpp"
#include "constants.hpp"

void test_rgb_strip_set_rgb() {
	auto serializer = RGB;
	RgbStrip strip {GPIO_NUM_12, 10, RMT_CHANNEL_0, {serializer, 10, 10, 10, 10}};

	for(int i = 0; i < 10; i++) {
		strip.setRgbPixel(i, 10*i, 10*i+1, 10*i+2);
	}
	uint8_t* buffer = strip.buffer();
	for(int i = 0; i < strip.length(); i++) {
		TEST_ASSERT_EQUAL_UINT8(10*i, buffer[3*i]);
		TEST_ASSERT_EQUAL_UINT8(10*i+1, buffer[3*i+1]);
		TEST_ASSERT_EQUAL_UINT8(10*i+2, buffer[3*i+2]);
	}
}

void test_gbr_strip_set_rgb() {
	auto serializer = GBR;
	RgbStrip strip {GPIO_NUM_12, 10, RMT_CHANNEL_0, {serializer, 10, 10, 10, 10}};

	for(int i = 0; i < 10; i++) {
		strip.setRgbPixel(i, 10*i, 10*i+1, 10*i+2);
	}
	uint8_t* buffer = strip.buffer();
	for(int i = 0; i < strip.length(); i++) {
		TEST_ASSERT_EQUAL_UINT8(10*i+1, buffer[3*i]);
		TEST_ASSERT_EQUAL_UINT8(10*i+2, buffer[3*i+1]);
		TEST_ASSERT_EQUAL_UINT8(10*i, buffer[3*i+2]);
	}
}

void test_gbr_strip_set_hsb() {
	auto serializer = GBR;
	RgbStrip strip {GPIO_NUM_12, 10, RMT_CHANNEL_0, {serializer, 10, 10, 10, 10}};

	for(int i = 0; i < 10; i++) {
		strip.setHsbPixel(i, 10*i, 10*i+1, 10*i+2);
	}
	uint8_t* buffer = strip.buffer();
	for(int i = 0; i < strip.length(); i++) {
		hsb_pixel hsb (10*i, 10*i+1, 10*i+2);
		rgb_pixel rgb = HsbToRgbConverter()(hsb);

		TEST_ASSERT_EQUAL_UINT8(rgb.green, buffer[3*i]);
		TEST_ASSERT_EQUAL_UINT8(rgb.blue, buffer[3*i+1]);
		TEST_ASSERT_EQUAL_UINT8(rgb.red, buffer[3*i+2]);
	}
}

void test_rgbw_strip_set_rgbw() {
	auto serializer = RGBW;
	RgbwStrip strip {GPIO_NUM_12, 10, RMT_CHANNEL_0, {serializer, 10, 10, 10, 10}};

	for(int i = 0; i < 10; i++) {
		strip.setRgbwPixel(i, 10*i, 10*i+1, 10*i+2, 10*i+3);
	}
	uint8_t* buffer = strip.buffer();
	for(int i = 0; i < strip.length(); i++) {
		rgbw_pixel rgbw (10*i, 10*i+1, 10*i+2, 10*i+3);

		TEST_ASSERT_EQUAL_UINT8(rgbw.red, buffer[4*i]);
		TEST_ASSERT_EQUAL_UINT8(rgbw.green, buffer[4*i+1]);
		TEST_ASSERT_EQUAL_UINT8(rgbw.blue, buffer[4*i+2]);
		TEST_ASSERT_EQUAL_UINT8(rgbw.white, buffer[4*i+3]);
	}

}

void test_gbrw_strip_set_rgbw() {
	auto serializer = GBRW;
	RgbwStrip strip {GPIO_NUM_12, 10, RMT_CHANNEL_0, {serializer, 10, 10, 10, 10}};

	for(int i = 0; i < 10; i++) {
		strip.setRgbwPixel(i, 10*i, 10*i+1, 10*i+2, 10*i+3);
	}
	uint8_t* buffer = strip.buffer();
	for(int i = 0; i < strip.length(); i++) {
		rgbw_pixel rgbw (10*i, 10*i+1, 10*i+2, 10*i+3);

		TEST_ASSERT_EQUAL_UINT8(rgbw.green, buffer[4*i]);
		TEST_ASSERT_EQUAL_UINT8(rgbw.blue, buffer[4*i+1]);
		TEST_ASSERT_EQUAL_UINT8(rgbw.red, buffer[4*i+2]);
		TEST_ASSERT_EQUAL_UINT8(rgbw.white, buffer[4*i+3]);
	}

}

void test_gbrw_strip_set_rgb() {
	auto serializer = GBRW;
	RgbwStrip strip {GPIO_NUM_12, 10, RMT_CHANNEL_0, {serializer, 10, 10, 10, 10}};

	for(int i = 0; i < 10; i++) {
		strip.setRgbPixel(i, 10*i, 10*i+1, 10*i+2);
	}
	uint8_t* buffer = strip.buffer();
	for(int i = 0; i < strip.length(); i++) {
		rgb_pixel rgb (10*i, 10*i+1, 10*i+2);
		rgbw_pixel rgbw = RGB_TO_RGBW_CONVERTER()(rgb);

		TEST_ASSERT_EQUAL_UINT8(rgbw.green, buffer[4*i]);
		TEST_ASSERT_EQUAL_UINT8(rgbw.blue, buffer[4*i+1]);
		TEST_ASSERT_EQUAL_UINT8(rgbw.red, buffer[4*i+2]);
		TEST_ASSERT_EQUAL_UINT8(rgbw.white, buffer[4*i+3]);
	}

}

void test_gbrw_strip_set_hsb() {
	auto serializer = GBRW;
	RgbwStrip strip {GPIO_NUM_12, 10, RMT_CHANNEL_0, {serializer, 10, 10, 10, 10}};

	for(int i = 0; i < 10; i++) {
		strip.setHsbPixel(i, 10*i, 10*i+1, 10*i+2);
	}
	uint8_t* buffer = strip.buffer();
	for(int i = 0; i < strip.length(); i++) {
		hsb_pixel hsb (10*i, 10*i+1, 10*i+2);
		rgb_pixel rgb = HsbToRgbConverter()(hsb);
		rgbw_pixel rgbw = RGB_TO_RGBW_CONVERTER()(rgb);

		TEST_ASSERT_EQUAL_UINT8(rgbw.green, buffer[4*i]);
		TEST_ASSERT_EQUAL_UINT8(rgbw.blue, buffer[4*i+1]);
		TEST_ASSERT_EQUAL_UINT8(rgbw.red, buffer[4*i+2]);
		TEST_ASSERT_EQUAL_UINT8(rgbw.white, buffer[4*i+3]);
	}
}
