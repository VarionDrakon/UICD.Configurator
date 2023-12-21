#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QModbusDataUnit>
#include <QModbusDevice>
#include <QModbusRtuSerialMaster>
#include <QSerialPort>
#include <QStandardItemModel>
#include <QTranslator>
#include <QList>
#include <QStyledItemDelegate>
#include <QAbstractItemModel>
//Create object Modbus RTU masters
QModbusRtuSerialClient *modbusMaster = new QModbusRtuSerialClient();
//Create object Modbus RTU Answer
QList<int> *modbusRegisterAnswer = new QList<int>;

void MainWindow::UpdateListCOMPorts(){
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    ui->cmbx_listSerialPorts->clear();

    for(const QSerialPortInfo &portInfo : serialPortInfos){
        serialPortParametersList.append(portInfo.portName());
        ui->cmbx_listSerialPorts->addItem(portInfo.portName());
        ui->txtbrw_logBrowser->append("Found Serial Posrt`s: " + portInfo.portName());
    }
}

void MainWindow::ConnectedModbusDevice(){
    modbusMaster->setConnectionParameter(QModbusDevice::SerialPortNameParameter, nameSerialPort);
    modbusMaster->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, baudrate);
    modbusMaster->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, dataBits);
    modbusMaster->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, stopBits);
    modbusMaster->setConnectionParameter(QModbusDevice::SerialParityParameter, parityBits);
    modbusMaster->setTimeout(100);
    modbusMaster->setNumberOfRetries(3);
    int deviceAddress = 10;
    int startRegisterAddress = 0;
    int countReadRegister = 9;

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
                        modbusRegisterAnswer->append(data.value(i));
                    }
                }
                else if (reply->error() == QModbusDevice::ProtocolError) {
                    ui->txtbrw_logBrowser->append("Modbus protocol error:" + reply->errorString());
                }
                else {
                    ui->txtbrw_logBrowser->append("Modbus reply error:" + reply->errorString());
                }
                reply->deleteLater();
                modbusMaster->disconnectDevice();
                ResponseModbusDevice();
                //delete modbusRegisterAnswer;
            });
        }
        else{
            delete reply;
            delete modbusRegisterAnswer;
        }
    }
    else {
        ui->txtbrw_logBrowser->append("Failed to send Modbus request: " + modbusMaster->errorString());
        modbusMaster->disconnectDevice();
    }
}

void MainWindow::ResponseModbusDevice(){
    QAbstractItemModel *qaim = ui->tableView->model();
    int rows = qaim->rowCount();
    emit qaim->layoutAboutToBeChanged();
    for (int row = 0; row < rows; row++) {
        QModelIndex index = qaim->index(row, 1);
        qaim->setData(index, modbusRegisterAnswer->at(row), Qt::EditRole);
        ui->txtbrw_logBrowser->append("Success!");
    }
    emit qaim->layoutChanged();
    ui->tableView->reset();
}

class ReadOnlyDelegate: public QStyledItemDelegate{
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    QWidget *createEditor(QWidget *, const QStyleOptionViewItem &, const QModelIndex &) const override {
        return nullptr;
    }
};

void MainWindow::ParseModBusAnswer(){
    /*
    QStandardItemModel *model = new QStandardItemModel(0, 1, this); //create model with 0 rows, 1 column and use this class
    QTreeView *treeView = ui->treeView; //selected UI-object
    QStandardItem *rootItem = model->invisibleRootItem(); //retrun invisible root element (I don`t know nahuya)
    treeView->setRootIsDecorated(true); //show "arrow" for elements
    treeView->setEditTriggers(QAbstractItemView::NoEditTriggers); //ban on editable elements (Nehui rename something)
    treeView->setModel(model); //indicate use this model

    model->setHeaderData(0, Qt::Horizontal, "Parameters"); //Header text

    QStandardItem *item1 = new QStandardItem(QObject::tr("File"));
    rootItem->appendRow(item1);
    QStandardItem *childItem = new QStandardItem("Children elements 1");
    item1->appendRow(childItem);*/

    QStandardItemModel* model = new QStandardItemModel(0, 2, this);
    QTableView* tableView = ui->tableView;
    tableView->setModel(model);

    // Добавление элементов в модель
    QStandardItem* item1 = new QStandardItem("Modbus slave address");
    QStandardItem* item2 = new QStandardItem("Baudrate");
    QStandardItem* item3 = new QStandardItem("Totalize 1 (GENERAL)");
    QStandardItem* item4 = new QStandardItem("Totalize 2 (FORWARD)");
    QStandardItem* item5 = new QStandardItem("Totalize 3 (REVERSE)");
    model->appendRow(item1);
    model->appendRow(item2);
    model->appendRow(item3);
    model->appendRow(item4);
    model->appendRow(item5);
    // Получение доступа к элементу для изменения данных
    QStandardItem* itemToUpdate = model->item(0); // получаем элемент с индексом 0
    if (itemToUpdate) {
        ui->txtbrw_logBrowser->append("Updated Item");
    }
    tableView->setItemDelegateForColumn(0, new ReadOnlyDelegate);
    // Обновление данных в представлении
    model->itemChanged(itemToUpdate); // уведомляем модель о изменении элемента
    tableView->update(); // обновляем представление
}
