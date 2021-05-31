#include "tmp102.h"

//tm102 registers
#define tmpReg 0x00
#define configReg 0x01
#define lowReg 0x02
#define highReg 0x03

//tmp102 config register MSB
#define SD 0x01
#define TM 0x02
#define POL 0x04
#define F0 0x08
#define F1 0x10

//tmp102 config register LSB
#define EM 0x10
#define AL 0x20
#define CR0 0x40
#define CR1 0x80

#define send false
#define receive true

#define conversionFactor 0.0625f

uint32_t i2cPort;
uint32_t address;
uint8_t configRegMSB;
uint8_t configRegLSB;

void tmp102Begin(uint8_t addr, uint8_t port){
    address = addr;

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

    // Set pointer register of tmp102 to the config register
    I2CMasterSlaveAddrSet(i2cPort, address, false);
    I2CMasterControl(i2cPort, I2C_MASTER_CMD_SINGLE_SEND);
    I2CMasterDataPut(i2cPort, configReg);
    while(I2CMasterBusy(i2cPort));
    
    // Receive current configuration and store locally
    I2CMasterSlaveAddrSet(i2cPort, address, true);
    I2CMasterControl(i2cPort, I2C_MASTER_CMD_BURST_RECEIVE_START);
	while(I2CMasterBusy(i2cPort));
	configRegMSB = I2CMasterDataGet(i2cPort);

    I2CMasterControl(i2cPort, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
	while(I2CMasterBusy(i2cPort));
	configRegLSB = I2CMasterDataGet(i2cPort) | EM;


    // Set pointer register of tmp102 to the config register
    I2CMasterSlaveAddrSet(i2cPort, address, false);
    I2CMasterControl(i2cPort, I2C_MASTER_CMD_BURST_SEND_START);
    I2CMasterDataPut(i2cPort, configReg);
    while(I2CMasterBusy(i2cPort));

    // Send back configuration with EM bit turned on
    I2CMasterControl(i2cPort, I2C_MASTER_CMD_BURST_SEND_CONT);
    I2CMasterDataPut(i2cPort, configRegMSB);
    while(I2CMasterBusy(i2cPort));

    I2CMasterControl(i2cPort, I2C_MASTER_CMD_BURST_SEND_FINISH);
    I2CMasterDataPut(i2cPort, configRegLSB);
    while(I2CMasterBusy(i2cPort));
}

static void tmp102Communicate(bool dir, uint8_t reg, uint32_t data){

    I2CMasterSlaveAddrSet(i2cPort, address, send);
    I2CMasterControl(i2cPort, I2C_MASTER_CMD_SINGLE_SEND);
    I2CMasterDataPut(i2cPort, reg);
    while(I2CMasterBusy(i2cPort));

    switch (dir)
    {
        case send:
            break;

        case receive:
    }
    I2CMasterSlaveAddrSet(i2cPort, address, dir);
    I2CMasterControl(i2cPort, )
}

void tmp102Enable(void){
    configRegMSB &= ~SD;

    // Set pointer register of tmp102 to the config register
    I2CMasterSlaveAddrSet(i2cPort, address, false);
    I2CMasterControl(i2cPort, I2C_MASTER_CMD_BURST_SEND_START);
    I2CMasterDataPut(i2cPort, configReg);
    while(I2CMasterBusy(i2cPort));

    I2CMasterControl(i2cPort, I2C_MASTER_CMD_BURST_SEND_FINISH);
    I2CMasterDataPut(i2cPort, configRegMSB);
    while(I2CMasterBusy(i2cPort));
}

void tmp102Disable(void){
    configRegMSB |= SD;

    // Set pointer register of tmp102 to the config register
    I2CMasterSlaveAddrSet(i2cPort, address, false);
    I2CMasterControl(i2cPort, I2C_MASTER_CMD_BURST_SEND_START);
    I2CMasterDataPut(i2cPort, configReg);
    while(I2CMasterBusy(i2cPort));

    I2CMasterControl(i2cPort, I2C_MASTER_CMD_BURST_SEND_FINISH);
    I2CMasterDataPut(i2cPort, configRegMSB);
    while(I2CMasterBusy(i2cPort));
}

float tmp102GetTemp(){
    unsigned long data;

    // Set pointer register of tmp102 to the temp register
    I2CMasterSlaveAddrSet(i2cPort, address, false);

    I2CMasterControl(i2cPort, I2C_MASTER_CMD_SINGLE_SEND);
    I2CMasterDataPut(i2cPort, tmpReg);
    while(I2CMasterBusy(i2cPort));

    // Receive temp from tmp102 temp register and convert it to float in celsius
    I2CMasterSlaveAddrSet(i2cPort, address, true);

    I2CMasterControl(i2cPort, I2C_MASTER_CMD_BURST_RECEIVE_START);
	while(I2CMasterBusy(i2cPort));
	data = I2CMasterDataGet(i2cPort) << 5;
	
	I2CMasterControl(i2cPort, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
	while(I2CMasterBusy(i2cPort));
	data += I2CMasterDataGet(i2cPort) >> 3;

    return data * conversionFactor;
}

bool tmp102Alert(void){
    //IMPLEMENT//
}

void tmp102AlertPolarity(bool polarity){
    //IMPLEMENT//
}

void tmp102SetHighLimit(uint32_t temp){
    //IMPLEMENT//
}

void tmp102SetLowLimit(uint32_t temp){
    //IMPLEMENT//
}