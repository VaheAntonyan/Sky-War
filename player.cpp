#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include <QGraphicsScene>
#include <QKeyEvent>

Player::Player(QGraphicsItem *parent): QGraphicsPixmapItem(parent){
    // set bullet sound
    bulletsound = new QMediaPlayer();
    bulletsound->setMedia(QUrl("qrc:/sounds/gun_shot.wav"));
    bulletsound->setVolume(50);

    // set graphic
    setPixmap(QPixmap(":/images/player.png"));
}

void Player::keyPressEvent(QKeyEvent *event){
    // move the player left and right
    switch (event->key()) {
    case Qt::Key_Left: {
        if (pos().x() > 0)
            setPos(x()-10,y());
        break;
    }
    case Qt::Key_Right: {
        if (pos().x() + 100 < 800)
            setPos(x()+10,y());
        break;
    }
    case Qt::Key_Space: {
        // create a bullet
        Bullet * bullet = new Bullet();
        bullet->setPos(x()+35,y()-30);
        scene()->addItem(bullet);

        // play bulletsound
        if(bulletsound->state() == QMediaPlayer::PlayingState){
            bulletsound->setPosition(0);
        }
        else if (bulletsound->state() == QMediaPlayer::StoppedState){
            bulletsound->play();
        }
        break;
    }
    default:
        break;
    }
}

void Player::spawn(){
    // create an enemy
    Enemy * enemy = new Enemy();
    scene()->addItem(enemy);
}
