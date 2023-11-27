#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSerialPortInfo>

void MainWindow::UpdateListCOMPorts(){
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    ui->cmbx_ListSerialPorts->clear();
    for(const QSerialPortInfo &portInfo : serialPortInfos){
        qDebug() << portInfo.portName();
        ui->cmbx_ListSerialPorts->addItem(portInfo.portName());
    }
}
