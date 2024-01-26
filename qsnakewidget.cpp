#include "qsnakewidget.h"
#include "qsnake.h"

#include <QHBoxLayout>

QSnakeWidget::QSnakeWidget(QWidget* parent)
    : QWidget(parent)
{
    this->setWindowTitle(tr("Snake"));
    this->setGeometry(0, 0, 600, 600);

    this->snakeGame = new QSnakeGame(this);
    this->snakeGame->setGeometry(10, 10, 400, 400);

    this->buttonState = new QPushButton(this);
    this->buttonState->setText("Start");
    connect(buttonState, SIGNAL(pressed()), this, SLOT(gamePlayPause()));
    connect(snakeGame, SIGNAL(stateChanged()), this, SLOT(changeButtonText()));

    QHBoxLayout* hl = new QHBoxLayout;
    hl->addWidget(snakeGame);
    hl->addWidget(buttonState);

    QShortcut* shortcut = nullptr;

    shortcut = new QShortcut(QKeySequence(Qt::Key_Left), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(moveLeft()));
    shortcut = new QShortcut(QKeySequence(Qt::Key_Right), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(moveRight()));
    shortcut = new QShortcut(QKeySequence(Qt::Key_Up), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(moveUp()));
    shortcut = new QShortcut(QKeySequence(Qt::Key_Down), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(moveDown()));
    shortcut = new QShortcut(QKeySequence(Qt::Key_Space), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(gamePlayPause()));
    //    shortcut = new QShortcut(QKeySequence(Qt::Key_Up), this);
    //    connect(shortcut, SIGNAL(activated()), this, SLOT(moveUp()));

    this->setLayout(hl);
}

QSnakeWidget::~QSnakeWidget()
{
    delete this->snakeGame;
}

void QSnakeWidget::gamePlayPause()
{
    if (snakeGame->getCurrentState() == QSnakeGame::stateRunning)
        snakeGame->setState(QSnakeGame::stateOnPause);
    else
        snakeGame->setState(QSnakeGame::stateRunning);
}

void QSnakeWidget::changeButtonText()
{
    int state = snakeGame->getCurrentState();

    if (state == QSnakeGame::stateRunning)
        buttonState->setText("Pause");
    else if (state == QSnakeGame::stateOnPause)
        buttonState->setText("Play");
    else
        buttonState->setText("Restart");
}

void QSnakeWidget::mousePressEvent(QMouseEvent* event)
{
    pressedX = event->pos().x();
    pressedY = event->pos().y();
    QWidget::mousePressEvent(event);
}
void QSnakeWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (!moved)
        gamePlayPause();
    moved = false;
    QWidget::mouseReleaseEvent(event);
}

void QSnakeWidget::mouseMoveEvent(QMouseEvent* event)
{
    int difX = event->pos().x() - pressedX;
    int difY = event->pos().y() - pressedY;
    if (!moved && (abs(difX) > touchSize || abs(difY) > touchSize)) { // ??????????????
        if (abs(difX) > abs(difY)) {
            if (difX > touchSize)
                moveRight();
            else if (difX < touchSize)
                moveLeft();
        } else if (abs(difX) < abs(difY)) {
            if (difY > touchSize)
                moveDown();
            else if (difY < touchSize)
                moveUp();
        }
        moved = true;
    }
    //    if (!moved && abs(difX) > touchSize && abs(difY) > touchSize) {
    //        if (abs(difX) > abs(difY)) {

    //        }
    //    }
    QWidget::mouseMoveEvent(event);
}
