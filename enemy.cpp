#include "Enemy.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include "Game.h"

extern Game * game;

Enemy::Enemy(QGraphicsItem *parent): QObject(), QGraphicsPixmapItem(parent){
    //set random x position
    int random_number = rand() % 700;
    setPos(random_number,0);
    setZValue(-1);

    // drew enemy
    setPixmap(QPixmap(":/images/enemy.png"));

    // connect
    enemyMoveTimer = new QTimer(this);
    connect(enemyMoveTimer,SIGNAL(timeout()),this,SLOT(move()));

    // start the timer
    enemyMoveTimer->start(20);
}

QTimer *Enemy::getEnemyMoveTimer(){
    return enemyMoveTimer;
}

void Enemy::move(){
    // move enemy down
    setPos(x(),y()+5);

    // destroy enemy when it goes out of the screen
    if (pos().y() > 600){
        //decrease the health
        game->health->decrease();

        scene()->removeItem(this);
        delete this;
    }
}
