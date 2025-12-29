#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#ifdef QT_SERIALPORT_AVAILABLE
#include <QSerialPort>
#include <QSerialPortInfo>
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_RefreshPorts_clicked();
    void on_pushButton_Connect_clicked();
    void on_pushButton_Disconnect_clicked();
    void on_pushButton_LED_ON_clicked();
    void on_pushButton_LED_OFF_clicked();
    void on_pushButton_ClearDebug_clicked();
    void serialDataReceived();
#ifdef QT_SERIALPORT_AVAILABLE
    void serialErrorOccurred(QSerialPort::SerialPortError error);
#endif
    void updateUptime();

private:
    Ui::MainWindow *ui;
#ifdef QT_SERIALPORT_AVAILABLE
    QSerialPort *serialPort;
#endif
    QTimer *uptimeTimer;
    
    void updateSerialPorts();
    void updateUI(bool connected);
    void sendCommand(const QString &command);
    void appendDebug(const QString &text, const QString &type = "INFO");
};

#endif // MAINWINDOW_H
