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
    if(mParser != nullptr) //else tester class should set all variables
        if(!parseOpts())
        {
            emit exitNormal();
            return;
        }


    QFile sourceFile(mSource);

    if(!sourceFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "[Error] can't open a file";
        exit(1);
    }

    QByteArray sByteArray;
    QByteArray dByteArray;

/*-----------------------------LOAD FILE-----------------------*/
    sByteArray.append(sourceFile.readAll());
    qint64 readSize = sByteArray.size();
    if(mEncryptBool)
        if(readSize%16 != 0)       //PKCS#7 padding
        {
            int toPad = (16-(readSize%16));
            char *padding = (char*)malloc(toPad);
            for(int i = 0;i<toPad;++i)
                padding[i] = (char)toPad;
            sByteArray.append(padding, toPad);
        }
    sourceFile.close();

    unsigned char key[32];
    QString usedKeyString = hash(mKey, key);
    QString controlHash;

    if(mEncryptBool)
    {
        encrypt(sByteArray, dByteArray, key, sourceFile.size());
        controlHash = hash(dByteArray);
    }
    else
    {
        controlHash = hash(sByteArray);
        if(controlHash != mHash)
        {
            qDebug() << "!!!HASH IS NOT PROVIDED OR NOT EQUAL!!!";
//            exit(0);
        }
        else
        {
            qDebug() << "HASHES ARE EQUAL!";
        }
        decrypt(sByteArray, dByteArray, key);
    }

    QFile destFile(mDest);
    if (!destFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "[ERROR] could not create file";
        exit(1);
    }

    destFile.write(dByteArray);

    destFile.close();

    qDebug() << "Operation successfully finished";
    if(mEncryptBool)
    {
        qDebug() << "Hash of encrypted file is " << controlHash;
        qDebug() << "SAVE THIS HASH FOR LATER USE!";
    }


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

QString MainConsole::hash(QByteArray &chunks)
{
    unsigned char digest[64];
    char out[128];
    memset(digest, 0, 64);
    memset(out, 0, 128);
    mbedtls_sha512((const unsigned char*)chunks.constData(),chunks.size(), digest, 0);
    for(int i = 0; i<64; ++i)
    {
        sprintf(out+i*2, "%02x", digest[i]);
    }
    QString qOut(out);
    return qOut;
}

QString MainConsole::hash(QString &sToHash, unsigned char *digest)
{
    char out[64];
    memset(out, 0, 64);
    memset(digest, 0, 32);
    mbedtls_sha256((const unsigned char*)sToHash.toStdString().c_str(), sToHash.size(), digest, 0);
    for(int i = 0; i < 32; ++i )
    {
        sprintf(out+i*2, "%02x", digest[i]);
    }
    QString qOut(out);
    return qOut;
}


void MainConsole::encrypt(const QByteArray &toEncrypt, QByteArray &encrypted, const unsigned char* key, size_t originalsize)
{
    unsigned char iv[16];
    qsrand(QDateTime::currentDateTime().toTime_t());
    for(int i = 0; i<16; ++i)
        iv[i] = qrand();
    encrypted.append((const char*)iv, 16);
    unsigned char* out;
    out = (unsigned char*)malloc(toEncrypt.size());
    memset(out, 0, toEncrypt.size());

    mbedtls_aes_context aescon;
    mbedtls_aes_setkey_enc(&aescon, key, 256);
    mbedtls_aes_crypt_cbc(&aescon, MBEDTLS_AES_ENCRYPT, toEncrypt.size(), iv, (const unsigned char*)toEncrypt.constData(), out);
    encrypted.append((const char*)out, toEncrypt.size());
    mbedtls_aes_free(&aescon);
    long unsigned int writesize = originalsize;
    encrypted.append((const char*)&writesize, sizeof(long unsigned int));

    free(out);
}

void MainConsole::decrypt(const QByteArray &toDecrypt, QByteArray &decrypted, const unsigned char *key)
{
    unsigned char *out;

    long unsigned int *originalsize = (long unsigned int*)(toDecrypt.data()+(toDecrypt.size()-sizeof(long unsigned int)));

    unsigned char *iv;
    iv = (unsigned char*)malloc(16);
    memcpy(iv, (toDecrypt.data()), 16);

    out = (unsigned char*)malloc(toDecrypt.size()-16-sizeof(long unsigned int));
    memset(out, 0, *originalsize);

    mbedtls_aes_context aescon;
    mbedtls_aes_setkey_dec(&aescon, key, 256);
    mbedtls_aes_crypt_cbc(&aescon, MBEDTLS_AES_DECRYPT, (toDecrypt.size()-16-sizeof(long unsigned int)), iv, (const unsigned char*)toDecrypt.data()+16, out);
    mbedtls_aes_free(&aescon);
    decrypted.append((char*)out, *originalsize);
    free(iv);
    free(out);
}


