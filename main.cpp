#include "mainwindow.h"
#include "UI/CustomSplashScreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    CustomSplashScreen splashScreen(QPixmap(":/img/spashScreenlogoOurCompany.png"));
    QFile styleFile( ":/qss/lightThemeApp.qss" );

    styleFile.open( QFile::ReadOnly );
    a.setStyleSheet( QString::fromLatin1( styleFile.readAll() ) );
    styleFile.close();
    /*
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "UBC_ModBus_Configurator-project_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }*/
    splashScreen.show();
    QTimer timer;
    timer.setInterval(2000);
    QObject::connect(&timer, &QTimer::timeout, [&](){
        splashScreen.close();
        w.showMaximized();
        timer.stop();
    });
    timer.start();
    return a.exec();
}
