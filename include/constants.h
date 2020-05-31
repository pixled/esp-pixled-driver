#define RMT_DIVIDER 8
#define RMT_CLOCK 8 //8Mhz, 80000000Hz
#define RMT_RATIO (RMT_CLOCK / RMT_DIVIDER) / 100 // Deduced from nS to S convertion : (([nS]) / 1000000000) * 80000000 / 8, simplified to avoid int overflow

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


