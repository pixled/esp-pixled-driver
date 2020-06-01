#include "output.hpp"

struct StripConfig {
	StripConfig(uint32_t t0h,  uint32_t t0l,  uint32_t t1h,  uint32_t t1l)
		: t0h(t0h), t0l(t0l), t1h(t1h), t1l(t1l) {}
	uint32_t t0h;
	uint32_t t0l;
	uint32_t t1h;
	uint32_t t1l;
};

struct RgbStripConfig : public StripConfig {
	RgbStripConfig(RgbSerializer& serializer, uint32_t t0h,  uint32_t t0l,  uint32_t t1h,  uint32_t t1l)
		: StripConfig(t0h, t0l, t1h, t1l), serializer(serializer) {}

	RgbSerializer serializer;
};

struct RgbwStripConfig : public StripConfig {

	RgbwStripConfig(RgbwSerializer& serializer, uint32_t t0h,  uint32_t t0l,  uint32_t t1h,  uint32_t t1l)
		: StripConfig(t0h, t0l, t1h, t1l), serializer(serializer) {}

	RgbwSerializer serializer;
};
