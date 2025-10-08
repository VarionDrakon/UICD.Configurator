QT += core gui serialport serialbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    RealWorldInterface/TimeUtils.h \
    SerialPort/ModbusCommunication.h \
    SerialPort/ModbusHandler.h \
    UI/CustomSplashScreen.h \
    UI/TableDataFiller.h \
    UI/ThemeApp.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    UBC_ModBus_Configurator-project_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources/resources.qrc

DISTFILES += \
    Resources/changeThemeIcon.png \
    Resources/darkThemeApp.qss \
    Resources/lightThemeApp.qss \
    Resources/logoOurCompany.png \
    Resources/spashScreenlogoOurCompany.png
