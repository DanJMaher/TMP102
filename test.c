#define PART_TM4C123GH6PM

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

//System Setup Prototypes
void clockConfig(void);
void I2CConfig(void);
void ledInit(void);

//Debug prototypes
void delayms(int time);
void greenBlink(int count);
void redBlink (int Count);

//Functionality Test Prototypes
void testReadConfig(void);
void testBegin(void);
void testSleep(void);
void testWake(void);
void testThermMode(void);
void testFaultCount(void);
void testSetHighLimit(void);
void testSetLowLimit(void);
void testConversionRate(void);
void testPolarity(void);
void testOneShot(void);
void testGetTemp(void);


//Gobals
float temp;


int main(void){
	//System Setup
	clockConfig();
	I2CConfig();
	ledInit();	
	
	//Test 1 (blink 1)
	testBegin();
	delayms(100);
	
	//Test 2 (blink 2)
	//testSleep();
	//delayms(100);
	
	//Test 3 (blink 3)
	//testWake();
	//delayms(100);
	
	//Test 4 (blink 4)
	//testThermMode();
	//delayms(100);
	
	//Test 5 (blink 5)
	//testFaultCount();
	//delayms(100);

	//Test 6 (blink 6)
	//testSetHighLimit();
	//delayms(100);
	
	//Test 7 (blink 7)
	//testSetLowLimit();
	//delayms(100);
	
	//Test 8 (blink 8)
	//testConversionRate();
	//delayms(100);
	
	//Test 9 (blink 9)
	testPolarity();
	delayms(100);
}

void clockConfig(void){
	// Configures clock to 16 MHz
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_XTAL_16MHZ);
}
void I2CConfig(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	
	GPIOPinConfigure(GPIO_PB2_I2C0SCL);
  GPIOPinConfigure(GPIO_PB3_I2C0SDA);

	GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
	
	I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);
}
void delayms(int time){
	volatile int x, y;
	for (x = 0; x < time; x++){
		for (y = 0; y < 7900; y++){}
	}
}

void ledInit(void){
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlDelay(1);
	
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	
}

void greenBlink(int count){
	for (int i = 0; i < count; i++){
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
		delayms(50);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, ~GPIO_PIN_3);
		delayms(50);
	}
}

void redBlink(int count){
	for (int i = 0; i < count; i++){
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
		delayms(50);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ~GPIO_PIN_1);
		delayms(50);
	}
}


void testBegin(void){
	tmp102Begin(0x48, 0);
	if (tmp102ReadConfig() == 0x60B0)
		greenBlink(1);
	else
		redBlink(1);
}

void testSleep(void){
	tmp102Sleep();
	if(tmp102ReadConfig() == 0xE1B0)
		greenBlink(2);
	else
		redBlink(2);
}

void testWake(void){
	tmp102Wake();
	if(tmp102ReadConfig() == 0x60B0)
		greenBlink(3);
	else
		redBlink(3);
}
void testThermMode(void){
	tmp102ThermostatMode(true);
	if(tmp102ReadConfig() != 0x62B0)
		redBlink(4);
	
	tmp102ThermostatMode(false);
	if(tmp102ReadConfig() != 0x60B0)
		redBlink(4);
	else
		greenBlink(4);
}

void testFaultCount(void){
	tmp102FaultCount(3);
	if(tmp102ReadConfig() != 0x78B0)
		redBlink(5);
	
	tmp102FaultCount(2);
	if(tmp102ReadConfig() != 0x70B0)
		redBlink(5);
	
	tmp102FaultCount(1);
	if(tmp102ReadConfig() != 0x68B0)
		redBlink(5);
	
	tmp102FaultCount(0);
	if(tmp102ReadConfig() != 0x60B0)
		redBlink(5);
	else
		greenBlink(5);
}

void testSetHighLimit(void){
	tmp102SetHighLimit(0);
	if(tmp102GetHighLimit() != 0.00f)
		redBlink(6);
	
	tmp102SetHighLimit(-55);
	if(tmp102GetHighLimit() != -55.00f)
		redBlink(6);
	
	tmp102SetHighLimit(-27);
	if(tmp102GetHighLimit() != -27.00f)
		redBlink(6);
	
	tmp102SetHighLimit(-13);
	if(tmp102GetHighLimit() != -13.00f)
		redBlink(6);
	
	tmp102SetHighLimit(0);
	if(tmp102GetHighLimit() != 0.000f)
		redBlink(6);
	
	tmp102SetHighLimit(1);
	if(tmp102GetHighLimit() != 1.000f)
		redBlink(6);
	
	tmp102SetHighLimit(11);
	if(tmp102GetHighLimit() != 11.000f)
		redBlink(6);
	
	tmp102SetHighLimit(27);
	if(tmp102GetHighLimit() != 27.000f)
		redBlink(6);
	
	tmp102SetHighLimit(127);
	if(tmp102GetHighLimit() != 127.000f)
		redBlink(6);
	
	tmp102SetHighLimit(150);
	if(tmp102GetHighLimit() != 150.000f)
		redBlink(6);
	
	tmp102SetHighLimit(29);
	if(tmp102GetHighLimit() != 29.000f)
		redBlink(6);
	
	else
		greenBlink(6);
}

void testSetLowLimit(void){
	tmp102SetLowLimit(0);
	if(tmp102GetLowLimit() != 0.00f)
		redBlink(7);
	
	tmp102SetLowLimit(-55);
	if(tmp102GetLowLimit() != -55.00f)
		redBlink(7);
	
	tmp102SetLowLimit(-27);
	if(tmp102GetLowLimit() != -27.00f)
		redBlink(7);
	
	tmp102SetLowLimit(-13);
	if(tmp102GetLowLimit() != -13.00f)
		redBlink(7);
	
	tmp102SetLowLimit(0);
	if(tmp102GetLowLimit() != 0.000f)
		redBlink(7);
	
	tmp102SetLowLimit(1);
	if(tmp102GetLowLimit() != 1.000f)
		redBlink(7);
	
	tmp102SetLowLimit(11);
	if(tmp102GetLowLimit() != 11.000f)
		redBlink(7);
	
	tmp102SetLowLimit(27);
	if(tmp102GetLowLimit() != 27.000f)
		redBlink(7);
	
	tmp102SetLowLimit(127);
	if(tmp102GetLowLimit() != 127.000f)
		redBlink(7);
	
	tmp102SetLowLimit(150);
	if(tmp102GetLowLimit() != 150.000f)
		redBlink(7);
	
	tmp102SetLowLimit(26);
	if(tmp102GetLowLimit() != 26.000f)
		redBlink(7);
	
	else
		greenBlink(7);
}

void testConversionRate(void){
	tmp102ConversionRate(0);
	if(tmp102ReadConfig() != 0x6030)
		redBlink(8);
	
	tmp102ConversionRate(3);
	if(tmp102ReadConfig() != 0x60F0)
		redBlink(8);
	
	tmp102ConversionRate(1);
	if(tmp102ReadConfig() != 0x6070)
		redBlink(8);
	
	tmp102ConversionRate(2);
	if(tmp102ReadConfig() != 0x60B0)
		redBlink(8);
	
	else
		greenBlink(8);
}

void testPolarity(void){
	if (tmp102Alert() != 1)
		redBlink(9);

	tmp102AlertPolarity(1);
	if (tmp102Alert() != 0)
		redBlink(9);
	
	else
		greenBlink(9);
}

void testOneShot(void){
	
}

void testGetTemp(void){
	
}