#include "mainconsole.h"

MainConsole::MainConsole(QCommandLineParser *parser, QObject *parent) : QObject(parent), mParser(parser)
{
//    qDebug() << "Hello world?";
    if(!parseOpts())
        return;
}

MainConsole::~MainConsole()
{

}

bool MainConsole::MainConsole::parseOpts()
{
    const QStringList args = mParser->positionalArguments();
    if(args.size() == 2)
    {
        mSource = args.at(0); mDest = args.at(1);
    }

    mEncryptBool = mParser->isSet("encrypt");
    mDecryptBool = mParser->isSet("decrypt");


    if(mParser->isSet("hash"))
        mHash = mParser->value("hash");

    if(!(mEncryptBool || mDecryptBool))
    {
        qDebug() << "[ERROR] You must set if you want to decrypt or encrypt";
        return false;
    }

    if(mSource.isEmpty() || mDest.isEmpty())
    {
        qDebug() << "[ERROR] Source or destination can't be empty";
        return false;
    }

    QFileInfo testSource(mSource);
    QFileInfo testDest(mDest);

    if(!testSource.exists() || !testSource.isFile())
    {
        qDebug() << "Invalid Source File";
        return false;
    }

    if(!testDest.isWritable())
    {
        qDebug() << "[ERROR] Can't write to this destination";
        return false;
    }

    return true;
}


