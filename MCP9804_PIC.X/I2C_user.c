#include <stdio.h>
#include "xc.h"
#include "./I2C_user.h"


void I2C_UserInit(void)
{
    ANSELB = 0;
    TRISB = 0x70;
    SSPCLKPPS = 0x0E;   //RB6->MSSP:SCL;
    SSPDATPPS = 0x0C;   //RB4->MSSP:SDA;
    RB6PPS = 0x10;   //RB6->MSSP:SCL;
    RB4PPS = 0x11;   //RB4->MSSP:SDA;

    SSP1STAT = 0x80; // standerd
    SSP1CON1 = 0x28;
    SSP1CON2 = 0x00;
    SSP1ADD = 0x4f;
    
    SSP1CON1bits.SSPEN =  1;
}

void I2C_UserRead(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    int i;
    
    PIR1bits.SSP1IF = 0; // clear flag
    
    // set start condition
    SSP1CON2bits.SEN = 1;
    I2C_WAIT_SSPIF();
    
    // send device address
    I2C_UserWriteByte(i2c_addr<<1); // write
    
    // send data
    I2C_UserWriteByte(reg_addr);
    
    // set repeat start condition
    SSP1CON2bits.RSEN = 1;
    I2C_WAIT_SSPIF();
    
    //send device address
    I2C_UserWriteByte((i2c_addr<<1)|0x01); // read
    
    // receive data
    for(i=0;i<len;i++){
        if(i==(len-1)){
            reg_data[i]=I2C_UserReadByte(1);  // send nack
        }
        else{
            reg_data[i]=I2C_UserReadByte(0);  // send ack
        }
    }
    
    // set stop condition
    I2C_UserStopCondition();
    

    return;
    
}

void I2C_UserWrite(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    int i;
    
    PIR1bits.SSP1IF = 0; // clear flag
    
    // set start condition
    SSP1CON2bits.SEN = 1;
    I2C_WAIT_SSPIF();
    
    // send device address
    I2C_UserWriteByte(i2c_addr<<1); // write
    
    // send data
    I2C_UserWriteByte(reg_addr);

    for(i=0;i<len;i++){
        I2C_UserWriteByte(reg_data[i]);
    }
    
        // set stop condition
    I2C_UserStopCondition();
    
    return;
}

void I2C_UserWriteByte(uint8_t data)
{
    SSP1BUF = data; // write data
    I2C_WAIT_SSPIF();
    
    // ACK check
    if(SSP1CON2bits.ACKSTAT!=0){
        I2C_UserAlert(I2C_ERROR_WRITE_NO_ACK); // NACK
    }
    
    return;
}

uint8_t I2C_UserReadByte(uint8_t ackbit)
{
    uint8_t rcvdata;
    
    SSP1CON2bits.RCEN = 1; // Receive enable
    I2C_WAIT_SSPIF();
    
    rcvdata = SSP1BUF;
    
    // send ack or nak
    SSP1CON2bits.ACKDT=ackbit;
    SSP1CON2bits.ACKEN = 1;
    I2C_WAIT_SSPIF();    
    
    return (rcvdata);
}

void I2C_UserStopCondition(void)
{
    SSP1CON2bits.PEN = 1;
    I2C_WAIT_SSPIF();
}

void debug_error(void);
void I2C_UserAlert(I2C_ERROR status)
{
    I2C_UserStopCondition();
    printf("I2C ERROR!!! : %d\r\n",status);
    debug_error();

}

