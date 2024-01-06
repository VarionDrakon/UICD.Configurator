#include <QPixmap>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QResizeEvent>
#include <QDebug>
#include <QApplication>

class CustomGraphicsItem : public QGraphicsPixmapItem {
public:
    CustomGraphicsItem(const QPixmap &pixmap) : QGraphicsPixmapItem(pixmap){ }

    void resizePixmap(const QSize &newSize, const QSize &origSize){
        if (newSize.width() > 10 && newSize.height() > 10) {
            QPixmap scaledPixmap = pixmap().scaled(newSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);
            setPixmap(scaledPixmap);
            qDebug() << "X1" << "Size pixmap: " << newSize << " scaledPixmap rect: " << scaledPixmap.rect() << " pixmap rect: " << pixmap().rect();
        }
    }
};

class ScribbleArea : public QGraphicsView {
    Q_OBJECT

public:
    void setPixmap(const QPixmap &pixmap){
        pixmapItem->setPixmap(pixmap);
    }
    void setOriginalPixmapSize(const QSize &size){
        originalPixmapItem = size;
    }
    ScribbleArea(QGraphicsView* gpcImageOurCompany = nullptr) : QGraphicsView(gpcImageOurCompany) {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        scene = new QGraphicsScene(this);
        setScene(scene);

        pixmapItem = new CustomGraphicsItem(QPixmap());
        scene->addItem(pixmapItem);
    }

protected:
    void resizeEvent(QResizeEvent *event) override {
        QSize newSize = event->size();
        pixmapItem->resizePixmap(newSize, originalPixmapItem);

        fitInView(scene->itemsBoundingRect(), Qt::IgnoreAspectRatio);
        QGraphicsView::resizeEvent(event);
        qDebug() << "X3" << "Size image: " << newSize << " Scene rect: " << scene->sceneRect();
    }

private:
    QGraphicsScene* scene;
    CustomGraphicsItem* pixmapItem;
    QSize originalPixmapItem;
};
