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
    friend class Test;
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
    QString hash(QByteArray &chunks);
    QString hash(QString &sToHash, unsigned char *digest);

    void encrypt(const QByteArray &toEncrypt, QByteArray &encrypted, const unsigned char *key, size_t originalsize);
    void decrypt(const QByteArray &toDecrypt, QByteArray &decrypted, const unsigned char *key);

    QCommandLineParser *mParser;
    bool mEncryptBool, mDecryptBool;
    QString mDest{""};
    QString mSource{""};
    QString mHash{""};
    QString mKey{""};
};

#endif // MAINCONSOLE_H
