#ifndef CONSTANTS_H
#define CONSTANTS_H

// Color orders
#define RGB pixled::RgbSerializer(0, 1, 2)
#define RBG pixled::RgbSerializer(0, 2, 1)
#define GRB pixled::RgbSerializer(1, 0, 2)
#define GBR pixled::RgbSerializer(2, 0, 1)
#define BRG pixled::RgbSerializer(1, 2, 0)
#define BGR pixled::RgbSerializer(2, 1, 0)

#define WRGB pixled::RgbwSerializer(1, 2, 3, 0)
#define WRBG pixled::RgbwSerializer(1, 3, 2, 0)
#define WGRB pixled::RgbwSerializer(2, 1, 3, 0)
#define WGBR pixled::RgbwSerializer(3, 1, 2, 0)
#define WBRG pixled::RgbwSerializer(2, 3, 1, 0)
#define WBGR pixled::RgbwSerializer(3, 2, 1, 0)

#define RWGB pixled::RgbwSerializer(0, 2, 3, 1)
#define RWBG pixled::RgbwSerializer(0, 3, 2, 1)
#define GWRB pixled::RgbwSerializer(2, 0, 3, 1)
#define GWBR pixled::RgbwSerializer(3, 0, 2, 1)
#define BWRG pixled::RgbwSerializer(2, 3, 0, 1)
#define BWGR pixled::RgbwSerializer(3, 2, 0, 1)

#define RGWB pixled::RgbwSerializer(0, 1, 3, 2)
#define RBWG pixled::RgbwSerializer(0, 3, 1, 2)
#define GRWB pixled::RgbwSerializer(1, 0, 3, 2)
#define GBWR pixled::RgbwSerializer(3, 0, 1, 2)
#define BRWG pixled::RgbwSerializer(1, 3, 0, 2)
#define BGWR pixled::RgbwSerializer(3, 1, 0, 2)

#define RGBW pixled::RgbwSerializer(0, 1, 2, 3)
#define RBGW pixled::RgbwSerializer(0, 2, 1, 3)
#define GRBW pixled::RgbwSerializer(1, 0, 2, 3)
#define GBRW pixled::RgbwSerializer(2, 0, 1, 3)
#define BRGW pixled::RgbwSerializer(1, 2, 0, 3)
#define BGRW pixled::RgbwSerializer(2, 1, 0, 3)

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
