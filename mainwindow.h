#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "CommandLibrary.h"
#include "telegram.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Telegram *serialTelegram;
    bool connectStatus;

private slots:
    void Connect(void);
    void GetPorts(void);
    void on_getAttButton_clicked();
    void on_getIMUButton_clicked();
    void on_caliAccButton_clicked();
    void on_caliMagButton_clicked();
    void on_getSettingsButton_clicked();
    void on_writePidButton_clicked();
    void on_getPidButton_clicked();
    void on_getMotorButton_clicked();
    void on_getRcButton_clicked();

    void UpdateStructs(void);
    void ReceivedPid(void);
    void on_statusButton_clicked();
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
