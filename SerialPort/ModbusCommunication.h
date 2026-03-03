#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "ModbusHandler.h"

void MainWindow::updateListCOMPorts(){
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    ui->cmbx_listSerialPorts->clear();

    for(const QSerialPortInfo &portInfo : serialPortInfos){
        serialPortParametersList.append(portInfo.portName());
        ui->cmbx_listSerialPorts->addItem(portInfo.portName());
        ui->txtbrw_logBrowser->append("Found Serial Posrt`s: " + portInfo.portName());
    }
}

void MainWindow::modbusDataReader(){
    setupModbusParameters();
    QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, 0, 9);

    if (!modbusMaster->connectDevice()){
        ui->txtbrw_logBrowser->append("Error connected! - Device not found or not connected.");
        modbusMaster->disconnectDevice();
        return;
    }
    if (auto *reply = modbusMaster->sendReadRequest(readUnit, slaveAddressBits)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, [=](){
                if (!reply)
                    return;
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit data = reply->result();
                    modbusRegisterAnswer->clear();
                    for (int i = 0; i < data.valueCount(); i++) {
                        modbusRegisterAnswer->append(data.value(i));
                    }
                    MainWindow::parseModbusResponse();
                }
                else if (reply->error() == QModbusDevice::ProtocolError) {
                    ui->txtbrw_logBrowser->append("Modbus protocol error:" + reply->errorString());
                }
                else {
                    ui->txtbrw_logBrowser->append("Modbus reply error:" + reply->errorString());
                }
                reply->deleteLater();
                modbusMaster->disconnectDevice();
            });
        }
        else{
            delete reply; //If response has already been completed, delete the response object.
        }
    }
    else {
        ui->txtbrw_logBrowser->append("Failed to send Modbus request: " + modbusMaster->errorString());
        modbusMaster->disconnectDevice();
    }
}

void MainWindow::modbusDataWriter(){
    setupModbusParameters();

    auto *tableOutputDataParse = ui->tableView;
    QAbstractItemModel *qaim = tableOutputDataParse->model();

    QModelIndex indexdataSlaveAddress = qaim->index(0, 1);
    QModelIndex indexTotalize1 = qaim->index(2, 1);
    QModelIndex indexTotalize2 = qaim->index(3, 1);
    QModelIndex indexTotalize3 = qaim->index(4, 1);
    QVariant dataSlaveAddress = qaim->data(indexdataSlaveAddress);
    QVariant dataTotalize1 = qaim->data(indexTotalize1);
    QVariant dataTotalize2 = qaim->data(indexTotalize2);
    QVariant dataTotalize3 = qaim->data(indexTotalize3);

    if(dataSlaveAddress.isNull() || dataTotalize1.isNull() || dataTotalize2.isNull() || dataTotalize3.isNull()){
        ui->txtbrw_logBrowser->append("Nothing to write. First select Serial ports, then 'Read device', after which you can write data to device!");
        return;
    }
    if(!dataSlaveAddress.isValid() || !dataTotalize1.isValid() || !dataTotalize2.isValid() || !dataTotalize3.isValid()){
        ui->txtbrw_logBrowser->append("Data valid?" + dataSlaveAddress.toString() + dataTotalize1.toString() + dataTotalize2.toString() + dataTotalize3.toString());
        return;
    }

    QModbusDataUnit writeUnit(QModbusDataUnit::HoldingRegisters, 0, 9);

    int slaveAddress = dataSlaveAddress.toInt();
    writeUnit.setValue(0, slaveAddress);

    int baudrateInteger = parametersListBaudrate.at(cmbxBaudrate->currentIndex());
    int baudratePart_1 = (baudrateInteger >> 16) & 0xFFFF;
    int baudratePart_2 = baudrateInteger & 0xFFFF;
    writeUnit.setValue(1, baudratePart_1);
    writeUnit.setValue(2, baudratePart_2);

    qlonglong totalize1 = dataTotalize1.toULongLong();
    int totalize1Part_1 = (totalize1 >> 16) & 0xFFFF;
    int totalize1Part_2 = totalize1 & 0xFFFF;
    writeUnit.setValue(3, totalize1Part_1);
    writeUnit.setValue(4, totalize1Part_2);

    qlonglong totalize2 = dataTotalize2.toULongLong();
    int totalize2Part_1 = (totalize2 >> 16) & 0xFFFF;
    int totalize2Part_2 = totalize2 & 0xFFFF;
    writeUnit.setValue(5, totalize2Part_1);
    writeUnit.setValue(6, totalize2Part_2);

    qlonglong totalize3 = dataTotalize3.toULongLong();
    int totalize3Part_1 = (totalize3 >> 16) & 0xFFFF;
    int totalize3Part_2 = totalize3 & 0xFFFF;
    writeUnit.setValue(7, totalize3Part_1);
    writeUnit.setValue(8, totalize3Part_2);

    if (!modbusMaster->connectDevice()){
        ui->txtbrw_logBrowser->append("Error connected! - Device not found or not connected.");
        modbusMaster->disconnectDevice();
        return;
    }
    if (auto *write = modbusMaster->sendWriteRequest(writeUnit, slaveAddressBits)) {
        if (!write->isFinished()) {
            connect(write, &QModbusReply::finished, [=](){
                if (!write)
                    return;
                if (write->error() == QModbusDevice::NoError) {
                    ui->txtbrw_logBrowser->append("Modbus success write! Time write: " + getCurrentTime());
                }
                else if (write->error() == QModbusDevice::ProtocolError) {
                    ui->txtbrw_logBrowser->append("Modbus protocol error:" + write->errorString());
                }
                else {
                    ui->txtbrw_logBrowser->append("Modbus reply error:" + write->errorString());
                }
                write->deleteLater();
                modbusMaster->disconnectDevice();
            });
        }
        else{
            delete write;
        }
    }
    else {
        ui->txtbrw_logBrowser->append("Failed to send Modbus request: " + modbusMaster->errorString());
        modbusMaster->disconnectDevice();
    }
}
