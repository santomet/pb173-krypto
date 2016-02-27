#ifndef MAINCONSOLE_H
#define MAINCONSOLE_H

#include <QDebug>
#include <QObject>
#include <QCommandLineParser>
#include <QString>
#include <mbedtls/aes.h>
#include <mbedtls/sha512.h>
#include <mbedtls/sha256.h>
#include <mbedtls/pkcs5.h>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QtConcurrent/QtConcurrent>
#include <bitset>




class MainConsole : public QObject
{
    Q_OBJECT
public:
    explicit MainConsole(QCommandLineParser *parser, QObject *parent = 0);
    ~MainConsole();


signals:
    void exitNormal();
    void exitError();

public slots:
    void init();

private:
    bool parseOpts();
    QVector<QByteArray> &encrypt(QVector<QByteArray> &chunks);
    QVector<QByteArray> &decrypt(QVector<QByteArray> &chunks);
    QString hash(QVector<QByteArray> &chunks);
    QString hash(QString &sToHash, unsigned char *bitstring);

    static QByteArray encryptChunk(const QByteArray &chunk);
    static QByteArray decryptChunk(const QByteArray &chunk);

    static void hexify2( unsigned char *obuf, const unsigned char *ibuf, int len )
    {
        unsigned char l, h;

        while( len != 0 )
        {
            h = *ibuf / 16;
            l = *ibuf % 16;

            if( h < 10 )
                *obuf++ = '0' + h;
            else
                *obuf++ = 'a' + h - 10;

            if( l < 10 )
                *obuf++ = '0' + l;
            else
                *obuf++ = 'a' + l - 10;

            ++ibuf;
            len--;
        }
    }

    QCommandLineParser *mParser;
    bool mEncryptBool, mDecryptBool;
    QString mDest{""};
    QString mSource{""};
    QString mHash{""};
    QString mKey{""};
};

#endif // MAINCONSOLE_H
