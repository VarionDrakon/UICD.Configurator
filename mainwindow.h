#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void LocalParametersInitilizatedOnStartup();

    const QList<QSerialPort::BaudRate> baudrateParameters = QList<QSerialPort::BaudRate>()
        << QSerialPort::Baud1200
        << QSerialPort::Baud2400
        << QSerialPort::Baud4800
        << QSerialPort::Baud9600
        << QSerialPort::Baud19200
        << QSerialPort::Baud38400
        << QSerialPort::Baud57600
        << QSerialPort::Baud115200;

    QList<QString> serialPortParameters;

private slots:
    void ResponseModbusDevice();
    void ConnectedModbusDevice();
    void on_btn_scaningExistSerialPorts_clicked();

    void on_btn_readChooserDevice_clicked();

    void on_cmbx_listSerialPorts_currentIndexChanged(int index);

    void on_cmbx_listBaudrate_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    int baudrate;
    QString nameSerialPort;
};
#endif // MAINWINDOW_H
