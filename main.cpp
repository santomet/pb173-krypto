#include "mainconsole.h"
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("marek-eav");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;

    parser.setApplicationDescription("Marek - Encrypt and Verify");

    parser.addPositionalArgument("source", "Source file to hash and encrypt/decrypt and verify");
    parser.addPositionalArgument("dest", "Destination of encrypted/decrypted file");
    parser.addHelpOption();

    QCommandLineOption key(QStringList() << "k" << "key", "Password for encryption/decryption. It will be hashed with SHA256 and the digest then used as a key", "password");
    QCommandLineOption decrypt(QStringList() << "d" << "decrypt", "Decrypt and verify hash");
    QCommandLineOption encrypt(QStringList() << "e" << "encrypt", "Encrypt and make hash");
    QCommandLineOption hash(QStringList() << "s" << "hash", "Hash to verify","hash");
    parser.addOptions(QList<QCommandLineOption>() << encrypt << decrypt << key << hash);

    parser.process(a);

    MainConsole n(&parser, &a);
    //QObject::connect(&n, SIGNAL(exitNormal()), &a, SLOT(exit()));

    QTimer::singleShot(0, &n, SLOT(init()));

    return a.exec();
}
