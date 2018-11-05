#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>
#include <QDebug>

class Enemy: public QObject,public QGraphicsPixmapItem{
    Q_OBJECT
public:
    //constructors
    Enemy(QGraphicsItem * parent=Q_NULLPTR);

    //attributes
    QTimer *getEnemyMoveTimer();

private:
    QTimer * enemyMoveTimer;

private slots:
    void move();

};

#endif // ENEMY_H
