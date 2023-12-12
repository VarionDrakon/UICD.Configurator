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
    void ResponseModbusDevice();
    void ConnectedModbusDevice();

    const QList<QSerialPort::BaudRate> baudrateParametersList = QList<QSerialPort::BaudRate>()
        << QSerialPort::Baud1200
        << QSerialPort::Baud2400
        << QSerialPort::Baud4800
        << QSerialPort::Baud9600
        << QSerialPort::Baud19200
        << QSerialPort::Baud38400
        << QSerialPort::Baud57600
        << QSerialPort::Baud115200;

    QList<QString> serialPortParametersList;

signals:
    //void interfaceLoaded();

private slots:
    //void successInterfaceLoaded();
    void on_btn_scaningExistSerialPorts_clicked();

    void on_btn_readChooserDevice_clicked();

    void on_cmbx_listSerialPorts_currentIndexChanged(int index);

    void on_cmbx_listBaudrate_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    int baudrate;
    QString nameSerialPort;
    bool isAppSucessStartup = false;
};
#endif // MAINWINDOW_H
