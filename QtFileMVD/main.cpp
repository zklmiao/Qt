#include "qtmyfile_mvd.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtMyFile_MVD w;
    w.show();

    return a.exec();
}
