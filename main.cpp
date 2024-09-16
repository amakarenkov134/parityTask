#include "paritybits.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ParityBits w;
    w.show();
    return a.exec();
}
