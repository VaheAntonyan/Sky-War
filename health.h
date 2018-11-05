#ifndef HEALTH_H
#define HEALTH_H

#include <QGraphicsTextItem>

class Health: public QGraphicsTextItem{
    Q_OBJECT
public:
    Health(QGraphicsItem * parent=Q_NULLPTR);
    void decrease();
    int getHealth();

signals:
    void gameOver();

private:
    int health;
};

#endif // HEALTH_H
