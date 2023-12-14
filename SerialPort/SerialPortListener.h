#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QModbusDataUnit>
#include <QModbusDevice>
#include <QModbusRtuSerialMaster>
#include <QSerialPort>
//Create object ModBus RTU masters
QModbusRtuSerialClient *modbusMaster = new QModbusRtuSerialClient();

void MainWindow::UpdateListCOMPorts(){
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    ui->cmbx_listSerialPorts->clear();

    for(const QSerialPortInfo &portInfo : serialPortInfos){
        ui->cmbx_listSerialPorts->addItem(portInfo.portName());
        serialPortParametersList.append(portInfo.portName());
        ui->txtbrw_logBrowser->append("Found Serial Posrt`s: " + portInfo.portName());
    }
}

void MainWindow::ConnectedModbusDevice(){
    modbusMaster->setConnectionParameter(QModbusDevice::SerialPortNameParameter, "COM9");
    modbusMaster->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, baudrate);
    modbusMaster->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
    modbusMaster->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);
    modbusMaster->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::NoParity);
    modbusMaster->setTimeout(100);
    modbusMaster->setNumberOfRetries(3);
    int deviceAddress = 10;
    int startRegisterAddress = 0;
    int countReadRegister = 2;

    QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, startRegisterAddress, countReadRegister);

    if (!modbusMaster->connectDevice()){
        ui->txtbrw_logBrowser->append("Error connected! - Device not found or not connected.");
        modbusMaster->disconnectDevice();
        return;
    }
    if (auto *reply = modbusMaster->sendReadRequest(readUnit, deviceAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, [=](){
                if (!reply)
                    return;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit data = reply->result();
                    for (int i = 0; i < data.valueCount(); i++) {
                        ui->txtbrw_logBrowser->append(QString::number(data.value(i)));
                        modbusMaster->disconnectDevice();
                    }
                }
                else if (reply->error() == QModbusDevice::ProtocolError) {
                    ui->txtbrw_logBrowser->append("Modbus protocol error:" + reply->errorString());
                }
                else {
                    ui->txtbrw_logBrowser->append("Modbus reply error:" + reply->errorString());
                }
                reply->deleteLater();
            });
        }
        else{
            delete reply;
        }
    }
    else {
        ui->txtbrw_logBrowser->append("Failed to send Modbus request: " + modbusMaster->errorString());
        modbusMaster->disconnectDevice();
        //delete modbusMaster;
    }
}

void MainWindow::ResponseModbusDevice(){
    //auto reply = qobject_cast<QModbusReply *>(sender());

}
