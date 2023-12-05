#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void UpdateListCOMPorts();

private slots:
    void ResponseModbusDevice();
    void ConnectedModbusDevice();
    void on_btn_scaningExistSerialPorts_clicked();

    void on_btn_readChooserDevice_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
