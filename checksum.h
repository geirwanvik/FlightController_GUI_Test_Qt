#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <QtGlobal>

#define P_CCITT 0x1021

class Checksum
{
// public functions
public:
    Checksum();
    void UpdateCRC(quint8 data);
    void ClearCRC(void);
    quint16 GetCRC(void);
};

#endif // CHECKSUM_H
