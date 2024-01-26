#include "qsnakewidget.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QSnakeWidget w;
    w.show();
    return a.exec();
}
