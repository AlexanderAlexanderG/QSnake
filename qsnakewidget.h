#ifndef QSNAKEWIDGET_H
#define QSNAKEWIDGET_H

#include <QMouseEvent>
#include <QPushButton>
#include <QShortcut>
#include <QWidget>
#include <qsnake.h>

class QSnakeWidget : public QWidget {
    Q_OBJECT

public:
    QSnakeWidget(QWidget* parent = nullptr);
    ~QSnakeWidget();

private slots:
    void moveUp() { snakeGame->setNewDirection(QSnake::dirUp); }
    void moveDown() { snakeGame->setNewDirection(QSnake::dirDown); }
    void moveLeft() { snakeGame->setNewDirection(QSnake::dirLeft); }
    void moveRight() { snakeGame->setNewDirection(QSnake::dirRight); }

    void gamePlayPause();

    void changeButtonText();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    QSnakeGame* snakeGame = nullptr;
    QPushButton* buttonState = nullptr;

    int pressedX = 0;
    int pressedY = 0;
    int touchSize = 5;

    bool moved = false;
};
#endif // QSNAKEWIDGET_H
