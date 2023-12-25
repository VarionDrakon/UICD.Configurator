#include "mainwindow.h"
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
    QModelIndex indexSlaveCell = qaim->index(0, 1);
    QVariant dataSlaveCell = qaim->data(indexSlaveCell);
    int slaveAddress = dataSlaveCell.toInt();

    int baudrateInteger = parametersListBaudrate.at(cmbxBaudrate->currentIndex());

    //ui->txtbrw_logBrowser->append(QString::number(Baudrate));

    QModbusDataUnit writeUnit(QModbusDataUnit::HoldingRegisters, 0, 3);

    int baudratePart_1 = (baudrateInteger >> 16) & 0xFFFF;
    int baudratePart_2 = baudrateInteger & 0xFFFF;

    writeUnit.setValue(0, slaveAddress);
    writeUnit.setValue(1, baudratePart_1);
    writeUnit.setValue(2, baudratePart_2);

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
