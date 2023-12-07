#include "mainwindow.h"
//Includes files from lib`s
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile styleFile( ":qss/stylesheet_mainwindows.qss" );
    styleFile.open( QFile::ReadOnly );
    a.setStyleSheet( QString::fromLatin1( styleFile.readAll() ) );
    styleFile.close();
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "UBC_ModBus_Configurator-project_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}
