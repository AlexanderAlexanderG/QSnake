#include "qsnake.h"

#include <QPainter>

QCell::QCell(QSnakeGame* owner, int row, int col)
{
    this->owner = owner;
    this->row = row;
    this->col = col;
}

QCell::QCell()
{
    this->owner = nullptr;
    this->row = 0;
    this->col = 0;
}

void QCell::draw(const QImage& img)
{
    QPainter qp(owner);
    QSize cellSize = owner->getCellSize();
    qp.drawImage(QRect(col * cellSize.width(),
                     row * cellSize.height(),
                     cellSize.width(),
                     cellSize.height()),
        img);
}

QCell QCell::operator=(QCell cell)
{
    this->row = cell.row;
    this->col = cell.col;
    this->owner = cell.owner;

    return *this;
}

bool QCell::operator==(const QCell& cell) const
{
    return this->row == cell.row && this->col == cell.col;
}

QCell QCell::operator+(QCell cell)
{
    return QCell(this->owner, this->row + cell.row, this->col + cell.col);
}

QCell QCell::operator-(QCell cell)
{
    return QCell(this->owner, this->row - cell.row, this->col - cell.col);
}

QCell QCell::operator+=(QPoint point)
{
    this->row += point.y();
    this->col += point.x();

    return *this;
}
