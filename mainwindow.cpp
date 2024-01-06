#include "ui_mainwindow.h"
#include "SerialPort/ModbusCommunication.h"
#include "UI/TableDataFiller.h"
//Libs included

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->txtbrw_logBrowser->setReadOnly(true);

    localParametersInitilizatedOnStartup();
    updateListCOMPorts();
    tableDataHandler();

    pixmapOurLogoCompany = new QPixmap(":/qss/logoOurCompany.png");

    ui->lblImageOurCompany->setFixedSize(QSize(400, 150));

    ui->lblImageOurCompany->setPixmap(pixmapOurLogoCompany->scaled(ui->lblImageOurCompany->size(), Qt::KeepAspectRatio));

    ui->lblImageOurCompany->setScaledContents(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_scaningExistSerialPorts_clicked()
{
    updateListCOMPorts();
}

void MainWindow::on_btn_readChooserDevice_clicked()
{
    modbusDataReader();
}

void MainWindow::on_btn_writeChooserDevice_clicked()
{
    modbusDataWriter();
}

void MainWindow::localParametersInitilizatedOnStartup()
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

void MainWindow::on_btnChangeTheme_clicked()
{
    currentThemeApp++;
    if(currentThemeApp >= listThemeApp.size()){
        currentThemeApp = 0;
    }

    QList<QWidget *> widgets = qApp->allWidgets();
    QFile file(listThemeApp.at(currentThemeApp));

    file.open(QFile::ReadOnly | QFile::Text);
    QString styleSheet = file.readAll();
    file.close();

    foreach (QWidget *widget, widgets) {
        QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect(widget);
        widget->setGraphicsEffect(opacityEffect);

        QPropertyAnimation* animationDisappearance = new QPropertyAnimation(opacityEffect, "opacity");
        animationDisappearance->setDuration(1000);
        animationDisappearance->setStartValue(1.0);
        animationDisappearance->setEndValue(0.0);

        QPropertyAnimation* animationAppearance = new QPropertyAnimation(opacityEffect, "opacity");
        animationAppearance->setDuration(1000);
        animationAppearance->setStartValue(0.0);
        animationAppearance->setEndValue(1.0);

        connect(animationDisappearance, &QPropertyAnimation::finished, [=](){
            qApp->setStyleSheet(styleSheet);
            animationAppearance->start();
        });
        animationDisappearance->start();
    }
}

