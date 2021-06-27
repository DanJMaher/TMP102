/*****************************************************************************
******************************************************************************
*tmp102.h
*TI TMP102 Header File for TM4C123X Microcontrollers
* 
*Dan Maher
*06/05/2021
*  
*https://github.com/DanJMaher/TMP102
*
*This file protoypes and describes the functions defined in tmp102.c that 
*configure and control the TI TMP102 I2C temperature sensor.
******************************************************************************
******************************************************************************
MIT License

Copyright (c) 2021 Dan Maher

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************
******************************************************************************/

#ifndef TMP102_H
#define	TMP102_H

/***************************************
 * Private Functions
 ***************************************/
// Internal method to simplify send/receive commands to the tmp102.
// dir is defined as send/receive, reg is the tmp102 internal register to 
// send/receive the data from. If dir == send, *data is the 16 bit data
// sent to reg. If dir == receive, *data is the 16 bit data received from
// reg.
static void tmp102Communicate(bool dir, uint8_t reg, uint16_t *data);

/***************************************
 * Public Functions
 ***************************************/
// Configures communication to the given I2C address at the given I2C port
// and reads tmp102 configuration register to local memory
void tmp102Begin(uint8_t addr, uint8_t port);

// Enables the tmp102. The tmp102 is enabled by default, and this only needs to be
// called if previously disabled vis tmp102Sleep
void tmp102Wake(void);

// Disables the tmp102, saving power by shutting down all device circuitry except 
// for the serial connection. Can be used with tmp102OneShot for low-power
// temp conversions
void tmp102Sleep(void);

// Activates Comparator Mode (false, default) or Interrupt Mode (true).
// These modes change how to ALERT pin functions. 
//
// While in Comparator Mode, the ALERT pin becomes active when the temperature equals 
// or exceeds the value set via tmp102SetHighLimit() for a consecutive number of
// faults according to the value set via tmp102FaultCount(). The ALERT pin remains
// active until the temperature falls below the value set via tmp102SetLowLimit() for 
// the same number of faults.
//
// While in Interrupt Mode, the ALERT pin becomes active when the temperature equals
// or exceeds the value set via tmp102SetHighLimit() for a consecutive number of
// faults according to the value set via tmp102FaultCount(). The ALERT pin remains active
// until any function is called that reads a value (ex. tmp102GetTemp()). The ALERT pin
// becomes active again when the temperature falls below the value set via
// tmp102SetLowLimit(), and remains until any function is called that reads a value from
// the tmp102 (ex. tmp102GetTemp()).
void tmp102ThermostatMode(bool b);

// The number of fault conditions required to generate an alert.
// 0 = 1 fault (default), 1 = 2 faults, 2 = 4 faults, 3 = 6 faults.
void tmp102FaultCount(uint8_t faults);

// Receives the current temperature from the tmp102 device
float tmp102GetTemp(void);

// Upper alert level for the alert bit of the tmp102. Default is 80 c
void tmp102SetHighLimit(int16_t temp);

// Returns the current High Temp limit from the tmp102 device
float tmp102GetHighLimit(void);

// Lower alert level for the alert bit of the tmp102. Default is 75 c
void tmp102SetLowLimit(int16_t temp);

// Returns the current Low Temp limit from the tmp102 device
float tmp102GetLowLimit(void);

// Sets temp conversion rate. 0 = 0.25Hz, 1 = 1Hz, 2 = 4Hz (default), and 
// 3 = 8Hz.
void tmp102ConversionRate(uint8_t rate);

// Reads the alert bit of the tmp102, which indicates whether the temperature has
// equals or exeeds the limit set in the tmp102's high temp register. Returns 0 
// on alert at default, or 1 if the alert polarity register has been toggled.
bool tmp102Alert(void);

// Adjusts whether tmp102Alert returns a 0 when alerting (default) or a 1.
void tmp102AlertPolarity(bool polarity);

// If tmp102 is disabled (shut down), one-shot mode can be used. This function 
// performs one temp conversion per call. The conversion is ready for reading
// when indicated by tmp102OneShotReady()
void tmp102OneShot();

// Returns true when a OneShot conversion is ready. tmp102OneShot() must be called
// prior to tmp102OneShotReady()
bool tmp102OneShotReady();

// Utility function for reading the current value of the tmp102's config register
uint16_t tmp102ReadConfig(void);

#endif