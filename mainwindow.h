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
    void ParseModBusAnswer();

    const QList<QSerialPort::BaudRate> parametersListBaudrate = QList<QSerialPort::BaudRate>()
        << QSerialPort::Baud1200
        << QSerialPort::Baud2400
        << QSerialPort::Baud4800
        << QSerialPort::Baud9600
        << QSerialPort::Baud19200
        << QSerialPort::Baud38400
        << QSerialPort::Baud57600
        << QSerialPort::Baud115200;
    const QList<QSerialPort::DataBits> parametersListDataBits = QList<QSerialPort::DataBits>()
        << QSerialPort::Data5
        << QSerialPort::Data6
        << QSerialPort::Data7
        << QSerialPort::Data8;
    const QList<QSerialPort::StopBits> parametersListStopBits = QList<QSerialPort::StopBits>()
        << QSerialPort::OneStop
        << QSerialPort::TwoStop;
    const QList<QSerialPort::Parity> parametersListParityBits = QList<QSerialPort::Parity>()
        << QSerialPort::NoParity
        << QSerialPort::EvenParity
        << QSerialPort::MarkParity
        << QSerialPort::OddParity
        << QSerialPort::SpaceParity;

    QList<QString> serialPortParametersList;

signals:
    void cmbx_listSerialPorts_currentIndexChanged(int index);

private slots:
    void on_btn_scaningExistSerialPorts_clicked();

    void on_btn_readChooserDevice_clicked();

    void on_cmbx_listSerialPorts_currentIndexChanged(int index);

    void on_cmbx_listBaudrate_currentIndexChanged(int index);

    void on_cmbx_listDataBits_currentIndexChanged(int index);

    void on_cmbx_listStopBits_currentIndexChanged(int index);

    void on_cmbx_listParity_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    int baudrate;
    int dataBits;
    int stopBits;
    int parityBits;
    QString nameSerialPort;
};
#endif // MAINWINDOW_H
