#include <QtGui/QApplication>
#include "opencvgl.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OpenCVGL w;
    w.show();

    return a.exec();
}
