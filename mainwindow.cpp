#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SerialPort/SerialPortListener.h"
//Libs included

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), modbusDevice(nullptr)
{
    ui->setupUi(this);

    init();
}

MainWindow::~MainWindow()
{
    if(modbusDevice)
        modbusDevice->disconnectDevice();
    delete modbusDevice;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //UpdateListCOMPorts();
    on_connectButton_clicked();
    on_readButton_clicked();
}


void MainWindow::init() {
    modbusDevice = new QModbusRtuSerialMaster(this);
    connect(modbusDevice, &QModbusClient::errorOccurred, [this](QModbusDevice::Error) {
        statusBar()->showMessage(modbusDevice->errorString(), 5000);
    });

    if(modbusDevice) {
        connect(modbusDevice, &QModbusClient::stateChanged,
                this, &MainWindow::onStateChanged);
    }
}

void MainWindow::onStateChanged(int state) {
    if (state == QModbusDevice::UnconnectedState)
        qDebug() << "Connected";
    else if (state == QModbusDevice::ConnectedState)
        qDebug() << "Disconnected";
}

void MainWindow::on_connectButton_clicked()
{
    if (!modbusDevice)
        return;

    if (modbusDevice->state() != QModbusDevice::ConnectedState) {
        modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
                                             PORT);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,
                                             PARITY);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
                                             BAUDS);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
                                             DATA_BITS);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
                                             STOP_BITS);
        modbusDevice->setTimeout(RESPONSE_TIME);
        modbusDevice->setNumberOfRetries(1);
        if (!modbusDevice->connectDevice()) {
            qDebug() << "Error 1";
        }
    } else {
        modbusDevice->disconnectDevice();
    }
}

QModbusDataUnit MainWindow::readRequest() const
{
    return QModbusDataUnit(QModbusDataUnit::HoldingRegisters, START_ADDRESS, AMOUNT);
}
void MainWindow::readReady()
{
    auto lastRequest = qobject_cast<QModbusReply *>(sender());
    if (!lastRequest)
        return;

    if (lastRequest->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = lastRequest->result();
        for (uint i = 0; i < unit.valueCount(); i++) {
            const QString entry = tr("Address: %1, Value: %2").arg(unit.startAddress())
                                      .arg(QString::number(unit.value(i)));
            qDebug() << entry;
        }
    } else if (lastRequest->error() == QModbusDevice::ProtocolError) {
        statusBar()->showMessage(tr("Read response error: %1 (Mobus exception: 0x%2)").
                                 arg(lastRequest->errorString()).
                                 arg(lastRequest->rawResult().exceptionCode(), -1, 16), 5000);
    } else {
        statusBar()->showMessage(tr("Read response error: %1 (code: 0x%2)").
                                 arg(lastRequest->errorString()).
                                 arg(lastRequest->error(), -1, 16), 5000);
    }

    lastRequest->deleteLater();
}
void MainWindow::on_readButton_clicked()
{
    if (!modbusDevice)
        return;
    statusBar()->clearMessage();

    if (auto *lastRequest = modbusDevice->sendReadRequest(readRequest(), SERVER_ADDRESS)) {
        if (!lastRequest->isFinished())
            connect(lastRequest, &QModbusReply::finished, this, &MainWindow::readReady);
        else
            delete lastRequest; // broadcast replies return immediately
    } else {
        statusBar()->showMessage(tr("Read error: ") + modbusDevice->errorString(), 5000);
    }
}
