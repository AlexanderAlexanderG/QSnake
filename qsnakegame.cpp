#include "qsnake.h"

#include <QDebug>
#include <QPainter>
#include <QRandomGenerator>

QSnakeGame::QSnakeGame(QWidget* parent)
    : QWidget(parent)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerStep()));
    initGame();
}

QSnakeGame::~QSnakeGame()
{
    foods.clear();
    delete this->snake;
    this->snake = nullptr;
}

void QSnakeGame::setFieldSize(QSize newSize)
{
    if (newSize.width() < 4)
        newSize.setWidth(4);
    if (newSize.width() > 64)
        newSize.setWidth(64);

    if (newSize.height() < 4)
        newSize.setHeight(4);
    if (newSize.height() > 64)
        newSize.setHeight(64);

    this->fieldSize = newSize;

    this->setMinimumSize(fieldSize.width() * cellMinSize,
        fieldSize.height() * cellMinSize);
}

void QSnakeGame::drawGrid()
{
    QPainter qp(this);
    qp.setPen(QPen(Qt::gray));
    for (int i = 1; i < fieldSize.width(); i++)
        qp.drawLine(i * cellSize.width(), 0, i * cellSize.width(), this->height());
    for (int i = 1; i < fieldSize.height(); i++)
        qp.drawLine(0, i * cellSize.height(), this->width(), i * cellSize.height());
}

void QSnakeGame::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    cellSize.setWidth(this->width() / fieldSize.width());
    cellSize.setHeight(this->height() / fieldSize.height());

    drawGrid();

    foreach (auto food, foods) {
        food.draw();
    }
    snake->draw();

    if (currentState == stateGameOver)
        drawMessage(tr("GAME OVER!"), Qt::red);
    else if (currentState == stateWin)
        drawMessage(tr("WIN!"), Qt::red);
}

QPoint QSnakeGame::getRandFreeCell()
{

    QSet<QPoint> freeCells;

    for (int i = 0; i < fieldSize.width(); i++) {
        for (int j = 0; j < fieldSize.height(); j++) {
            freeCells.insert(QPoint(i, j));
        }
    }

    if (snake != nullptr)
        freeCells.subtract(snake->getQSetSnake());

    foreach (auto food, foods) {
        freeCells.remove(food.getCoord());
    }

    if (freeCells.isEmpty())
        return QPoint(-1, -1);

    int rnd = QRandomGenerator::global()->bounded(freeCells.size());

    QList<QPoint> vals = freeCells.values();
    return vals.at(rnd);
}

void QSnakeGame::setNewDirection(QPoint dir)
{
    if (dir == QSnake::dirDown || dir == QSnake::dirUp || dir == QSnake::dirLeft || dir == QSnake::dirRight)
        this->newDirection = dir;
}

void QSnakeGame::snakeMove()
{
    QPoint newSnakePos = snake->moveHead(this->newDirection, transparentBorders);

    if (newSnakePos == QPoint(-1, -1)) {
        currentState = stateGameOver;
        emit stateChanged();
        // timer.stop();
        return;
    }

    bool eats = false;
    for (auto food = foods.begin(); food != foods.end(); food++) {
        if (food->checkCollision(newSnakePos)) {
            qDebug() << newSnakePos.x() << ":" << newSnakePos.y();
            eats = true;
            if (food->move() == QPoint(-1, -1)) {
                currentState = stateWin;
                emit stateChanged();
                return;
            }
        }
    }
    if (!eats)
        snake->moveTail();

    if (snake->checkCollision()) {
        currentState = stateGameOver;
        emit stateChanged();
        // timer.stop();
        return;
    }
}

void QSnakeGame::timerStep()
{
    if (currentState == stateRunning) {
        snakeMove();
    }
    if (parentWidget() != nullptr) {
        // parentWidget()->update();
        parentWidget()->repaint();
    } else {
        // this->update();
        this->repaint();
    }
}

void QSnakeGame::drawMessage(QString message, const QColor color)
{
    QPainter qp(this);

    QFont font("Courier", 15, QFont::DemiBold);
    font.setPixelSize(width() / (message.size() + 1));

    QFontMetrics fm(font);
    int textWidth = fm.horizontalAdvance(message);

    qp.setPen(color);
    qp.setFont(font);
    qp.drawText((width() - textWidth) / 2, height() / 2, message);
}

/*
void QSnakeGame::gamePlayPause()
{
    if (currentState == stateOnPause) {
        currentState = stateRunning;
        emit stateChanged();
    } else if (currentState == stateRunning) {
        currentState = stateOnPause;
        emit stateChanged();
    } else if (currentState == stateWin || currentState == stateGameOver) {
        gameRestart();
        emit stateChanged();
    }
}
*/

void QSnakeGame::gameRestart()
{
    initGame();
}

void QSnakeGame::initGame()
{
    // To do: get field size, snake size, food amount as parameters
    this->setFieldSize(QSize(8, 8));

    if (this->snake != nullptr) {
        delete this->snake;
        this->snake = nullptr;
    }
    this->snake = new QSnake(this, 6);

    foods.clear();
    this->foods.push_back(QFood(this, 1, 1));
    this->foods.push_back(QFood(this, 0, 0));

    this->cellSize = QSize(8, 8); // Changes automatically during the resize (QPaintEvent)

    this->newDirection = QSnake::dirLeft;

    // Restarts timer and sets it to pending state, change snake's state
    timer.setInterval(snakeInitDelay);
    timer.stop();
    this->currentState = stateOnPause;
    emit stateChanged();

    this->repaint();
}

void QSnakeGame::setState(int newState)
{
    if (newState == stateRunning) {
        if (currentState == stateOnPause) {
            currentState = stateRunning;
            emit stateChanged();
            timer.start();
        } else if (currentState == stateWin || currentState == stateGameOver)
            gameRestart();
    } else if (newState == stateOnPause) {
        if (currentState == stateRunning) {
            currentState = stateOnPause;
            emit stateChanged();
        }
    }
}
