#include "pixled_driver.hpp"
#include "output.hpp"
#include "unity.h"

void test_serializer_rgb_output() {
	rgb_pixel pixel {238, 7, 12};
	RGB rgb;
	uint8_t output[3];

	rgb.serialize(pixel, output);

	TEST_ASSERT_EQUAL_UINT8(output[0], 238);
	TEST_ASSERT_EQUAL_UINT8(output[1], 7);
	TEST_ASSERT_EQUAL_UINT8(output[2], 12);
}

void test_serializer_grb_output() {
	rgb_pixel pixel {238, 7, 12};
	GRB grb;
	uint8_t output[3];

	grb.serialize(pixel, output);

	TEST_ASSERT_EQUAL_UINT8(output[0], 7);
	TEST_ASSERT_EQUAL_UINT8(output[1], 238);
	TEST_ASSERT_EQUAL_UINT8(output[2], 12);
}

void test_serializer_grbw_output() {
	rgbw_pixel pixel {238, 7, 12, 75};
	GRBW grbw;
	uint8_t output[4];

	grbw.serialize(pixel, output);

	TEST_ASSERT_EQUAL_UINT8(output[0], 7);
	TEST_ASSERT_EQUAL_UINT8(output[1], 238);
	TEST_ASSERT_EQUAL_UINT8(output[2], 12);
	TEST_ASSERT_EQUAL_UINT8(output[3], 75);
}
