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
```
## Drive led strips
### Example usage
```
#include "pixled_driver.hpp"
#include "freertos/task.h"

#define LED_COUNT 16

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
With red, green, blue in [0-255]. Note that it is **not required** to use this function to use the white LED!
It is recommended to use the `setRgbPixel` function even on RGBW strips, to efficiently convert RGB colors to RGBW and smartly use the available white LED.
