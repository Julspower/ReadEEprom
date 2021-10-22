#include <QCoreApplication>
#include <QThread>
#include "bcm2835.h"


void WriteReg(char cReg, char cData)
{
    char cRet;
    char bufOut[2];

    bufOut[0] = cReg;
    bufOut[1] = cData;

    do
    {
        cRet = bcm2835_i2c_write(bufOut, 2);
        if(cRet != 0)
        {
            delay(100);
            bcm2835_i2c_end();
            bcm2835_i2c_begin();
            bcm2835_i2c_setSlaveAddress(0x28);
            bcm2835_i2c_set_baudrate(400000);
        }
    }while(cRet != 0);
}

char ReadReg(char cReg)
{
    char cRet;
    char bufIn;

    do
    {
        cRet = bcm2835_i2c_write_read_rs(&cReg, 1, &bufIn, 1);
        if(cRet != 0)
        {
            delay(100);
            bcm2835_i2c_end();
            bcm2835_i2c_begin();
            bcm2835_i2c_setSlaveAddress(0x28);
            bcm2835_i2c_set_baudrate(400000);
        }
    }while(cRet != 0);
    return bufIn;
}

int main(int argc, char *argv[])
{
    char data[140];
    int i;
    int nReturn;

    nReturn = bcm2835_init();
    if(nReturn)
    {
        bcm2835_i2c_setSlaveAddress(0x28);
        bcm2835_i2c_set_baudrate(400000);

        // First put SENtral in standby mode
        WriteReg(0x34, 0x01);
        while(1)
        {
  //          cRet = bcm2835_i2c_write_read_rs(&cAdr, 1, bufIn, 1);
  //          nReturn = wiringPiI2CReadReg16(fd, cAdr);
            WriteReg(0x54, 0x00);
            delay(5000);
            printf("1- %d - %d - %d\n\r", ReadReg(0x38), ReadReg(0x54), ReadReg(0x92));
            fflush(stdout);
            WriteReg(0x54, 0x01);
            delay(5000);
            printf("2- %d - %d - %d\n\r", ReadReg(0x38), ReadReg(0x54), ReadReg(0x92));
            fflush(stdout);
        }
        /*
        bufOut[0] = 0x54;
        bufOut[1] = 0x01;
        cRet = bcm2835_i2c_write(bufOut, 2);
        while(true)
        {
            delay(5);
            cAdr = 0x38;
            cRet = bcm2835_i2c_write_read_rs(&cAdr, 1, bufIn, 1);
            cAdr = 0x35;
            if (bufIn[0] & 0x01)
                break;
            cRet = bcm2835_i2c_write(bufOut, 2);
        }

        // Place SENtral in pass-through mode
        bufOut[0] = 0xA0;
        bufOut[1] = 0x01;
        cRet = bcm2835_i2c_write(bufOut, 2);
        while(true)
        {
            delay(5);
            cRet = bcm2835_i2c_write_read_rs(&cAdr, 1, bufIn, 1);
            if (bufIn[0] & 0x01)
                break;
            cRet = bcm2835_i2c_write(bufOut, 2);
        }


        bufOut[0] = 0;
        bufOut[1] = 0;
        bcm2835_i2c_setSlaveAddress(0x50);
        bcm2835_i2c_write_read_rs(bufOut, 2, bufIn, 128);
        for(i=0;i<128;i++)
            printf("%#02X, ", bufIn[i]);
        printf("\n");*/
        bcm2835_i2c_end();
        bcm2835_close();
    }
    else
        printf("Init Failed\n");
}
