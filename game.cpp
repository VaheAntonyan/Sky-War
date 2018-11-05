#include "Game.h"
#include "Enemy.h"
#include "Button.h"
#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <QBrush>
#include <QImage>
#include <cstdlib>
#include <ctime>
#include <typeinfo>

Game::Game(QWidget *parent){
    // create the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,800,600);
    setBackgroundBrush(QBrush(QImage(":/images/bg.jpg").scaled(800,600)));
    setScene(scene);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(802,602);

    // initial game states
    isStarted = false;
    isPaused = false;
}

void Game::displayMainMenu(){
    // create the title text
    QGraphicsTextItem* titleText = new QGraphicsTextItem(QString("Sky War"));
    QFont titleFont("comic sans",50);
    titleText->setFont(titleFont);
    int txPos = this->width()/2 - titleText->boundingRect().width()/2;
    int tyPos = 150;
    titleText->setPos(txPos,tyPos);
    scene->addItem(titleText);

    // create the play button
    Button* playButton = new Button(QString("Play"));
    int bxPos = this->width()/2 - playButton->boundingRect().width()/2;
    int byPos = 275;
    playButton->setPos(bxPos,byPos);
    connect(playButton,SIGNAL(clicked()),this,SLOT(start()));
    scene->addItem(playButton);

    // create the quit button
    Button* quitButton = new Button(QString("Quit"));
    int qxPos = this->width()/2 - quitButton->boundingRect().width()/2;
    int qyPos = 350;
    quitButton->setPos(qxPos,qyPos);
    connect(quitButton,SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(quitButton);
}

void Game::start(){

    isStarted = true;
    srand(time(0));
    scene->clear();

    // create the player
    player = new Player();
    player->setPos(350,500);
    // make the player focusable and set it to be the current focus
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    scene->addItem(player);

    // create the score/health
    score = new Score();
    scene->addItem(score);
    health = new Health();
    health->setPos(health->x(),health->y()+25);
    connect(health,SIGNAL(gameOver()),this,SLOT(displayGameOverWindow()));
    scene->addItem(health);

    // spawn enemies
    enemySpawnTimer = new QTimer();
    connect(enemySpawnTimer,SIGNAL(timeout()),player,SLOT(spawn()));
    enemySpawnTimer->start(2000);

    // create playlist
    bgMediaPlaylist = new QMediaPlaylist();
    bgMediaPlaylist->addMedia(QUrl("qrc:/sounds/bg_sound.mp3"));
    bgMediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);

    // set playlist for mediaplayer
    bgMediaPlayer = new QMediaPlayer();
    bgMediaPlayer->setPlaylist(bgMediaPlaylist);
    bgMediaPlayer->play();

    // create pauseKey
    pauseKey = new QShortcut(this);
    pauseKey->setKey(Qt::Key_P);
    connect(pauseKey, &QShortcut::activated, this, &Game::displayPauseWindow);

    show();
}

void Game::restartGame(){

    //some clean-up
    isPaused=!isPaused;
    scene_items.clear();
    scene_enemy_items.clear();
    scene_bullet_items.clear();
    bgMediaPlaylist->clear();
    delete pauseKey;

    start();
}

void Game::displayGameOverWindow(){

    pause();
    disconnect(pauseKey, &QShortcut::activated, this, &Game::displayPauseWindow);

    // pop up semi transparent panel
    drawPanel(0,0,800,600,Qt::black,0.45);

    // draw panel
    drawPanel(200,100,400,400,Qt::lightGray,0.75);

    // create playAgain button
    Button* playAgain = new Button(QString("Play Again"));
    int pAxPos = this->width()/2 - playAgain->boundingRect().width()/2;
    int pAyPos = 275;
    playAgain->setPos(pAxPos,pAyPos);
    scene->addItem(playAgain);
    connect(playAgain,SIGNAL(clicked()),this,SLOT(restartGame()));

    // create quit button
    Button* quit = new Button(QString("Quit"));
    int qxPos = this->width()/2 - quit->boundingRect().width()/2;
    int qyPos = 350;
    quit->setPos(qxPos,qyPos);
    scene->addItem(quit);
    connect(quit,SIGNAL(clicked()),this,SLOT(close()));

    // create text annoucning score
    QGraphicsTextItem* overText = new QGraphicsTextItem(QString("Your score is: ") + QString::number(score->getScore()));
    QFont textFont("comic sans",20);
    overText->setFont(textFont);
    int txPos = this->width()/2 - overText->boundingRect().width()/2;
    overText->setPos(txPos,155);
    scene->addItem(overText);
}

void Game::displayPauseWindow(){
    if (!isPaused) {
        // draw panel
        drawPanel(200,100,400,150,Qt::lightGray,0.2);

        // create text GAME IS PAUSED
        pauseText = new QGraphicsTextItem(QString("GAME IS PAUSED"));
        QFont textFont("comic sans",30);
        pauseText->setFont(textFont);

        int txPos = this->width()/2 - pauseText->boundingRect().width()/2;
        pauseText->setPos(txPos,145);
        scene->addItem(pauseText);
    }
    else{
        scene->removeItem(panel);
        scene->removeItem(pauseText);
        delete panel;
        delete pauseText;
    }

    pause();
}

void Game::pause(){

    if (!isStarted){
        return;
    }

    isPaused = !isPaused;

    scene_items = scene->items();
    if (isPaused) {
        // disable all scene items
        for (size_t i = 0, n = scene_items.size(); i < n; i++){
            scene_items[i]->setEnabled(false);
        }

        // stop background music
        bgMediaPlayer->pause();

        // stop spawning enemies
        enemySpawnTimer->stop();

        //stop enemies and bullets
        for (int i = 0, n = scene_items.size(); i < n; ++i){
            if (typeid(*(scene_items[i])) == typeid(Enemy)){
                scene_enemy_items.append(dynamic_cast<Enemy *>(scene_items[i]));
            }
            if (typeid(*(scene_items[i])) == typeid(Bullet)){
                scene_bullet_items.append(dynamic_cast<Bullet *>(scene_items[i]));
            }
        }
        for(int i = 0, n = scene_enemy_items.size(); i < n; ++i){
            //stop enemies' timers
            scene_enemy_items[i]->getEnemyMoveTimer()->stop();
        }
        for(int i = 0, n = scene_bullet_items.size(); i < n; ++i){
            //stop bullets' timers
            scene_bullet_items[i]->getBulletMoveTimer()->stop();
        }
    }
    else{
        // enable all scene items
        for (size_t i = 0, n = scene_items.size(); i < n; i++){
            scene_items[i]->setEnabled(true);
        }

        // play background music
        bgMediaPlayer->play();

        // start spawning enemies
        enemySpawnTimer->start(2000);

        for(int i = 0, n = scene_enemy_items.size(); i < n; i++){
            //start enemies' timers
            scene_enemy_items[i]->getEnemyMoveTimer()->start(20);
        }
        for(int i = 0, n = scene_bullet_items.size(); i < n; i++){
            //start bullets' timers
            scene_bullet_items[i]->getBulletMoveTimer()->start(20);
        }
        player->setFocus();

        scene_items.clear();
        scene_enemy_items.clear();
        scene_bullet_items.clear();
    }
}


void Game::drawPanel(int x, int y, int width, int height, QColor color, double opacity){
    // draws a panel at the specified location with the specified properties
    panel = new QGraphicsRectItem(x,y,width,height);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(color);
    panel->setBrush(brush);
    panel->setOpacity(opacity);
    scene->addItem(panel);
}
