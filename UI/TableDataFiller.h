#include "../mainwindow.h"
#include "ui_mainwindow.h"

class ReadOnlyDelegate: public QStyledItemDelegate{
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    QWidget *createEditor(QWidget *, const QStyleOptionViewItem &, const QModelIndex &) const override {
        return nullptr;
    }
};

void MainWindow::tableDataHandler(){

    QStandardItemModel *modelTree = new QStandardItemModel(0, 1, this); //create model with 0 rows, 1 column and use this class
    QTreeView *treeView = ui->treeView; //selected UI-object
    QStandardItem *rootItem = modelTree->invisibleRootItem(); //retrun invisible root element (I don`t know nahuya)
    treeView->setRootIsDecorated(true); //show "arrow" for elements
    treeView->setEditTriggers(QAbstractItemView::NoEditTriggers); //ban on editable elements (Nehui rename something)
    treeView->setModel(modelTree); //indicate use this model

    modelTree->setHeaderData(0, Qt::Horizontal, "Parameters"); //Header text

    QStandardItem *itemTree1 = new QStandardItem(QObject::tr("File"));
    rootItem->appendRow(itemTree1);
    QStandardItem *childItem = new QStandardItem("Children elements 1");
    itemTree1->appendRow(childItem);

    QStandardItemModel* model = new QStandardItemModel(0, 2, this);
    QTableView* tableView = ui->tableView;
    tableView->setModel(model);

    //add item`s to model
    QStandardItem* item1 = new QStandardItem("Modbus slave address");
    QStandardItem* item2 = new QStandardItem("Baudrate");
    QStandardItem* item3 = new QStandardItem("Totalize 1 (GENERAL)");
    QStandardItem* item4 = new QStandardItem("Totalize 2 (FORWARD)");
    QStandardItem* item5 = new QStandardItem("Totalize 3 (REVERSE)");
    model->appendRow(item1);
    model->appendRow(item2);
    model->appendRow(item3);
    model->appendRow(item4);
    model->appendRow(item5);

    QStandardItem* itemToUpdate = model->item(0);
    tableView->setItemDelegateForColumn(0, new ReadOnlyDelegate);

    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    model->itemChanged(itemToUpdate);
    tableView->update();
}
