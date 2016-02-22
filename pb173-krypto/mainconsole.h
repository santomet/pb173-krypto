#ifndef MAINCONSOLE_H
#define MAINCONSOLE_H

#include <QDebug>
#include <QObject>

class MainConsole : public QObject
{
    Q_OBJECT
public:
    explicit MainConsole(QObject *parent = 0);
    ~MainConsole();

signals:

public slots:
};

#endif // MAINCONSOLE_H
