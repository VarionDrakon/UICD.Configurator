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
    for (const auto& baudrate : parametersListBaudrate)
    {
        ui->cmbx_listBaudrate->addItem(QString::number(baudrate));
    }
    for (const auto& dataBits : parametersListDataBits)
    {
        ui->cmbx_listDataBits->addItem(QString::number(dataBits));
    }
    for (const auto& stopBits : parametersListStopBits)
    {
        ui->cmbx_listStopBits->addItem(QString::number(stopBits));
    }
    for (const auto& parity : parametersListParity)
    {
        ui->cmbx_listParity->addItem(QString::number(parity));
    }

    ui->cmbx_listBaudrate->setCurrentIndex(3);
    ui->cmbx_listDataBits->setCurrentIndex(3);
    ui->cmbx_listStopBits->setCurrentIndex(0);
    ui->cmbx_listParity->setCurrentIndex(0);
}

void MainWindow::on_cmbx_listSerialPorts_currentIndexChanged(int index)
{
    if(!serialPortParametersList.empty() && index >= 0){
        nameSerialPort = serialPortParametersList[index];
        ui->txtbrw_logBrowser->append("Selected serial port: " + nameSerialPort);
    }
}

void MainWindow::on_cmbx_listBaudrate_currentIndexChanged(int index)
{
    //ui->txtbrw_logBrowser->append("Current baudrate: " + QString::number(index));
    baudrate = parametersListBaudrate[index];
}


void MainWindow::on_cmbx_listDataBits_currentIndexChanged(int index)
{

}

