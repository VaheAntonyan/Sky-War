#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>

class Bullet: public QObject,public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Bullet(QGraphicsItem * parent=Q_NULLPTR);
    QTimer *getBulletMoveTimer();
private:
    QTimer * bulletMoveTimer;
private slots:
    void move();
};

#endif // BULLET_H
