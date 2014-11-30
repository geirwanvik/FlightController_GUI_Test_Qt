#include "protocol.h"


Protocol::Protocol()
{
    serialPort = new QSerialPort;
    connect(serialPort,SIGNAL(readyRead()),this,SLOT(ReadFrame()));

    checksum = new Checksum;

}

Protocol::~Protocol()
{
    delete serialPort;
    delete checksum;
}

bool Protocol::Connect(QString port, int baud)
{
    serialPort->setPortName(port);
    serialPort->setBaudRate(baud);
    if(serialPort->open(QIODevice::ReadWrite))
    {
        return true;
    }
        return false;
}

void Protocol::Disconnect(void)
{
    serialPort->close();
}

void Protocol::ReadFrame(void)
{
    static FrameState state;
    do
    {
        char data;
        serialPort->read(&data,1);
        switch(state)
        {
        case Start:
            state = CaseStart(data);
            break;
        case Direction:
            state = CaseDirection(data);
            break;
        case Command:
            state = CaseCommand(data);
            break;
        case Length:
            state = CaseLength(data);
            break;
        case Payload:
            state = CasePayload(data);
            if(state == Payload)
            { break; }
        case CRC:
            state = CaseCRC(data);
            if( state == CRC)
            { break; }
        case End:
            state = CheckCRC();
            break;
        }
    }
    while(serialPort->bytesAvailable());
}


Protocol::FrameState Protocol::CaseStart(quint8 data)
{
    if(data == StartByte)
    {
        incomingFrame.start = data;
        checksum->ClearCRC();
        checksum->UpdateCRC(data);
        return Direction;
    }
    return Start;
}

Protocol::FrameState Protocol::CaseDirection(quint8 data)
{
    incomingFrame.direction = data;
    checksum->UpdateCRC(data);
    return Command;
}

Protocol::FrameState Protocol::CaseCommand(quint8 data)
{
    incomingFrame.command = data;
    checksum->UpdateCRC(data);
    return Length;
}

Protocol::FrameState Protocol::CaseLength(quint8 data)
{
    incomingFrame.length = data;
    incomingFrame.payload.clear();
    checksum->UpdateCRC(data);
    return Payload;
}

Protocol::FrameState Protocol::CasePayload(quint8 data)
{
    if(incomingFrame.payload.size() == incomingFrame.length)
    {
        return CRC;
    }
    else
    {
        incomingFrame.payload.append(data);
        checksum->UpdateCRC(data);
    }
    return Payload;
}

Protocol::FrameState Protocol::CaseCRC(quint8 data)
{
    static int counter = 0;

    if(counter == 0)
    {
        incomingFrame.crc = (quint8)data;
        counter++;
        return CRC;
    }

    else if(counter == 1)
    {
        incomingFrame.crc = incomingFrame.crc | ( (quint8) data << 8);
        counter = 0;
    }
    return End;
}

Protocol::FrameState Protocol::CheckCRC(void)
{
    if(incomingFrame.crc == checksum->GetCRC())
    {
        emit FrameComplete(incomingFrame.command,incomingFrame.direction, incomingFrame.payload);
    }
    return Start;
}

void Protocol::CreateFrameAndSend(char& command, char& direction, QByteArray& payload)
{

    QByteArray sendArray;
    sendArray.append(StartByte);
    sendArray.append(direction);
    sendArray.append(command);
    sendArray.append(payload.size());
    sendArray.append(payload);

    checksum->ClearCRC();
    for(short i = 0; i< sendArray.size(); i++)
    {
        checksum->UpdateCRC(sendArray.at(i));
    }
    quint16 crc = checksum->GetCRC();

    sendArray.append(crc & 0xFF);
    sendArray.append(crc >> 8);

    serialPort->write(sendArray);
}


