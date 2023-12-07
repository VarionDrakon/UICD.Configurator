#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SerialPort/SerialPortListener.h"
//Libs included
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QFile styleFile( ":/Resources/stylesheet_mainwindows.qss" );
    styleFile.open( QFile::ReadOnly );
    parent->setStyleSheet( QString::fromLatin1( styleFile.readAll() ) );
    styleFile.close();

    ui->setupUi(this);
    ui->txtbrw_logBrowser->setReadOnly(true);
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

