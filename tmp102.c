/*****************************************************************************
******************************************************************************
*tmp102.c
*TI TMP102 Source File for TM4C123X Microcontrollers
* 
*Dan Maher
*06/05/2021
*  
*https://github.com/DanJMaher/TMP102
*
*This file defines the functions prototyped and desrived in tmp102.h. See the
*readme on detailed implementation information.
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

/***************************************
 * Includes
 ***************************************/
#include "tmp102.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/i2c.h"

/***************************************
 * Defines
 ***************************************/
//tm102 registers
#define     TMPREG      0x00
#define     CONFIGREG   0x01
#define     LOWREG      0x02
#define     HIGHREG     0x03

//tmp102 config register MSB
#define     SD  0x0100
#define     TM  0x0200
#define     POL 0x0400
#define     FQ  0x1800
#define     OS  0x8000

//tmp102 config register LSB
#define     EM  0x0010
#define     AL  0x0020
#define     CR  0x00C0

//For static tmp102Communicate method clarity
#define     SEND    0
#define     RECEIVE 1

//Signed 13-bit int RECEIVEd needs to be multiplied
//by this value to get degrees in celsius
#define conversionFactor 0.0625f

/***************************************
 * Local Variables
 ***************************************/
static uint32_t i2cPort;
static uint32_t address;
static uint16_t configRegData;

/***************************************
 * Private Functions
 ***************************************/
static void tmp102Communicate(bool dir, uint8_t reg, uint16_t *data){
    // data is broken into most and least significant bytes, since data
    // is sent one byte at a time over i2c
    uint8_t msb;
    uint8_t lsb;

    switch (dir)
    {
        case SEND:
            msb = *data >> 8;
            lsb = *data;
            
            // Set tmp102 pointer address and SEND two bytes of data
            I2CMasterSlaveAddrSet(i2cPort, address, SEND);
            I2CMasterControl(i2cPort, I2C_MASTER_CMD_BURST_SEND_START);
            I2CMasterDataPut(i2cPort, reg);
            while(I2CMasterBusy(i2cPort));

            I2CMasterControl(i2cPort, I2C_MASTER_CMD_BURST_SEND_CONT);
            I2CMasterDataPut(i2cPort, msb);
            while(I2CMasterBusy(i2cPort));

            I2CMasterControl(i2cPort, I2C_MASTER_CMD_BURST_SEND_FINISH);
            I2CMasterDataPut(i2cPort, lsb);
            while(I2CMasterBusy(i2cPort));

            break;

        case RECEIVE:
            // Set tmp102 pointer address and RECEIVE two bytes of data
            I2CMasterSlaveAddrSet(i2cPort, address, SEND);
            I2CMasterControl(i2cPort, I2C_MASTER_CMD_SINGLE_SEND);
            I2CMasterDataPut(i2cPort, reg);
            while(I2CMasterBusy(i2cPort));

            I2CMasterSlaveAddrSet(i2cPort, address, RECEIVE);
            I2CMasterControl(i2cPort, I2C_MASTER_CMD_BURST_RECEIVE_START);
            while(I2CMasterBusy(i2cPort));
            msb = I2CMasterDataGet(i2cPort);

            I2CMasterControl(i2cPort, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
            while(I2CMasterBusy(i2cPort));
            lsb = I2CMasterDataGet(i2cPort);

            *data = (msb << 8) + lsb;

            break;
    }
}

/***************************************
 * Public Functions
 ***************************************/
void tmp102Begin(uint8_t addr, uint8_t port){
    address = addr;

    // Set hardware I2C port
    switch (port)
    {
        case 0:
            i2cPort = I2C0_BASE;
            break;
        case 1:
            i2cPort = I2C1_BASE;
            break;
        case 2:
            i2cPort = I2C2_BASE;
            break;
        case 3:
            i2cPort = I2C3_BASE;
            break;
    }

   // RECEIVE current CONFIGREG from tmp102, turn on Extended Mode bit and 
   // SEND back
    tmp102Communicate(RECEIVE, CONFIGREG, &configRegData);
    configRegData |= EM;
    tmp102Communicate(SEND, CONFIGREG, &configRegData);
}

void tmp102Wake(void){
    // Turns off the Shutdown Mode bit in the config register of the tmp102
    configRegData &= ~SD;
    tmp102Communicate(SEND, CONFIGREG, &configRegData);
}

void tmp102Sleep(void){
    // Turns on the Shutdown Mode bit in the config register of the tmp102
    configRegData |= SD;
    tmp102Communicate(SEND, CONFIGREG, &configRegData);
}

void tmp102ThermostatMode(bool b){
    if(b){
        configRegData |= TM;
    }else{
        configRegData &= ~TM;
    }
    tmp102Communicate(SEND, CONFIGREG, &configRegData);
}

void tmp102FaultCount(uint8_t faults){
    configRegData &= ~FQ;
    configRegData |= faults << 11;
    tmp102Communicate(SEND, CONFIGREG, &configRegData);
}

float tmp102GetTemp(){
    int16_t data;

    tmp102Communicate(RECEIVE, TMPREG, &data);
    data = data >> 3;

    return data * conversionFactor;
}

void tmp102SetHighLimit(int16_t temp){
    temp /= conversionFactor;
    temp = temp << 3;
    tmp102Communicate(SEND, HIGHREG, &temp);
}

float tmp102GetHighLimit(void){
    int16_t data;

    tmp102Communicate(RECEIVE, HIGHREG, &data);
    data = data >> 3;

    return data * conversionFactor;
}

void tmp102SetLowLimit(int16_t temp){
    temp /= conversionFactor;
    temp = temp << 3;
    tmp102Communicate(SEND, LOWREG, &temp);
}

float tmp102GetLowLimit(void){
    int16_t data;

    tmp102Communicate(RECEIVE, LOWREG, &data);
    data = data >> 3;

    return data * conversionFactor;
}

void tmp102ConversionRate(uint8_t rate){
    configRegData &= ~CR;
    configRegData |= rate << 6;

    tmp102Communicate(SEND, CONFIGREG, &configRegData);
}

bool tmp102Alert(void){
    tmp102Communicate(RECEIVE, CONFIGREG, &configRegData);
    bool alert = (configRegData & AL) >> 5;
    return alert;
}

void tmp102AlertPolarity(bool polarity){
    configRegData |= POL & (polarity << 10);
    tmp102Communicate(SEND, CONFIGREG, &configRegData);
}

void tmp102OneShot(){
    configRegData |= OS;
    tmp102Communicate(SEND, CONFIGREG, &configRegData);
}

bool tmp102OneShotReady(){
    tmp102Communicate(RECEIVE, CONFIGREG, &configRegData);
    return (configRegData & OS) >> 15;
}

uint16_t tmp102ReadConfig(void){
    tmp102Communicate(RECEIVE, CONFIGREG, &configRegData);
    return configRegData;
}