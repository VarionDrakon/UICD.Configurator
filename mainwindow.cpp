#include "mainwindow.h"
#include "ui_mainwindow.h"
//Libs included
#include <QDebug>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include <QList>

QList<QString> listCOMPorts;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    UpdateListCOMPorts();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateListCOMPorts(){
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    listCOMPorts.clear();
    for(const QSerialPortInfo &portInfo : serialPortInfos){
        qDebug() << portInfo.portName();
        ui->cmbx_ListSerialPorts->clear();
        listCOMPorts.append(portInfo.portName());
    }
    ui->cmbx_ListSerialPorts->addItems(listCOMPorts);
}

void MainWindow::on_pushButton_clicked()
{
    UpdateListCOMPorts();
}

