#include "unity.h"

#include "output.hpp"
#include "constants.hpp"

using namespace pixled;

void test_serializer_rgb_output() {
	rgb_pixel pixel {238, 7, 12};
	auto rgb = RGB;
	uint8_t output[3];

	rgb.serialize(pixel, output);

	TEST_ASSERT_EQUAL_UINT8(output[0], 238);
	TEST_ASSERT_EQUAL_UINT8(output[1], 7);
	TEST_ASSERT_EQUAL_UINT8(output[2], 12);
}

void test_serializer_grb_output() {
	rgb_pixel pixel {238, 7, 12};
	auto grb = GRB;
	uint8_t output[3];

	grb.serialize(pixel, output);

	TEST_ASSERT_EQUAL_UINT8(output[0], 7);
	TEST_ASSERT_EQUAL_UINT8(output[1], 238);
	TEST_ASSERT_EQUAL_UINT8(output[2], 12);
}

void test_serializer_grbw_output() {
	rgbw_pixel pixel {238, 7, 12, 75};
	auto grbw = GRBW;
	uint8_t output[4];

	grbw.serialize(pixel, output);

	TEST_ASSERT_EQUAL_UINT8(output[0], 7);
	TEST_ASSERT_EQUAL_UINT8(output[1], 238);
	TEST_ASSERT_EQUAL_UINT8(output[2], 12);
	TEST_ASSERT_EQUAL_UINT8(output[3], 75);
}
