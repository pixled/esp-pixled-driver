#include "test_output.hpp"
#include "test_strip.hpp"
#include "unity.h"
#include "pixled_driver.hpp"

extern "C" void app_main() {
	UNITY_BEGIN();

	printf("\n> Running esp-pixled-driver test suite\n\n");
	printf(">> Testing output serializers\n");

	RUN_TEST(test_serializer_rgb_output);
	RUN_TEST(test_serializer_grb_output);
	RUN_TEST(test_serializer_grbw_output);

	pixled_driver::init(RMT_CHANNEL_0);

	printf("\n>> Testing rgb strip\n");
	RUN_TEST(test_rgb_strip_set_rgb);

	RUN_TEST(test_gbr_strip_set_rgb);
	RUN_TEST(test_gbr_strip_set_hsb);

	printf("\n>> Testing rgbw strip\n");
	RUN_TEST(test_rgbw_strip_set_rgbw);

	RUN_TEST(test_gbrw_strip_set_rgbw);
	RUN_TEST(test_gbrw_strip_set_rgb);
	RUN_TEST(test_gbrw_strip_set_hsb);

	UNITY_END();
}
