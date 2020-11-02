#include "accounting.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    qApp->addLibraryPath("C:/customPath/plugins");
    qApp->addLibraryPath("C:/Qt/Qt5.5.0/5.5/mingw492_32/plugins");

    Accounting accounting;
    accounting.run();

    return a.exec();
}
