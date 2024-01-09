#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QModbusDataUnit>
#include <QModbusDevice>
#include <QModbusRtuSerialMaster>
#include <QStandardItemModel>
#include <QTranslator>
#include <QStyledItemDelegate>
#include <QAbstractItemModel>
#include <QList>
#include <QString>
#include <QComboBox>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QFile>
#include <QIcon>
#include <QDesktopServices>
#include <QApplication>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateListCOMPorts();
    void localParametersInitilizatedOnStartup();
    void responseModbusDevice();
    void modbusDataReader();
    void modbusDataWriter();
    void tableDataHandler();
    void parseModbusResponse();
    void parametersModbusDevice();
    void setupModbusParameters();
    //Create object Modbus RTU Answer
    QList<int> *modbusRegisterAnswer = new QList<int>;
    QList<unsigned int> *modbusParseAnswer = new QList<unsigned int>;
    QComboBox *cmbxBaudrate;
    //Create object Modbus RTU masters
    QModbusRtuSerialClient *modbusMaster = new QModbusRtuSerialClient();

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

    QList<QString> listThemeApp = QList<QString>()
        << ":/qss/darkThemeApp.qss"
        << ":/qss/lightThemeApp.qss";
/* Pattern for resizeEvent handler...
protected:
    void resizeEvent(QResizeEvent *event) override {
        QSize newSize = event->size();
    }
*/
signals:
    void cmbx_listSerialPorts_currentIndexChanged(int index);

private slots:
    void on_btn_scaningExistSerialPorts_clicked();

    void on_cmbx_listSerialPorts_currentIndexChanged(int index);

    void on_cmbx_listBaudrate_currentIndexChanged(int index);

    void on_cmbx_listDataBits_currentIndexChanged(int index);

    void on_cmbx_listStopBits_currentIndexChanged(int index);

    void on_cmbx_listParity_currentIndexChanged(int index);

    void on_spnbx_listSlaveID_valueChanged(int arg1);

    void on_btn_readChooserDevice_clicked();

    void on_btn_writeChooserDevice_clicked();

    void on_btnChangeTheme_clicked();

    void on_btnOpenWebCompany_clicked();

    void on_btnOpenWebRepository_clicked();

private:
    Ui::MainWindow *ui;
    QPixmap *pixmapOurLogoCompany;

    int indexValue = -1;
    int baudrate;
    int dataBits;
    int stopBits;
    int parityBits;
    int slaveAddressBits;
    QString nameSerialPort;
    int currentThemeApp = 1;
};
#endif // MAINWINDOW_H
