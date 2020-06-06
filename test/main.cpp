#include "test_output.hpp"
#include "test_strip.hpp"
#include "test_config.hpp"
#include "unity.h"
#include "pixled_driver.hpp"

extern "C" void app_main() {
	UNITY_BEGIN();

	printf("\n> Running esp-pixled-driver test suite\n\n");
	printf(">> Testing output serializers\n");

	RUN_TEST(test_serializer_rgb_output);
	RUN_TEST(test_serializer_grb_output);
	RUN_TEST(test_serializer_grbw_output);

	printf("\n>> Testing rgb strip\n");
	RUN_TEST(test_rgb_strip_set_rgb);

	RUN_TEST(test_gbr_strip_set_rgb);
	RUN_TEST(test_gbr_strip_set_hsb);

	printf("\n>> Testing rgbw strip\n");
	RUN_TEST(test_rgbw_strip_set_rgbw);

	RUN_TEST(test_gbrw_strip_set_rgbw);
	RUN_TEST(test_gbrw_strip_set_rgb);
	RUN_TEST(test_gbrw_strip_set_hsb);

	printf("\n>> Testing predefined strip configs\n");
	RUN_TEST(test_ws2812);
	RUN_TEST(test_ws2815);
	RUN_TEST(test_sk6812);
	RUN_TEST(test_sk6812w);

	UNITY_END();
}
