#ifndef __CHUN_AT24C04_H_
#define __CHUN_AT24C04_H_

#include <Arduino.h>

class AT24C04
{
public:
    AT24C04(byte deviceAddress);


    void initialize();

   
    void writeByte(word address, byte data);

    void writeBytes(word address, word length, byte *p_data);

   
    byte readByte(word address);

   
    void readBytes(word address, word length, byte *p_buffer);

private:
    byte m_deviceAddress;

    void writePage(word address, byte length, byte *p_data);

    void writeBuffer(word address, byte length, byte *p_data);

    void readBuffer(word address, byte length, byte *p_data);
};

#endif // __CHUN_AT24C04_H_
