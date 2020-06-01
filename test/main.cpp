#include "output.hpp"
#include "unity.h"

extern "C" void app_main() {
	UNITY_BEGIN();

	RUN_TEST(test_serializer_rgb_output);

	UNITY_END();
}
