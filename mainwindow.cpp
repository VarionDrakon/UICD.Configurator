#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SerialPort/SerialPortListener.h"
//Libs included

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ConnectedModbusDevice();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_scaningExistSerialPorts_clicked()
{
    //UpdateListCOMPorts();
    ConnectedModbusDevice();
}

