#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModbusDataUnit>
#include <QModbusRtuSerialMaster>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void UpdateListCOMPorts();
    QModbusDataUnit readRequest() const;

private slots:
    void on_pushButton_clicked();
    void onStateChanged(int state);
    void on_connectButton_clicked();
    void on_readButton_clicked();

private:
    Ui::MainWindow *ui;
    QModbusClient *modbusDevice;
    void init();
    void readReady();

    const int SERVER_ADDRESS = 10;
    const int START_ADDRESS = 0;
    const int AMOUNT = 2;
    const int WRITE_SIZE = 1;

    const int BAUDS = 9600;
    const int STOP_BITS = 1;
    const int DATA_BITS = 8;
    const QString PORT = "COM8";
    const int PARITY = QSerialPort::NoParity;
    const int RESPONSE_TIME = 100;
};
#endif // MAINWINDOW_H
