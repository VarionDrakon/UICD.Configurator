#ifndef THEMEAPP_H
#define THEMEAPP_H

#include "../mainwindow.h"
#include <ui_mainwindow.h>

void MainWindow::changeThemeApp() {
    currentThemeApp++;
    if(currentThemeApp >= listThemeApp.size()){
        currentThemeApp = 0;
    }

    QList<QWidget*> widgets = qApp->allWidgets();
    QFile file(listThemeApp.at(currentThemeApp));
    QGraphicsOpacityEffect* opacityEffect = nullptr;

    file.open(QFile::ReadOnly | QFile::Text);
    QString styleSheet = file.readAll();
    file.close();

    foreach (QWidget *widget, widgets) {
        opacityEffect = new QGraphicsOpacityEffect(widget);
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
        connect(animationAppearance, &QPropertyAnimation::finished, [=](){
            opacityEffect->deleteLater();
            animationAppearance->deleteLater();
            animationDisappearance->deleteLater();
        });

        animationDisappearance->start();
    }
}

void MainWindow::resourceThemeApp() {
    pixmapOurLogoCompany = new QPixmap(":/img/logoOurCompany.png");
    ui->lblImageOurCompany->setFixedSize(QSize(432, 110));
    ui->lblImageOurCompany->setPixmap(pixmapOurLogoCompany->scaled(ui->lblImageOurCompany->size(), Qt::KeepAspectRatio));
    ui->lblImageOurCompany->setScaledContents(true);

    QIcon iconResChangeTheme(":/img/changeThemeIcon.png");

    ui->btnChangeTheme->setIcon(iconResChangeTheme);
    ui->btnChangeTheme->setFixedSize(120, 120);
    ui->btnChangeTheme->setIconSize(ui->btnChangeTheme->size() - QSize(10, 10));
    ui->btnChangeTheme->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

#endif // THEMEAPP_H
