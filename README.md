# TI TMP102 Driver

Simple driver for the TI TMP102 I2C temperature sensor. For use with TI TM4C123X MCUs and TI's TivaWare™ Peripheral Driver Library.

## Description

This is a very basic driver for the TI TMP102 I2C temperature sensor for use with TI TM4C123X microcontrollers. I wrote this driver solely for the learning experience, and it is the first MCU driver I made. Because of this, you should use caution when implementing it into your project. The current version contains no error handling or detection, and was only tested using the limited tools that I had available at the time.

This driver relies heavily on TI's TivaWare™ Peripheral Driver Library. Because of this, it should be portable to all TM3C123X MCUs, but was only tested using a TM4C123GH6PM.

This driver implements nearly every feature of the TMP102 sensor including temperature readings, alerts, alert polarity adjustment, conversion rate adjustment, shutdown mode, comparator mode, interrupt mode, fault queue adjustment, one-shot mode, and high/low limits. 

The only feature I didn't implement was the ability to turn off enhanced mode. Enhanced mode allows for 13-bit temperature readings, and I hardcoded it into this driver. I saw no reason to allow the use of 12-bit readings.

## Getting Started

For function details, please see the function descriptions in tmp102.h

### Dependencies

* You must be using TI's TivaWare™ Peripheral Driver Library in order for this driver to work. It can be downloaded here: https://www.ti.com/tool/SW-TM4C
* You must be using an MCU compatible with TI's TivaWare™ Peripheral Driver Library. In my case, I used a TM4C123GH6PM.

### Installing

* Download tmp.c and tmp.h to your project directory, or another directory and add it to your project's include path.
* Make sure to add ```#include "tmp102.h"``` to your project.

### Usage

In order to use this driver, you must have an I2C port on your MCU configured. Once that is complete, you simply have to initialize your tmp102 sensor by using the tmp102Begin() function. 

tmp102Begin() takes two arguments, the sensor address (default 0x48) and the I2C port number that you are using. This driver is able to use ports 0-3.

For example, if I wanted to initialize device 0x48 on I2C port 0, I would use the following:

```
tmp102Begin(0x48, 0);
```
This statement does three things:

* Saves the device address locally
* Saves the device's port number locally
* Retrieves the current configuration from the sensor and ensures that EM mode (13-bit temperatures) is active

Everytime tmp102Begin() is called, the local address and port variables are overwritten with the new values. This means that you can use multiple TMP102 sensors on different addresses by calling the tmp102Begin() function to switch between sensors. Configuration settings are stored in the sensors themselves as long as they maintain power.

After tmp102Begin() is called, you are able to use any of the other functions. If you you want the most basic of functionality, you can immediately start calling tmp102GetTemp() after tmp102Begin():

```
float temp;
tmp102Begin(0x48, 0);

while(1){
   temp = tmp102GetTemp();
   //do something with temp
}
```

## Authors

Dan Maher
* [LinkedIn](https://www.linkedin.com/in/dan-maher-8404134a/)
* [Github](https://github.com/DanJMaher)

## Version History

* 0.1
    * Initial Release

## License

This project is licensed under the MIT License - see LICENSE.MD for details

## Acknowledgments

Inspiration and Reference Material
* [Getting Stated with TivaWare](https://www.ti.com/lit/ug/spmu373/spmu373.pdf?ts=1622989782546&ref_url=https%253A%252F%252Fwww.ti.com%252Ftool%252FSW-TM4C)
* [Tivaware Peripheral Driver Library](https://www.ti.com/lit/ug/spmu298e/spmu298e.pdf?ts=1622989784912&ref_url=https%253A%252F%252Fwww.ti.com%252Ftool%252FSW-TM4C)
* [TMP102 Data Sheet](https://www.ti.com/lit/ds/symlink/tmp102.pdf?ts=1622990619468&ref_url=https%253A%252F%252Fwww.google.com%252F)
* [SparkFun TMP102 Arduino Driver](https://github.com/sparkfun/SparkFun_TMP102_Arduino_Library)
(Note: My driver is not based on the SparkFun driver, but I did use theirs as reference material when determining what features to include.)
