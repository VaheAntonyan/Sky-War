#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include <QShortcut>
#include "Player.h"
#include "Score.h"
#include "Health.h"
#include "enemy.h"
#include "bullet.h"

class Game: public QGraphicsView{
    Q_OBJECT
public:
    // constructors
    Game(QWidget * parent=Q_NULLPTR);

    // public methods
    void displayMainMenu();

    // public attributes
    QGraphicsScene * scene;
    Player * player;
    Score * score;
    Health * health;

public slots:
    void start();
    void restartGame();
    void displayGameOverWindow();
    void displayPauseWindow();

private:
    // private methods
    void drawPanel(int x, int y, int width, int height, QColor color, double opacity);
    void pause();

    // private attributes
    QGraphicsRectItem * panel;
    QGraphicsTextItem * pauseText;
    bool isStarted;
    bool isPaused;
    QShortcut *pauseKey;
    QMediaPlaylist * bgMediaPlaylist;
    QMediaPlayer * bgMediaPlayer;
    QTimer * enemySpawnTimer;
    QList<QGraphicsItem *> scene_items;
    QList<Enemy *> scene_enemy_items;
    QList<Bullet *> scene_bullet_items;
};

#endif // GAME_H
