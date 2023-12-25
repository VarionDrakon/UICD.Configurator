#include <QString>
#include <QDateTime>

QString getCurrentTime(){
    QDateTime currentTime = QDateTime::currentDateTime();
    QString formattedTime = currentTime.toString("yyyy-MM-dd hh:mm:ss");
    return formattedTime;
}
