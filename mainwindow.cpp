#include "mainwindow.h"
#include "ui_mainwindow.h"
//Libs included
#include <QDebug>
#include <QtSerialPort/QSerialPortInfo>

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
    for(const QSerialPortInfo &portInfo : serialPortInfos){
        qDebug() << portInfo.portName();
    }  
}
