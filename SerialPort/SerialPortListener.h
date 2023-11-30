#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSerialPortInfo>
#include <QModbusDataUnit>
#include <QModbusDevice>
#include <QModbusRtuSerialMaster>
#include <QSerialPort>

//Create object ModBus RTU masters
QModbusRtuSerialClient *modbusMaster = new QModbusRtuSerialClient();

void MainWindow::UpdateListCOMPorts(){
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    ui->cmbx_ListSerialPorts->clear();
    for(const QSerialPortInfo &portInfo : serialPortInfos){
        qDebug() << portInfo.portName();
        ui->cmbx_ListSerialPorts->addItem(portInfo.portName());
    }
}

void MainWindow::ConnectedModbusDevice(){
    modbusMaster->setConnectionParameter(QModbusDevice::SerialPortNameParameter, "COM8");
    modbusMaster->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, QSerialPort::Baud9600);
    modbusMaster->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
    modbusMaster->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);
    modbusMaster->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::NoParity);
    modbusMaster->setTimeout(100);
    modbusMaster->setNumberOfRetries(3);

    if (!modbusMaster->connectDevice()){
        qDebug()<< "Error connected!" << modbusMaster->errorString();
        //modbusMaster->disconnectDevice();
        //delete modbusMaster;
    }

    int deviceAddress = 10;
    int startRegisterAddress = 0;
    int countReadRegister = 2;

    QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, startRegisterAddress, countReadRegister);

    if (auto *reply = modbusMaster->sendReadRequest(readUnit, deviceAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, [=](){

                if (!reply)
                    return;

                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit data = reply->result();
                    for (int i = 0; i < data.valueCount(); i++) {
                        qDebug() << "Value:" << data.value(i);
                    }
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    qDebug() << "Modbus protocol error:" << reply->errorString();
                } else {
                    qDebug() << "Modbus reply error:" << reply->errorString();
                }
                reply->deleteLater();
            });
        }
        else{
            delete reply;
        }
    } else {
        qDebug() << "Failed to send Modbus request:" << modbusMaster->errorString();
    }
}

void MainWindow::ResponseModbusDevice(){
    //auto reply = qobject_cast<QModbusReply *>(sender());


}
