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

void clockConfig(void);
void UARTConfig(void);
void I2CConfig(void);
void delayms(int time);
void ledInit(void);

float temp;
char str[20];


int main(void){
	clockConfig();
	UARTConfig();
	I2CConfig();
	ledInit();
	
	tmp102Begin(0x48, 0);
	tmp102AlertPolarity(true);
	tmp102SetHighLimit(30);
	tmp102SetLowLimit(28);
	
	while(1){		
		temp = tmp102GetTemp();
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00 + (tmp102Alert() << 1));
	
		sprintf(str,"%d.%02u", (int) temp, (int) ((temp - (int) temp ) * 100) );
			
		int i = 0;
		while(str[i] != '\0'){ 
				UARTCharPut(UART0_BASE, str[i++]);
		}
		UARTCharPut(UART0_BASE, '\n');
		UARTCharPut(UART0_BASE, '\r');
		
		delayms(100);
	}
}

void clockConfig(void){
	// Configures clock to 16 MHz
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_XTAL_16MHZ);
}


void UARTConfig(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	UARTConfigSetExpClk(UART0_BASE, 16000000, 115200, 
		UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
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
	
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, 0x02);
	
}