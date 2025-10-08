#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "../RealWorldInterface/TimeUtils.h"

void MainWindow::setupModbusParameters(){
    modbusMaster->setConnectionParameter(QModbusDevice::SerialPortNameParameter, nameSerialPort);
    modbusMaster->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, baudrate);
    modbusMaster->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, dataBits);
    modbusMaster->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, stopBits);
    modbusMaster->setConnectionParameter(QModbusDevice::SerialParityParameter, parityBits);
    modbusMaster->setNumberOfRetries(3);
    modbusMaster->setTimeout(100);
}

void MainWindow::parseModbusResponse(){
    modbusParseAnswer->clear();
    modbusParseAnswer->resize(5);
    modbusParseAnswer->fill(0);

    modbusParseAnswer->replace(0, modbusRegisterAnswer->at(0));
    ui->txtbrw_logBrowser->append(nameSerialPort);

    unsigned int baudrate = modbusRegisterAnswer->at(1);
    baudrate = (baudrate << 16) |  modbusRegisterAnswer->at(2);
    modbusParseAnswer->replace(1, baudrate);
    //ui->txtbrw_logBrowser->append(QString::number(parseModbusAnswer->at(1)));

    unsigned int totalizeOne = modbusRegisterAnswer->at(3);
    totalizeOne = (totalizeOne << 16) |  modbusRegisterAnswer->at(4);
    modbusParseAnswer->replace(2, totalizeOne);
    //ui->txtbrw_logBrowser->append(QString::number(modbusRegisterAnswer->at(2)));

    unsigned int totalizeTwo = modbusRegisterAnswer->at(5);
    totalizeTwo = (totalizeTwo << 16) |  modbusRegisterAnswer->at(6);
    modbusParseAnswer->replace(3, totalizeTwo);
    //ui->txtbrw_logBrowser->append(QString::number(modbusRegisterAnswer->at(3)));

    unsigned int totalizeThree = modbusRegisterAnswer->at(7);
    totalizeThree = (totalizeThree << 16) |  modbusRegisterAnswer->at(8);
    modbusParseAnswer->replace(4, totalizeThree);
    //ui->txtbrw_logBrowser->append(QString::number(modbusRegisterAnswer->at(4)));
    responseModbusDevice();
}

void MainWindow::responseModbusDevice(){

    auto *tableOutputDataParse = ui->tableView;
    QAbstractItemModel *qaim = tableOutputDataParse->model();
    cmbxBaudrate = new QComboBox;
    emit qaim->layoutAboutToBeChanged();

    for (int row = 0; row < qaim->rowCount(); row++) {
        QModelIndex index = qaim->index(row, 1);
        qaim->setData(index, modbusParseAnswer->at(row), Qt::EditRole);
        //ui->txtbrw_logBrowser->append("Success!");
    }

    emit qaim->layoutChanged();
    tableOutputDataParse->reset();

    for (const auto& baudrate : parametersListBaudrate)
    {
        cmbxBaudrate->addItem(QString::number(baudrate));
    }

    for (int i = 0; i < parametersListBaudrate.count(); i++){
        if(parametersListBaudrate.at(i) == modbusParseAnswer->at(1)){
            indexValue = i;
            break;
        }
        else {
            indexValue = -1;
        }
    }
    cmbxBaudrate->setCurrentIndex(indexValue);
    tableOutputDataParse->setIndexWidget(tableOutputDataParse->model()->index(1, 1), cmbxBaudrate);
    ui->txtbrw_logBrowser->append("Modbus success read! Time read: " + getCurrentTime());
}
