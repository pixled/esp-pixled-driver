#ifndef CONSTANTS_H
#define CONSTANTS_H

// Color orders
#define RGB RgbSerializer(0, 1, 2)
#define RBG RgbSerializer(0, 2, 1)
#define GRB RgbSerializer(1, 0, 2)
#define GBR RgbSerializer(2, 0, 1)
#define BRG RgbSerializer(1, 2, 0)
#define BGR RgbSerializer(2, 1, 0)

#define WRGB RgbwSerializer(1, 2, 3, 0)
#define WRBG RgbwSerializer(1, 3, 2, 0)
#define WGRB RgbwSerializer(2, 1, 3, 0)
#define WGBR RgbwSerializer(3, 1, 2, 0)
#define WBRG RgbwSerializer(2, 3, 1, 0)
#define WBGR RgbwSerializer(3, 2, 1, 0)

#define RWGB RgbwSerializer(0, 2, 3, 1)
#define RWBG RgbwSerializer(0, 3, 2, 1)
#define GWRB RgbwSerializer(2, 0, 3, 1)
#define GWBR RgbwSerializer(3, 0, 2, 1)
#define BWRG RgbwSerializer(2, 3, 0, 1)
#define BWGR RgbwSerializer(3, 2, 0, 1)

#define RGWB RgbwSerializer(0, 1, 3, 2)
#define RBWG RgbwSerializer(0, 3, 1, 2)
#define GRWB RgbwSerializer(1, 0, 3, 2)
#define GBWR RgbwSerializer(3, 0, 1, 2)
#define BRWG RgbwSerializer(1, 3, 0, 2)
#define BGWR RgbwSerializer(3, 1, 0, 2)

#define RGBW RgbwSerializer(0, 1, 2, 3)
#define RBGW RgbwSerializer(0, 2, 1, 3)
#define GRBW RgbwSerializer(1, 0, 2, 3)
#define GBRW RgbwSerializer(2, 0, 1, 3)
#define BRGW RgbwSerializer(1, 2, 0, 3)
#define BGRW RgbwSerializer(2, 1, 0, 3)

// RMT config
#define RMT_DIVIDER 8
#define RMT_CLOCK 8 //80Mhz, 80000000Hz
#define RMT_RATIO (RMT_CLOCK / RMT_DIVIDER) / 100 // Deduced from nS to S convertion : (([nS]) / 1000000000) * 80000000 / 8, simplified to avoid int overflow
#define NS_TO_RMT_TICKS(NS) NS * RMT_RATIO

// Delays in nS

// RGB WS2812
#define WS2812_T0H 350
#define WS2812_T0L 800
#define WS2812_T1H 700
#define WS2812_T1L 600

// RGB WS2815
#define WS2815_T0H 300
#define WS2815_T0L 800
#define WS2815_T1H 800
#define WS2815_T1L 300

// RGB SK6812
#define SK6812_T0H 300
#define SK6812_T0L 900
#define SK6812_T1H 600
#define SK6812_T1L 600

// RGBW SK6812
#define SK6812W_T0H 300
#define SK6812W_T0L 900
#define SK6812W_T1H 600
#define SK6812W_T1L 600

#endif
