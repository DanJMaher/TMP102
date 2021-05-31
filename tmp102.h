#ifndef TMP102_H
#define	TMP102_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/i2c.h"
#include "tmp102.h"

// Configures communication to the given I2C address at the given I2C port
// and reads tmp102 configuration register to local memory
void tmp102Begin(uint8_t addr, uint8_t port);

// Receives the current temperature from the tmp102 device
float tmp102GetTemp(void);

// Reads the alert bit of the tmp102, which indicates whether the temperature has
// equals or exeeds the limit set in the tmp102's high temp register. Returns 0 
// on alert at default, or 1 if the alert polarity register has been toggled./
bool tmp102Alert(void);

// Adjusts whether tmp102Alert returns a 0 when alerting (default) or a 1./
void tmp102AlertPolarity(bool polarity);

// Upper alert level for the alert bit of the tmp102. 
void tmp102SetHighLimit(uint32_t temp);

// Lower alert level for the alert bit of the tmp102. 
void tmp102SetLowLimit(uint32_t temp);

// Enables the tmp102. The tmp102 is enabled by default, and this only needs to be
// called if previously disabled vis tmp102Disable
void tmp102Enable(void);

// Disables the tmp102, saving power by shutting down all device circuitry except 
// for the serial connection.
void tmp102Disable(void);

#endif