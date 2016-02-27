#include "mainconsole.h"

MainConsole::MainConsole(QCommandLineParser *parser, QObject *parent) : QObject(parent), mParser(parser)
{
//    qDebug() << "Hello world?";

}

MainConsole::~MainConsole()
{

}

void MainConsole::init()
{
    if(!parseOpts())
    {
        emit exitNormal();
        return;
    }

    QFile sourceFile(mSource);




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
    if(mParser->isSet("key"))
        mKey = mParser->value("key");


    if(!(mEncryptBool || mDecryptBool) || (mEncryptBool && mDecryptBool))
    {
        qDebug() << "[ERROR] You must set if you want to decrypt or encrypt!";
        return false;
    }

    if(mKey.isEmpty())
    {
        qDebug() << "[ERROR] You really need some key for this...";
    }

    if(mDecryptBool && mHash.isEmpty())
    {
        qDebug() << "[ERROR] you must set hash (file or hash itself) for verification";
    }

    if(mSource.isEmpty() || mDest.isEmpty())
    {
        qDebug() << "[ERROR] Source or destination can't be empty, use -h/--help";
        return false;
    }

    QFileInfo testSource(mSource);
    QFileInfo testDest(mDest);

    if(!testSource.exists() || !testSource.isFile())
    {
        qDebug() << "Invalid Source File";
        return false;
    }

    if(testDest.exists() /* or something else TODO */)
    {
        qDebug() << "[ERROR] Can't write to this destination, use -h/--help";
        return false;
    }

    return true;
}


