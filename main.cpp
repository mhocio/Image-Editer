#include "photoapp.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PhotoApp w;
    w.show();
    return a.exec();
}
