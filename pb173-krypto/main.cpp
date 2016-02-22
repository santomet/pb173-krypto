#include "mainconsole.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MainConsole n();

    return a.exec();
}
