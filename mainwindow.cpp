#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SerialPort/SerialPortListener.h"
//Libs included
#include <QSerialPort>
#include <QList>
#include <QString>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->txtbrw_logBrowser->setReadOnly(true);

    LocalParametersInitilizatedOnStartup();
    UpdateListCOMPorts();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_scaningExistSerialPorts_clicked()
{
    UpdateListCOMPorts();
}


void MainWindow::on_btn_readChooserDevice_clicked()
{
    ConnectedModbusDevice();
}

void MainWindow::LocalParametersInitilizatedOnStartup()
{
    for (const auto& baudrate : baudrateParameters)
    {
        ui->cmbx_listBaudrate->addItem(QString::number(baudrate));
    }
}

void MainWindow::on_cmbx_listSerialPorts_currentIndexChanged(int index)
{
    ui->txtbrw_logBrowser->append("Current serial ports: " + QString::number(index));
    //nameSerialPort = nameSerialPort[index];
    //qDebug() << nameSerialPort[index];
}


void MainWindow::on_cmbx_listBaudrate_currentIndexChanged(int index)
{
    ui->txtbrw_logBrowser->append("Current baudrate: " + QString::number(index));
    baudrate = baudrateParameters[index];
}

