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

    if(!sourceFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "[Error] can't open a file";
    }

    QVector<QByteArray> sByteArray((sourceFile.size()/16));

    //I know that Vector of ByteArrays is not the most effective way but i just wanted to touch qtConcurrent through this homework...
    //Yes, i actually hope that mbedTLS is made to be threadsafe

    while(!sourceFile.atEnd())
    {
        char *chunkData;
        qint64 readSize = sourceFile.read(chunkData, 16);

        if(readSize < 16 && readSize > 0)       //PKCS#7 padding
        {
            char toPad = (char)(16-readSize);
            chunkData = (char*)realloc((void*)chunkData, 16);
            for(int i = readSize-1;i<16;++i)
                chunkData[i] = (char)toPad;
        }
        sByteArray.append(QByteArray(chunkData,16));
    }
    sourceFile.close();

    if(mEncryptBool)
        QVector<QByteArray> dByteArray = encrypt(sByteArray);
    else
        QVector<QByteArray> dByteArray = decrypt(sByteArray);



    exit(0);


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

QVector<QByteArray> &MainConsole::encrypt(QVector<QByteArray> &chunks)
{
//    QVector<QByteArray> d = QtConcurrent::blockingMapped(chunks, encryptChunk);
    QVector<QByteArray> d;
    d.resize(chunks.size());
    foreach(QByteArray &chunk, chunks)
    {
        d.append(encryptChunk(chunk));
    }

    return d;
}

QVector<QByteArray> &MainConsole::decrypt(QVector<QByteArray> &chunks)
{
//    QVector<QByteArray> d = QtConcurrent::blockingMapped(chunks, decryptChunk);

    QVector<QByteArray> d;
    d.resize(chunks.size());
    foreach(QByteArray &chunk, chunks)
    {
        d.append(decryptChunk(chunk));
    }

    return d;
}

QString MainConsole::hash(QVector<QByteArray> &chunks)
{
    unsigned char digest[64];
    char out[128];
    memset(digest, 0, 64);
    memset(out, 0, 128);

    mbedtls_sha512_context shacon;
    mbedtls_sha512_init(&shacon);
    mbedtls_sha512_starts(&shacon, 0);





}

QString MainConsole::hash(QString &sToHash, unsigned char *bitstring)
{
    unsigned char digest[32];
    char out[64];
    memset(out, 0, 64);
    memset(digest, 0, 32);
    mbedtls_sha256((const unsigned char*)sToHash.toStdString().c_str(), sToHash.size(), digest, 0);
    for(int i = 0; i < 32; i++ ) {
            sprintf(out+i*2, "%02x", digest[i]);
        }
    QString qOut(out);
    return qOut;
}

QByteArray MainConsole::encryptChunk(const QByteArray &chunk)
{
    char* fuck = (char*)malloc(16);
    return QByteArray(fuck,16);
}

QByteArray MainConsole::decryptChunk(const QByteArray &chunk)
{
    char* fuck = (char*)malloc(16);
    return QByteArray(fuck,16);
}


