#include <stdio.h>
#include <stdint.h>

#include "xc.h"
#include "./MCP9804.h"

#define I2C_MAX_DATA_LEN    16
 uint8_t I2CWorkRegData[I2C_MAX_DATA_LEN+1];

void MCP9804_IdRead(void)
{
    I2C_UserRead(MCP9804_DEVICE_ADDR,MCP9804_ADDR_MANUFACTURE_ID,I2CWorkRegData,2);
    printf("Manufacture ID: 0x%02x%02x\r\n",I2CWorkRegData[0],I2CWorkRegData[1]);
    
    I2C_UserRead(MCP9804_DEVICE_ADDR,MCP9804_ADDR_DEVICE_ID,I2CWorkRegData,2);
    printf("Device ID:0x%02x, Rev:%02x\r\n",I2CWorkRegData[0],I2CWorkRegData[1]);
    
    return;
}

void MCP9804_Temperature(void)
{
    uint8_t     UpperByte;
    uint8_t     LowerByte;
    uint8_t     temperature;
    uint32_t    tempeature_point;

    I2C_UserRead(MCP9804_DEVICE_ADDR,MCP9804_ADDR_TEMPERATURE,I2CWorkRegData,2);
    
    UpperByte = I2CWorkRegData[0];
    LowerByte = I2CWorkRegData[1];

//    printf("Temp RAW: 0x%02x%02x\r\n",UpperByte,LowerByte);

    if(UpperByte&0x80){
        printf("Warning: Temperature Critical\r\n");
    }
        if(UpperByte&0x40){
        printf("Warning: Temperature Upper\r\n");
    }
    if(UpperByte&0x20){
        printf("Warning: Temperature Lower\r\n");
    }
    
    UpperByte = UpperByte & 0x1F; //Clear flag bits
    if ((UpperByte & 0x10) == 0x10){ //TA < 0°C
        UpperByte = UpperByte & 0x0F; //Clear SIGN
        temperature = 256 - ((UpperByte << 4) + (LowerByte >> 4));
    }
    else{ //TA ≥ 0°C
        temperature = ((UpperByte << 4) + (LowerByte >> 4));
    }
    
    tempeature_point = (uint32_t)625*(uint32_t)(LowerByte&0x0f);

    printf("Temprature: %u.%04u[C]\r\n\r\n",temperature,tempeature_point);
    
    return;

}

void MCP9804_SetTupper(void)
{
    I2CWorkRegData[0] = (uint8_t)MCP9804_PARAM_T_UPPER_H;
    I2CWorkRegData[1] = (uint8_t)MCP9804_PARAM_T_UPPER_L;
    
    I2C_UserWrite(MCP9804_DEVICE_ADDR,MCP9804_ADDR_T_UPPER,I2CWorkRegData,2);
}

void MCP9804_SetTlower(void)
{
    I2CWorkRegData[0] = (uint8_t)MCP9804_PARAM_T_LOWER_H;
    I2CWorkRegData[1] = (uint8_t)MCP9804_PARAM_T_LOWER_L;
    
    I2C_UserWrite(MCP9804_DEVICE_ADDR,MCP9804_ADDR_T_LOWER,I2CWorkRegData,2);
}

void MCP9804_SetTcrit(void)
{
    I2CWorkRegData[0] = (uint8_t)MCP9804_PARAM_T_CRIT_H;
    I2CWorkRegData[1] = (uint8_t)MCP9804_PARAM_T_CRIT_L;
    
    I2C_UserWrite(MCP9804_DEVICE_ADDR,MCP9804_ADDR_T_CRIT,I2CWorkRegData,2);
}

void MCP9804_ConfigRead(void)
{
    I2C_UserRead(MCP9804_DEVICE_ADDR,MCP9804_ADDR_T_UPPER,I2CWorkRegData,2);
    printf("Tupper: 0x%02x%02x\r\n",I2CWorkRegData[0],I2CWorkRegData[1]);

    I2C_UserRead(MCP9804_DEVICE_ADDR,MCP9804_ADDR_T_LOWER,I2CWorkRegData,2);
    printf("Tlower: 0x%02x%02x\r\n",I2CWorkRegData[0],I2CWorkRegData[1]);
    
    I2C_UserRead(MCP9804_DEVICE_ADDR,MCP9804_ADDR_T_CRIT,I2CWorkRegData,2);
    printf("Tcrit: 0x%02x%02x\r\n",I2CWorkRegData[0],I2CWorkRegData[1]);
    
    return;
}
