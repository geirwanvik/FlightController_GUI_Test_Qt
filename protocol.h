#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <QObject>
#include <QtSerialPort>
#include <QMessageBox>
#include <QTimer>
#include "checksum.h"

class Protocol : public QObject
{
    Q_OBJECT

//Public variabels
public:
void GetFrameData(quint8& command, QByteArray& payload);


//Private variabels
private:
    enum FrameState { Start,Direction,Length,Command,Payload,CRC,End };
    enum Identifier { StartByte = '#', NoPayload = 0x00 };
    struct FrameData { quint8 start; quint8 direction; quint8 command; quint8 length;
                       QByteArray payload; quint16 crc; };
    FrameData incomingFrame;
    QSerialPort *serialPort;
    Checksum *checksum;
    quint8 frameCounter;

//Public functions
public:
        Protocol();
        ~Protocol();

//Private functions
private:
        FrameState CaseStart(quint8 data);
        FrameState CaseDirection(quint8 data);
        FrameState CaseCommand(quint8 data);
        FrameState CaseLength(quint8 data);
        FrameState CasePayload(quint8 data);
        FrameState CaseCRC(quint8 data);
        FrameState CheckCRC(void);


public slots:
bool Connect(QString port, int baud);
void Disconnect(void);
void CreateFrameAndSend(char &command, char &direction, QByteArray& payload);

private slots:
    void ReadFrame(void);


signals:
    void FrameComplete(quint8& command, quint8& direction, QByteArray& payload);

};






#endif // PROTOCOL_H
