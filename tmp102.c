#include "tmp102.h"

//tm102 registers
#define     tmpReg      0x00
#define     configReg   0x01
#define     lowReg      0x02
#define     highReg     0x03

//tmp102 config register MSB
#define     SD  0x0100
#define     TM  0x0200
#define     POL 0x0400
#define     F0  0x0800
#define     F1  0x1000

//tmp102 config register LSB
#define     EM  0x0010
#define     AL  0x0020
#define     CR0 0x0040
#define     CR1 0x0080

//For static tmp102Communicate method clarity
#define send false
#define receive true

//Signed 13-bit int received needs to be multiplied
//by this value to get degrees in celsius
#define conversionFactor 0.0625f

uint32_t i2cPort;
uint32_t address;
uint16_t configRegData;

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

    // OLD
    /*
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
    */

   //NEW

   // Receive current configReg from tmp102, turn on EM bit and 
   // send back
    tmp102Communicate(receive, configReg, &configRegData);
    configRegData |= EM;
    tmp102Communicate(send, configReg, &configRegData);
}

void tmp102Enable(void){

    configRegData &= ~SD;
    tmp102Communicate(send, configReg, &configRegData);

    //OLD
    /*
    // Set pointer register of tmp102 to the config register
    I2CMasterSlaveAddrSet(i2cPort, address, false);
    I2CMasterControl(i2cPort, I2C_MASTER_CMD_BURST_SEND_START);
    I2CMasterDataPut(i2cPort, configReg);
    while(I2CMasterBusy(i2cPort));

    I2CMasterControl(i2cPort, I2C_MASTER_CMD_BURST_SEND_FINISH);
    I2CMasterDataPut(i2cPort, configRegMSB);
    while(I2CMasterBusy(i2cPort));
    */
}

void tmp102Disable(void){
    configRegData |= SD;
    tmp102Communicate(send, configReg, &configRegData);

    //OLD
    /*
    // Set pointer register of tmp102 to the config register
    I2CMasterSlaveAddrSet(i2cPort, address, false);
    I2CMasterControl(i2cPort, I2C_MASTER_CMD_BURST_SEND_START);
    I2CMasterDataPut(i2cPort, configReg);
    while(I2CMasterBusy(i2cPort));

    I2CMasterControl(i2cPort, I2C_MASTER_CMD_BURST_SEND_FINISH);
    I2CMasterDataPut(i2cPort, configRegMSB);
    while(I2CMasterBusy(i2cPort));
    */
}

float tmp102GetTemp(){
    unsigned long data;

    tmp102Communicate(receive, tmpReg, &data);
    data = data >> 3;

    //OLD
    /*
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
    */

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

static void tmp102Communicate(bool dir, uint8_t reg, uint16_t *data){

    uint8_t msb;
    uint8_t lsb;

    switch (dir)
    {
        case send:
            msb = *data >> 8;
            lsb = *data;
            
            // Set tmp102 pointer address and send two bytes of data
            I2CMasterSlaveAddrSet(i2cPort, address, send);
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

        case receive:
            // Set tmp102 pointer address and receive two bytes of data
            I2CMasterSlaveAddrSet(i2cPort, address, send);
            I2CMasterControl(i2cPort, I2C_MASTER_CMD_SINGLE_SEND);
            I2CMasterDataPut(i2cPort, reg);
            while(I2CMasterBusy(i2cPort));

            I2CMasterSlaveAddrSet(i2cPort, address, receive);
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