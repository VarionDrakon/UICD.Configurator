#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include "ConnectionParametersModbus.h"
#include <QModbusDevice>
#include <QModbusDataUnit>
void MainWindow::WriteModbusDevice(){
    modbusMaster = new QModbusRtuSerialClient();
    modbusMaster->setConnectionParameter(QModbusDevice::SerialPortNameParameter, nameSerialPort);
    modbusMaster->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, baudrate);
    modbusMaster->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, dataBits);
    modbusMaster->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, stopBits);
    modbusMaster->setConnectionParameter(QModbusDevice::SerialParityParameter, parityBits);
    modbusMaster->setTimeout(100);
    modbusMaster->setNumberOfRetries(3);
    //int deviceAddress = 10;
    int startRegisterAddress = 0;
    int countReadRegister = 9;

    QModbusDataUnit writeUnit(QModbusDataUnit::HoldingRegisters, startRegisterAddress, countReadRegister);

    if (!modbusMaster->connectDevice()){
        ui->txtbrw_logBrowser->append("Error connected! - Device not found or not connected.");
        modbusMaster->disconnectDevice();
        return;
    }
    writeUnit.setValue(0, slaveAddressBits);
    writeUnit.setValue(1, baudrate);
    if (auto *reply = modbusMaster->sendWriteRequest(writeUnit, slaveAddressBits)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, [=](){
                if (!reply)
                    return;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit data = reply->result();

                }
                else if (reply->error() == QModbusDevice::ProtocolError) {
                    ui->txtbrw_logBrowser->append("Modbus protocol error:" + reply->errorString());
                }
                else {
                    ui->txtbrw_logBrowser->append("Modbus reply error:" + reply->errorString());
                }
                reply->deleteLater();
                modbusMaster->disconnectDevice();
                //delete modbusRegisterAnswer;
            });
        }
        else{
            delete reply;
        }
    }
    else {
        ui->txtbrw_logBrowser->append("Failed to send Modbus request: " + modbusMaster->errorString());
        modbusMaster->disconnectDevice();
    }
}
