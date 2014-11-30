#include "telegram.h"

Telegram::Telegram(QObject *parent) :
    QObject(parent)
{
    serialProtocol = new Protocol;
    connect(serialProtocol,SIGNAL(FrameComplete(quint8&,quint8&,QByteArray&)),
            this,SLOT(Process(quint8&,quint8&,QByteArray&)));
}

bool Telegram::Connect(QString port, int baud)
{
    bool status = serialProtocol->Connect(port,baud);
    return status;
}

void Telegram::Disconnect(void)
{
    serialProtocol->Disconnect();
}

void Telegram::Request(char command, char direction)
{

    char mycommand = command;
    QByteArray payload;
    char dataArray[64];

    switch (command)
    {
    case IMU_CMD:
        serialProtocol->CreateFrameAndSend(mycommand,direction,payload);
        break;
    case ATT_CMD:
        serialProtocol->CreateFrameAndSend(mycommand,direction,payload);
        break;
    case PID_CMD:
        if(direction == WriteDirection)
        {
            memcpy(dataArray,&p,sizeof(p));
            payload.append(dataArray,sizeof(p));
        }
        serialProtocol->CreateFrameAndSend(mycommand,direction,payload);
        break;
    case RC_CMD:
        serialProtocol->CreateFrameAndSend(mycommand,direction,payload);
        break;
    case MOTOR_CMD:
        serialProtocol->CreateFrameAndSend(mycommand,direction,payload);
        break;
    case EEPROM_CMD:
        serialProtocol->CreateFrameAndSend(mycommand,direction,payload);
        break;
    case STATUS_CMD:
        if(direction == WriteDirection)
        {
           memcpy(dataArray,&status,sizeof(status));
           payload.append(dataArray,sizeof(status));
        }
        serialProtocol->CreateFrameAndSend(mycommand,direction,payload);
        break;

    case LOOPT_CMD:
        serialProtocol->CreateFrameAndSend(mycommand,direction,payload);
        break;
    }
}

void Telegram::Process(quint8& command, quint8& direction, QByteArray& payload)
{
    if(direction == Ack)
    {
        qDebug() << "Ack from " << command;
        return; // No payload
    }

    switch (command)
    {
    case IMU_CMD:
        memcpy(&imu,payload.constData(),payload.size());
        break;

    case ATT_CMD:
        memcpy(&att,payload.constData(),payload.size());
        break;

    case PID_CMD:
        memcpy(&p,payload.constData(),payload.size());
        emit ReceivedPid();
        return;
        break;

    case RC_CMD:
        memcpy(&rc,payload.constData(),payload.size());
        break;

    case MOTOR_CMD:
        memcpy(&motor,payload.constData(),payload.size());
        break;

    case EEPROM_CMD:
        memcpy(&settings,payload.constData(),payload.size());
        break;

    case STATUS_CMD:
        memcpy(&status,payload.constData(),payload.size());
        break;

    case LOOPT_CMD:
        memcpy(&loopTime,payload.constData(),payload.size());
        break;
    }

    if(direction == WriteDirection)
    {
        emit UpdateStructs();
    }
}

