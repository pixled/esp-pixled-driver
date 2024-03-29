# esp-pixled-driver :rainbow: :sheep:

A standalone
[esp-idf](https://docs.espressif.com/projects/esp-idf/en/stable/index.html)
component to drive RGB / RGBW led strip.

- [x] Common `Strip` interface, to seamlessly drive RGB and RGBW LED strips
- [x] Efficient esp-idf implementation (based on RMT)
- [x] Support for **any** LED strip hardware
- [x] True RGBW support : classical RGB colors are converted to RGBW to take
	full advantage of the white LED.

# Installation

The component can be installed in different ways.

## Global installation

The component can be installed directly in the `esp-idf` `components` folder. The
advantage of this method is that the component is available directly in any
`esp-idf` project, without any additional configuration.

The `$IDF_PATH` environment variable is defined once the idf environment is
loaded in your current session, and correspond to your `esp-idf` installation
folder. See the official [esp-idf
documentation](https://docs.espressif.com/projects/esp-idf/en/stable/get-started/index.html#step-4-set-up-the-environment-variables)
for more details.

```
cd $IDF_PATH/components
git clone https://github.com/pixled/esp-pixled-driver
```
And that's it.

## User repository

An other method consists in cloning this repository into any `user_directory`, using
```
git clone https://github.com/pixled/esp-pixled-driver
```
Then, the component can be imported into any esp-idf project with the following
statement, added to the appropriate `CMakeLists.txt` :
```
set(EXTRA_COMPONENT_DIRS user_directory)
```
or compiling with the following command :
```
idf.py -DEXTRA_COMPONENT_DIRS=user_directory build
```
Notice that `user_directory` is the *parent* folder of `esp-pixled-driver`, not the `esp-pixled-driver` folder itself (ie `user_directory/esp-pixled-driver`).

# Usage
## Load the component

The `main` esp-idf component automatically loads any available component, so you
don't need to add axtra configuration to load the library from this component.

For any other component, the `esp-pixled-driver` component must be defined as a
dependency. For example :
```
idf_component_register(SRCS "my_component.cpp"
                  INCLUDE_DIRS "."
                  REQUIRES esp-pixled-driver)
```

## Include the library

Once the component has been properly installed and loaded, the library can be included
using the following statement : 
```
#include "pixled_driver.hpp"

using namespace pixled;
```
## Drive led strips

A Strip can be built using the following constructors :
#### `RgbStrip(gpio_num_t gpio, uint16_t length, rmt_channel_t channel, RgbConfig config)`
Builds an RGB strip.
#### `RgbStrip(gpio_num_t gpio, uint16_t length, RgbConfig config)`
Same as before, using default RMT channel RMT_CHANNEL_0.
#### `RgbwStrip(gpio_num_t gpio, uint16_t length, rmt_channel_t channel, RgbwConfig config)`
Builds an RGBW strip.
#### `RgbwStrip(gpio_num_t gpio, uint16_t length, RgbConfig config)`
Same as before, using default RMT channel RMT_CHANNEL_0.

### RMT Channel

The `channel` parameter must correspond to one of the 8 [RMT
channels](https://docs.espressif.com/projects/esp-idf/en/release-v4.2/esp32/api-reference/peripherals/rmt.html#_CPPv413rmt_channel_t)
available. Only a single led strip can be bound to each RMT channel.

### Predefined led types

The following predefined classes can be used as the `config` parameter:
RGB strips | RGBW strips
-----------|------------
WS2812() | SK6812W()
WS2815() |
SK6812() |

The library can also be used to drive any existing led type. See the [Using
custom LED types](#using-custom-led-types) section to learn how.

### Example usage
```
#include "pixled_driver.hpp"
#include "freertos/task.h"

#define LED_COUNT 16

using namespace pixled;

extern 'C' void app_main() {
	// Initiates an RGB WS2812 Led strip on GPIO 12, on default RMT_CHANNEL_0
	RgbStrip rgb {GPIO_NUM_12, length, WS2812()};

	// Initiates an RGBW SK6812W Led strip on GPIO 14, on RMT channel RMT_CHANNEL_1
	RgbwStrip rgbw {GPIO_NUM_14, length, RMT_CHANNEL_1, SK6812()};

	while(1) {
		for(int h = 0; h < 360; h++) {
			for(int i = 0; i < LED_COUNT; i++) {
				// HSB is converted to RGB
				rgb.setHsbPixel(i, h, 0.6, 0.4);
				// HSB is converted to RGBW
				rgbw.setHsbPixel(i, h, 0.6, 0.4);
			}
			rgb.show();
			rgbw.show();

			vTaskDelay(50 / portTICK_PERIOD_MS);
		}
	}
}
```

### Strip interface
Any led strip can be controlled using the following generic interface :
#### `strip.setRgbPixel(int index, uint8_t red, uint8_t green, uint8_t blue)`
With red, green, blue in [0-255]

#### `strip.setHsbPixel(int index, float hue, float saturation, float brightness)`
With hue in [0,360], saturation, brightness in [0,1] 

#### `strip.clear()`
Sets all the colors to (0, 0, 0).

#### `strip.show()`
Transmits the internal buffer to the LEDs.

### RGBW Strip specific
Additionally, the following function is available **only** for RGBW strips, to manually control the white LED :

#### `strip.setRgbwPixel(int index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white)`
With red, green, blue in [0-255].

Note that it is **not required** to use this function to use the white LED! It
is recommended to use the `setRgbPixel()` function even on RGBW strips, to
efficiently convert RGB colors to RGBW and smartly use the available white LED.

Moreover, the `setHsbPixel()` method apply to an `RgbwStrip` automatically takes
advantage of the white LED.

## Efficiently use the generic Strip interface
One of the main interest of this library is the ability to seamlessly drive **any** type of led hardware thanks to a generic interface.
This allows to write generic code, for example to generate animations, that will be compatible with every strips.

```
#include "pixled_driver.hpp"

using namespace pixled;

void animate(Strip& strip) {
   uint8_t r = /*something*/;
   uint8_t g = /*something*/;
   uint8_t b = /*something*/;
   
   // Genertic setRgb function.
   // Different internal behavior according to the actual led type.
   strip.setRgb(r, g, b);
}

extern 'C' void app_main() {
    RgbStrip rgb {GPIO_NUM_12, length, WS2812()};
    RgbwStrip rgbw {GPIO_NUM_14, length, RMT_CHANNEL_1, SK6812()};
    
    // The same function can be used to control any strip.
    // In the case of rgbw, RGB colors are automatically converted to RGBW
    animate(rgb);
    animate(rgbw);
    
    rgb.show();
    rgbw.show();
}
```

# Advanced use
## Using custom LED types
The library can also be used to drive **any** user defined led type.

To do so, the appropriate `config` must be defined.
Two things must be known, that can be find in the corresponding LED datasheet :
- Color output order : RGB, GBR, GBRW, etc...
- T0H, T0L, T1H, T1L constants, in nS. (Typically in the order of 100nS)

A custom configuration can be defined as follow :
```
#define CUSTOM_T0H 200
#define CUSTOM_T0L 300
#define CUSTOM_T1H 200
#define CUSTOM_T1L 400

extern 'C' void app_main() {
	// Custom RGB strip config
	RgbStripConfig rgb_config {GBR, CUSTOM_T0H, CUSTOM_T0L, CUSTOM_T1H, CUSTOM_T1L};
	// Custom RGBW strip config
	RgbwStripConfig rgb_config {GBRW, CUSTOM_T0H, CUSTOM_T0L, CUSTOM_T1H, CUSTOM_T1L};

	RgbStrip rgb {GPIO_NUM_12, 20, RMT_CHANNEL_0, rgb_config};

	RgbwStrip rgbw {GPIO_NUM_14, 20, RMT_CHANNEL_1, rgbw_config};
}
```

The custom config can also be wrapped in a custom class :
```
#define CUSTOM_T0H 200
#define CUSTOM_T0L 300
#define CUSTOM_T1H 200
#define CUSTOM_T1L 400

struct CUSTOM_RGB : public RgbStripConfig {
	CUSTOM_RGB()
		: RgbStripConfig(GBR, CUSTOM_T0H, CUSTOM_T0L, CUSTOM_T1H, CUSTOM_T1L)
		{}
}

extern 'C' void app_main() {
	RgbStrip rgb {GPIO_NUM_12, 20, RMT_CHANNEL_0, CUSTOM_RGB()};
}
```

