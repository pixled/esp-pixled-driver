#include "output.hpp"
#include "unity.h"

extern "C" void app_main() {
	UNITY_BEGIN();

	printf("\n> Running esp-pixled-driver test suite\n\n");
	printf(">> Testing output serializers\n");

	RUN_TEST(test_serializer_rgb_output);
	RUN_TEST(test_serializer_grb_output);
	RUN_TEST(test_serializer_grbw_output);

	UNITY_END();
}
