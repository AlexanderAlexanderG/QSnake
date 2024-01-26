#ifndef QSNAKE_H
#define QSNAKE_H

#include <QImage>
#include <QRandomGenerator>
#include <QTimer>
#include <QWidget>

class QSnakeGame;
class QSnake;

class QCell {
public:
    QCell();
    QCell(QSnakeGame* owner, int row, int col);
    QCell(const QCell& copy)
        : row(copy.row)
        , col(copy.col)
        , owner(copy.owner)
    {
    }

    void draw(const QImage& img);
    QPoint getCoord() { return QPoint(col, row); }
    void setCoord(QPoint qp)
    {
        col = qp.x();
        row = qp.y();
    }

    QCell operator=(QCell cell);
    QCell operator+(QCell cell);
    QCell operator-(QCell cell);

    QCell operator+=(QPoint point);
    bool operator==(const QCell& cell) const;

protected:
    int row, col;
    QSnakeGame* owner;
};

class QFood : public QCell {
public:
    QFood(QSnakeGame* owner, int row, int col);

    QPoint move();
    void draw();

    bool checkCollision(QPoint cell);

private:
    QImage apple;
};

class QSnake {
public:
    QSnake(QSnakeGame* owner, int);
    ~QSnake();
    static constexpr QPoint dirRight = { 1, 0 };
    static constexpr QPoint dirLeft = { -1, 0 };
    static constexpr QPoint dirUp = { 0, -1 };
    static constexpr QPoint dirDown = { 0, 1 };

    void draw();

    QSet<QPoint> getQSetSnake(void);
    bool checkCollision();

    QPoint moveHead(QPoint dir, bool);
    void moveTail();

protected:
private:
    QList<QCell> snake;

    QImage body;
    QImage corner;
    QImage head;
    QImage tail;

    QPoint direction;

    QSnakeGame* owner;

    QPoint standardizeDir(QPoint);
    int dirToAngle(QPoint dir);
};

class QSnakeGame : public QWidget {
    Q_OBJECT

public:
    QSnakeGame(QWidget* parent = nullptr);
    ~QSnakeGame();

    static const int cellMinSize = 8;

    static const int stateOnPause = 1;
    static const int stateRunning = 2;
    static const int stateGameOver = 3;
    static const int stateWin = 4;

    static const int snakeInitDelay = 500;
    static const int snakeMinDelay = 200;
    static const int snakeStepDelay = 100;

    QSize getCellSize(void) { return this->cellSize; }
    QSize getFieldSize(void) { return this->fieldSize; }
    QPoint getRandFreeCell(void);
    int getCurrentState() { return this->currentState; }

    void setNewDirection(QPoint newDirection);

signals:
    // void stateChanged(int);
    void stateChanged();

protected:
    void paintEvent(QPaintEvent*) override;

public slots:
    void setState(int newState);

private slots:
    void timerStep();
    // void gamePlayPause();

private:
    QTimer timer;

    QSnake* snake = nullptr;
    QList<QFood> foods;

    QSize fieldSize = QSize(16, 16);
    QSize cellSize = QSize(8, 8);

    QPoint newDirection = QSnake::dirLeft;

    int currentState = stateRunning;

    void gameRestart();

    bool transparentBorders = true;

    void snakeMove();

    void setFieldSize(QSize);
    void drawGrid();

    void drawMessage(QString message, const QColor color);

    void initGame();
};

#endif // QSNAKE_H
