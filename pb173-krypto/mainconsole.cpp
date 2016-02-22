#include "mainconsole.h"

MainConsole::MainConsole(QObject *parent) : QObject(parent)
{
    qDebug() << "Hello world?";
}

MainConsole::~MainConsole()
{

}

