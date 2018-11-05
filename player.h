#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>
#include <QMediaPlayer>

class Player:public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Player(QGraphicsItem * parent=Q_NULLPTR);
private:
    void keyPressEvent(QKeyEvent * event);
    QMediaPlayer * bulletsound;
private slots:
    void spawn();
};

#endif // PLAYER_H
