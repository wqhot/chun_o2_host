#include <Arduino.h>
#include <Wire.h>

#include <at24c04.h>

#define EEPROM__PAGE_SIZE 16

#define EEPROM__RD_BUFFER_SIZE BUFFER_LENGTH

#define EEPROM__WR_BUFFER_SIZE (BUFFER_LENGTH - 1)

AT24C04::AT24C04(byte deviceAddress)
{
    m_deviceAddress = deviceAddress;
}

void AT24C04::initialize()
{
    Wire.begin();
}

void AT24C04::writeByte(word address, byte data)
{
    Wire.beginTransmission((byte)(m_deviceAddress | ((address >> 8) & 0x07)));
    Wire.write(address & 0xFF);
    Wire.write(data);
    Wire.endTransmission();
}

void AT24C04::writeBytes(word address, word length, byte *p_data)
{
    // Write first page if not aligned.
    byte notAlignedLength = 0;
    byte pageOffset = address % EEPROM__PAGE_SIZE;
    if (pageOffset > 0)
    {
        notAlignedLength = EEPROM__PAGE_SIZE - pageOffset;
        writePage(address, notAlignedLength, p_data);
        length -= notAlignedLength;
    }

    if (length > 0)
    {
        address += notAlignedLength;
        p_data += notAlignedLength;

        // Write complete and aligned pages.
        byte pageCount = length / EEPROM__PAGE_SIZE;
        for (byte i = 0; i < pageCount; i++)
        {
            writePage(address, EEPROM__PAGE_SIZE, p_data);
            address += EEPROM__PAGE_SIZE;
            p_data += EEPROM__PAGE_SIZE;
            length -= EEPROM__PAGE_SIZE;
        }

        if (length > 0)
        {
            // Write remaining uncomplete page.
            writePage(address, EEPROM__PAGE_SIZE, p_data);
        }
    }
}

byte AT24C04::readByte(word address)
{
    Wire.beginTransmission((byte)(m_deviceAddress | ((address >> 8) & 0x07)));
    Wire.write(address & 0xFF);
    Wire.endTransmission();
    Wire.requestFrom((byte)(m_deviceAddress | ((address >> 8) & 0x07)), (byte)1);
    byte data = 0;
    if (Wire.available())
    {
        data = Wire.read();
    }
    return data;
}

void AT24C04::readBytes(word address, word length, byte *p_data)
{
    byte bufferCount = length / EEPROM__RD_BUFFER_SIZE;
    for (byte i = 0; i < bufferCount; i++)
    {
        word offset = i * EEPROM__RD_BUFFER_SIZE;
        readBuffer(address + offset, EEPROM__RD_BUFFER_SIZE, p_data + offset);
    }

    byte remainingBytes = length % EEPROM__RD_BUFFER_SIZE;
    word offset = length - remainingBytes;
    readBuffer(address + offset, remainingBytes, p_data + offset);
}

void AT24C04::writePage(word address, byte length, byte *p_data)
{
    // Write complete buffers.
    byte bufferCount = length / EEPROM__WR_BUFFER_SIZE;
    for (byte i = 0; i < bufferCount; i++)
    {
        byte offset = i * EEPROM__WR_BUFFER_SIZE;
        writeBuffer(address + offset, EEPROM__WR_BUFFER_SIZE, p_data + offset);
    }

    // Write remaining bytes.
    byte remainingBytes = length % EEPROM__WR_BUFFER_SIZE;
    byte offset = length - remainingBytes;
    writeBuffer(address + offset, remainingBytes, p_data + offset);
}

void AT24C04::writeBuffer(word address, byte length, byte *p_data)
{
    Wire.beginTransmission((byte)(m_deviceAddress | ((address >> 8) & 0x07)));
    Wire.write(address & 0xFF);
    for (byte i = 0; i < length; i++)
    {
        Wire.write(p_data[i]);
    }
    Wire.endTransmission();

    // Write cycle time (tWR). See EEPROM memory datasheet for more details.
    delay(10);
}

void AT24C04::readBuffer(word address, byte length, byte *p_data)
{
    Wire.beginTransmission((byte)(m_deviceAddress | ((address >> 8) & 0x07)));
    Wire.write(address & 0xFF);
    Wire.endTransmission();
    Wire.requestFrom((byte)(m_deviceAddress | ((address >> 8) & 0x07)), length);
    for (byte i = 0; i < length; i++)
    {
        if (Wire.available())
        {
            p_data[i] = Wire.read();
        }
    }
}
