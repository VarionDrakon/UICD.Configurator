#include "mainwindow.h"
#include "ui_mainwindow.h"
//Libs included
#include <QDebug>
#include <QSerialPortInfo>
#include <QString>
#include <QList>

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
    ui->cmbx_ListSerialPorts->clear();
    for(const QSerialPortInfo &portInfo : serialPortInfos){
        qDebug() << portInfo.portName();
        ui->cmbx_ListSerialPorts->addItem(portInfo.portName());
    }
}

void MainWindow::on_pushButton_clicked()
{
    UpdateListCOMPorts();
}

