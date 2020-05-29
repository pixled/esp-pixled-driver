# esp-pixled-driver :rainbow: :sheep:

A standalone
[esp-idf](https://docs.espressif.com/projects/esp-idf/en/stable/index.html)
component to drive RGB / RGBW led strip.

Features :
- Support for any LED strip hardware
- RGBW support : classical RGB colors can be converted to RGBW colors to take
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

An other method consists in cloning this repository into any user defined directory, using
```
git clone https://github.com/pixled/esp-pixled-driver
```
Then, the component can be imported into any esp-idf project with the following
statement, added to the appropriate `CMakeLists.txt` :
```
set(EXTRA_COMPONENT_DIRS /path/to/driver/installation/folder)
```

## Embedded in project

Finally, the project can be embedded directly in a project using a GitHub
submodule.

// TODO

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
#include "pixled_driver.h"
```
## Drive led strips
// TODO
