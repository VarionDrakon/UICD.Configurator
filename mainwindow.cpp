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
    ParseModbusAnswer();
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

void MainWindow::on_btn_writeChooserDevice_clicked()
{
    WriteModbusDevice();
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
    for (const auto& parity : parametersListParityBits)
    {
        ui->cmbx_listParity->addItem(QString::number(parity));
    }

    MainWindow::on_spnbx_listSlaveID_valueChanged(10);
    MainWindow::on_cmbx_listBaudrate_currentIndexChanged(3);
    MainWindow::on_cmbx_listDataBits_currentIndexChanged(3);
    MainWindow::on_cmbx_listStopBits_currentIndexChanged(0);
    MainWindow::on_cmbx_listParity_currentIndexChanged(0);
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
    ui->cmbx_listBaudrate->setCurrentIndex(index);
    baudrate = parametersListBaudrate[index];
    //ui->txtbrw_logBrowser->append("Current baudrate: " + QString::number(index));
}

void MainWindow::on_cmbx_listDataBits_currentIndexChanged(int index)
{
    ui->cmbx_listDataBits->setCurrentIndex(index);
    dataBits = parametersListDataBits[index];
    //ui->txtbrw_logBrowser->append("Current dataBits: " + QString::number(index));
}

void MainWindow::on_cmbx_listStopBits_currentIndexChanged(int index)
{
    ui->cmbx_listStopBits->setCurrentIndex(index);
    stopBits = parametersListStopBits[index];
    //ui->txtbrw_logBrowser->append("Current stopBits: " + QString::number(index));
}

void MainWindow::on_cmbx_listParity_currentIndexChanged(int index)
{
    ui->cmbx_listParity->setCurrentIndex(index);
    parityBits = parametersListParityBits[index];
    //ui->txtbrw_logBrowser->append("Current parityBits: " + QString::number(index));
}

void MainWindow::on_spnbx_listSlaveID_valueChanged(int arg1)
{
    slaveAddressBits = arg1;
    //ui->txtbrw_logBrowser->append("Current slave address: " + QString::number(arg1));
}
