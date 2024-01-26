#include "qsnake.h"

QSnake::QSnake(QSnakeGame* owner, int size = 6)
{
    this->owner = owner;
    /*
        // To do: random snake creation based on field's size
        this->snake.push_back(QCell(owner, 2, 2));
        this->snake.push_back(QCell(owner, 2, 3));
        this->snake.push_back(QCell(owner, 2, 4));
        this->snake.push_back(QCell(owner, 2, 5));
        this->snake.push_back(QCell(owner, 3, 5));
        this->snake.push_back(QCell(owner, 3, 4));
        this->snake.push_back(QCell(owner, 4, 4));
        this->snake.push_back(QCell(owner, 4, 5));
        this->snake.push_back(QCell(owner, 5, 5));
    */
    QPoint fstCell = owner->getRandFreeCell();
    QPoint newCoord;
    QSize fieldSize = owner->getFieldSize();
    int rnd, x, y;

    this->snake.push_back(QCell(owner, fstCell.y(), fstCell.x()));
    while (snake.size() < size) {
        x = snake.back().getCoord().x();
        y = snake.back().getCoord().y();

        rnd = QRandomGenerator::global()->bounded(4);
        newCoord = snake.back().getCoord();
        switch (rnd) {
        case 0:
            newCoord.ry()++;
            break;
        case 1:
            newCoord.ry()--;
            break;
        case 2:
            newCoord.rx()++;
            break;
        case 3:
            newCoord.rx()--;
            break;
        }

        if (newCoord.x() > -1 && newCoord.y() > -1 && newCoord.x() < fieldSize.width() && newCoord.y() < fieldSize.height()) {
            QCell newCell = QCell(owner, newCoord.y(), newCoord.x());
            if (!snake.contains(newCell))
                this->snake.push_back(newCell);
        }
    }

    this->direction = this->snake.value(0).getCoord() - this->snake.value(1).getCoord();

    this->body.load(":/Images/body.png");
    this->corner.load(":/Images/corner.png");
    this->head.load(":/Images/head.png");
    this->tail.load(":/Images/tail.png");
}

QSnake::~QSnake()
{
    snake.clear();
}

QSet<QPoint> QSnake::getQSetSnake()
{
    QSet<QPoint> busyCells;

    for (int i = 0; i < this->snake.size(); i++) {
        busyCells.insert(snake.value(i).getCoord());
    }

    return busyCells;
}

int QSnake::dirToAngle(QPoint dir)
{
    if (dir == dirLeft)
        return 180;
    if (dir == dirRight)
        return 0;
    if (dir == dirUp)
        return -90;
    if (dir == dirDown)
        return 90;
    return 0;
}
void QSnake::draw()
{
    int angle;
    QImage img;
    QTransform trans;

    for (int i = 0; i < snake.size(); i++) {
        angle = 0;
        if (i == 0) {
            img = head;
            angle = dirToAngle(direction);
        } else if (i == snake.size() - 1) {
            img = tail;
            // Calc direction to prev element
            // QCrell(i-1) - QCell(i) convert
            // result will be QPoint (QSnake::dirLeft, QSnake::dirRight, ...)
            // Convert direction to angle
            QPoint dirPrev = (snake.value(i - 1) - snake.value(i)).getCoord();
            dirPrev = standardizeDir(dirPrev);
            angle = dirToAngle(dirPrev);
        } else {
            QPoint dirPrev = (snake.value(i - 1) - snake.value(i)).getCoord();
            QPoint dirNext = (snake.value(i) - snake.value(i + 1)).getCoord();
            dirPrev = standardizeDir(dirPrev);
            dirNext = standardizeDir(dirNext);
            if (dirPrev == dirNext) {
                img = body;
                angle = dirToAngle(dirPrev);
            } else {
                img = corner;
                if (((dirPrev == dirRight) && (dirNext == dirUp)) || ((dirPrev == dirDown) && (dirNext == dirLeft)))
                    angle = 0;
                else if (((dirPrev == dirUp) && (dirNext == dirLeft)) || ((dirPrev == dirRight) && (dirNext == dirDown)))
                    angle = -90;
                else if (((dirPrev == dirUp) && (dirNext == dirRight)) || ((dirPrev == dirLeft) && (dirNext == dirDown)))
                    angle = 180;
                else if (((dirPrev == dirDown) && (dirNext == dirRight)) || ((dirPrev == dirLeft) && (dirNext == dirUp)))
                    angle = 90;
            }
        }

        trans.reset();
        trans.translate(img.width() / 2, img.height() / 2);
        trans.rotate(angle);

        snake.value(i).draw(img.transformed(trans));
    }
}

QPoint QSnake::moveHead(QPoint dir, bool transparentBorders)
{

    QSize fieldSize = owner->getFieldSize();

    this->snake.push_front(QCell(snake.front()));

    if (this->direction + dir != QPoint(0, 0))
        this->direction = dir;

    this->snake.front() += this->direction;

    int x = this->snake.front().getCoord().x();
    int y = this->snake.front().getCoord().y();
    if (!transparentBorders) {
        if (x >= fieldSize.width() || x < 0 || y >= fieldSize.height() || y < 0) {
            return QPoint(-1, -1);
        }
    } else {
        if (x < 0)
            this->snake.front().setCoord(QPoint(fieldSize.width() - 1, y));
        if (x >= fieldSize.width())
            this->snake.front().setCoord(QPoint(0, y));
        if (y < 0)
            this->snake.front().setCoord(QPoint(x, fieldSize.height() - 1));
        if (y >= fieldSize.height())
            this->snake.front().setCoord(QPoint(x, 0));
    }
    // || newSnakePos.getCoord().x() < 0 || newSnakePos.getCoord().y() >= fieldSize.height() || newSnakePos.getCoord().y() < 0) {

    return this->snake.front().getCoord();
}

void QSnake::moveTail()
{
    this->snake.pop_back();
}

bool QSnake::checkCollision()
{
    for (int i = 1; i < snake.size(); i++)
        if (snake.front().getCoord() == snake.value(i).getCoord())
            return true;
    return false;
}

QPoint QSnake::standardizeDir(QPoint dir)
{
    //    QSize fieldSize = owner->getFieldSize();
    if (dir.x() > 1 && dir.y() == 0) {
        dir = QPoint(-1, 0);
    } else if (dir.x() < -1 && dir.y() == 0) {
        dir = QPoint(1, 0);
    } else if (dir.x() == 0 && dir.y() > 1) {
        dir = QPoint(0, -1);
    } else if (dir.x() == 0 && dir.y() < -1) {
        dir = QPoint(0, 1);
    }

    return dir;
}
