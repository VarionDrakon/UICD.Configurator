#ifndef CUSTOMSPLASHSCREEN_H
#define CUSTOMSPLASHSCREEN_H

#include <QSplashScreen>
#include <QEvent>

class CustomSplashScreen : public QSplashScreen {
public:
    CustomSplashScreen(const QPixmap &pixmap) : QSplashScreen(pixmap, Qt::WindowStaysOnTopHint) { }

protected:
    bool event (QEvent *event) override {
        if((event->type() == QEvent::MouseButtonPress) ||
            (event->type() == QEvent::MouseButtonDblClick) ||
            (event->type() == QEvent::MouseButtonRelease) ||
            (event->type() == QEvent::KeyPress) ||
            (event->type() == QEvent::KeyRelease)){
            return true;
        }

        return QSplashScreen::event(event);
    }
};

#endif // CUSTOMSPLASHSCREEN_H
