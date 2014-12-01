#include "mainwindow.h"
#include "ui_mainwindow.h"

 //YOOOOOOOOOOOO
imu_t imu;
att_t att;
p_t p;
rc_t rc;
motor_t motor;
settings_t settings;
status_t status;
loopTime_t loopTime;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serialTelegram = new Telegram;

    connectStatus = false;

    connect(ui->connectButton,SIGNAL(clicked()),this,SLOT(Connect()));
    connect(serialTelegram,SIGNAL(UpdateStructs()),this,SLOT(UpdateStructs()));
    connect(serialTelegram,SIGNAL(ReceivedPid()),this,SLOT(ReceivedPid()));



    GetPorts();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Connect(void)
{
    if(connectStatus == true)
    {
        serialTelegram->Disconnect();
        ui->connectButton->setText("Connect");
        ui->statusBar->showMessage(tr("Disconnected!"));
        connectStatus = false;
    }
    else
    {
        QString port = ui->portBox->currentText();
        int baud = ui->baudBox->currentText().toInt();

        connectStatus= serialTelegram->Connect(port,baud);

        if(connectStatus)
        {

            ui->connectButton->setText("Disconnect");
            ui->statusBar->showMessage((tr("Connected! Port %1 Baud %2")
                                        .arg(port)
                                        .arg(baud)));
        }
        else
        {
            ui->statusBar->showMessage((tr("Failed to open Port %1 Baud %2")
                                        .arg(port)
                                        .arg(baud)));
        }
    }
}


void MainWindow::GetPorts(void)
{
    QStringList portList;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        portList << info.portName();
    }
    ui->portBox->addItems(portList);
}

void MainWindow::on_getAttButton_clicked()
{
    serialTelegram->Request(ATT_CMD,ReadDirection);
}

void MainWindow::on_getIMUButton_clicked()
{
    serialTelegram->Request(IMU_CMD,ReadDirection);
}

void MainWindow::on_caliAccButton_clicked()
{
    status.accDoCalibrate = 1;
    serialTelegram->Request(STATUS_CMD,WriteDirection);
}

void MainWindow::on_caliMagButton_clicked()
{
    status.magDoCalibrate = 1;
    serialTelegram->Request(STATUS_CMD,WriteDirection);
}

void MainWindow::on_getSettingsButton_clicked()
{
    serialTelegram->Request(EEPROM_CMD, ReadDirection);
}

void MainWindow::on_writePidButton_clicked()
{
    p.rollPID.kP = ui->rollPidkP->text().toFloat();
    p.rollPID.kI = ui->rollPidkI->text().toFloat();
    p.rollPID.kD = ui->rollPidkD->text().toFloat();

    p.pitchPID.kP = ui->pitchPidkP->text().toFloat();
    p.pitchPID.kI = ui->pitchPidkI->text().toFloat();
    p.pitchPID.kD = ui->pitchPidkD->text().toFloat();

    p.yawPID.kP = ui->yawPidkP->text().toFloat();
    p.yawPID.kI = ui->yawPidkI->text().toFloat();
    p.yawPID.kD = ui->yawPidkD->text().toFloat();
    p.iMax = ui->PidiMax->text().toUShort();
    serialTelegram->Request(PID_CMD, WriteDirection);
}

void MainWindow::on_getPidButton_clicked()
{
    serialTelegram->Request(PID_CMD, ReadDirection);
}

void MainWindow::on_getMotorButton_clicked()
{
    serialTelegram->Request(MOTOR_CMD, ReadDirection);
}

void MainWindow::on_getRcButton_clicked()
{
    serialTelegram->Request(RC_CMD, ReadDirection);
}

void MainWindow::UpdateStructs(void)
{
    //IMU
    ui->accRollLine->setText(QString::number(imu.accADC[ROLL]));
    ui->accPitchLine->setText(QString::number(imu.accADC[PITCH]));
    ui->accYawLine->setText(QString::number(imu.accADC[YAW]));

    ui->gyroRollLine->setText(QString::number(imu.gyroADC[ROLL]));
    ui->gyroPitchLine->setText(QString::number(imu.gyroADC[PITCH]));
    ui->gyroYawLine->setText(QString::number(imu.gyroADC[YAW]));

    ui->magRollLine->setText(QString::number(imu.magADC[ROLL]));
    ui->magPitchLine->setText(QString::number(imu.magADC[PITCH]));
    ui->magYawLine->setText(QString::number(imu.magADC[YAW]));

    //ATT
    float rollAngle = (att.angle[ROLL] / 10);
    float pitchAngle = (att.angle[PITCH] / 10);
    ui->attRollLine->setText(QString::number(rollAngle));
    ui->attPitchLine->setText(QString::number(pitchAngle));
    ui->attHeadingLine->setText(QString::number(att.heading));

    //Motor
    ui->frontMotor->setText(QString::number(motor.front));
    ui->backMotor->setText(QString::number(motor.back));
    ui->leftMotor->setText(QString::number(motor.left));
    ui->rightMotor->setText(QString::number(motor.right));

    //RC
    ui->rcRoll->setText(QString::number(rc.roll));
    ui->rcPitch->setText(QString::number(rc.pitch));
    ui->rcYaw->setText(QString::number(rc.yaw));
    ui->rcThrottle->setText(QString::number(rc.throttle));
    ui->rcAux1->setText(QString::number(rc.aux1));
    ui->rcAux2->setText(QString::number(rc.aux2));
    ui->rcAux3->setText(QString::number(rc.aux3));
    ui->rcAux4->setText(QString::number(rc.aux4));

    //EEPROM
    ui->accCaliRoll->setText(QString::number(settings.accOffset[ROLL]));
    ui->accCaliPitch->setText(QString::number(settings.accOffset[PITCH]));
    ui->accCaliYaw->setText(QString::number(settings.accOffset[YAW]));

    ui->magCaliRoll->setText(QString::number(settings.magOffset[ROLL]));
    ui->magCaliPitch->setText(QString::number(settings.magOffset[PITCH]));
    ui->magCaliYaw->setText(QString::number(settings.magOffset[YAW]));

    //STATUS
    ui->loopTime->setText(QString::number(loopTime.time));

    ui->armedStatus->setText(QString::number(status.armed));
}

void MainWindow::ReceivedPid(void)
{
    ui->rollPidkP->setText(QString::number(p.rollPID.kP));
    ui->rollPidkI->setText(QString::number(p.rollPID.kI));
    ui->rollPidkD->setText(QString::number(p.rollPID.kD));

    ui->pitchPidkP->setText(QString::number(p.pitchPID.kP));
    ui->pitchPidkI->setText(QString::number(p.pitchPID.kI));
    ui->pitchPidkD->setText(QString::number(p.pitchPID.kD));

    ui->yawPidkP->setText(QString::number(p.yawPID.kP));
    ui->yawPidkI->setText(QString::number(p.yawPID.kI));
    ui->yawPidkD->setText(QString::number(p.yawPID.kD));

    ui->PidiMax->setText(QString::number(p.iMax));
}


void MainWindow::on_statusButton_clicked()
{
    serialTelegram->Request(STATUS_CMD,ReadDirection);
}

void MainWindow::on_pushButton_clicked()
{
    serialTelegram->Request(LOOPT_CMD,ReadDirection);
}
