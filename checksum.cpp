#include "checksum.h"

//    Optimized CRC-XMODEM calculation.
//    Polynomial: x^16 + x^12 + x^5 + 1 (0x1021)
//    Initial value: 0x0
//    This is the CRC used by the Xmodem-CRC protocol.

quint16 crc = 0;

Checksum::Checksum() {}

void Checksum::UpdateCRC(quint8 data)
{
    int i;
    crc = crc ^ ((quint16)data << 8);
    for (i=0; i<8; i++)
    {
            if (crc & 0x8000)
                    crc = (crc << 1) ^ 0x1021;
            else
                    crc <<= 1;
    }
}

void Checksum::ClearCRC(void)
{
    crc = 0;
}

quint16 Checksum::GetCRC(void)
{
    return crc;
}
