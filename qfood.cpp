#include "qsnake.h"

QFood::QFood(QSnakeGame* owner, int row, int col)
    : QCell(owner, row, col)
{
    this->apple.load(":/Images/apple.png");
    this->move();
}

QPoint QFood::move()
{
    QPoint newPoint = owner->getRandFreeCell();

    this->row = newPoint.y();
    this->col = newPoint.x();

    return newPoint;
}

void QFood::draw()
{
    QCell::draw(apple);
}

bool QFood::checkCollision(QPoint cell)
{
    if (QPoint(this->col, this->row) == cell)
        return true;
    else
        return false;
}
