# TI TMP102 Driver

Simple driver for the TI TMP102 I2C temperature sensor. For use with TI TM4C123X MCUs.

## Description

This is a very basic driver for the TI TMP102 I2C temperature sensor for use with TI TM4C123X microcontrollers. I wrote this driver solely for the learning experience, and
it is the first MCU driver I've made. Because of this, you should use caution when implementing it into your project. The current version contains no error handling or
detection, and was only tested using the limited tools that I had available at the time.

This driver relies heavily on TI's TivaWare™ Peripheral Driver Library. Because of this, it should be portable to all TM3C123X MCUs, but was only tested using a TM4C123GH6PM.

This driver implements nearly every feature of the TMP102 sensor including temperature readings, alerts, alert polarity adjustment, conversion rate adjustment, shutdown mode,
comparator mode, interrupt mode, fault queue adjustment, one-shot mode, and high/low limits. 

The only feature I didn't implement was the ability to turn off enhanced mode. Enhanced mode allows for 13-bit temperature readings, and I hardcoded it into this driver. I
saw no reason to allow the use of 12-bit readings.

## Getting Started

### Dependencies

* You must be using TI's TivaWare™ Peripheral Driver Library in order for this driver to work. It can be downloaded here: https://www.ti.com/tool/SW-TM4C
* You must be using an MCU compatible with TI's TivaWare™ Peripheral Driver Library. In my case, I used a TM4C123GH6PM.

### Installing

* Download tmp.c and tmp.h to your project directory, or another directory and add it to your project's include path.
* Make sure to add ```#include "tmp102.h"``` to your project.

### Executing program

In order to use this driver, you must have an I2C port on your MCU configured. Once that is complete, you simply have to initialize your tmp102 sensor by using the
tmp102begin() function. The arguments are the sensor address (default 0x48) and the I2C port number that you are using. This driver is able to use ports 0-4.

For example, if I wanted to initialize device 0x48 on I2C port 0, I would use the following:

```
tmp102Begin(0x48, 0);
```
This statement does three things:
*Saves the current

## Help

Any advise for common problems or issues.
```
command to run if program contains helper info
```

## Authors

Contributors names and contact info

ex. Dominique Pizzie  
ex. [@DomPizzie](https://twitter.com/dompizzie)

## Version History

* 0.2
    * Various bug fixes and optimizations
    * See [commit change]() or See [release history]()
* 0.1
    * Initial Release

## License

This project is licensed under the [NAME HERE] License - see the LICENSE.md file for details

## Acknowledgments

Inspiration, code snippets, etc.
* [awesome-readme](https://github.com/matiassingers/awesome-readme)
* [PurpleBooth](https://gist.github.com/PurpleBooth/109311bb0361f32d87a2)
* [dbader](https://github.com/dbader/readme-template)
* [zenorocha](https://gist.github.com/zenorocha/4526327)
* [fvcproductions](https://gist.github.com/fvcproductions/1bfc2d4aecb01a834b46)
