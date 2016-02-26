#include "mainconsole.h"
#include <QCoreApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("marek-eav");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;

    parser.setApplicationDescription("Marek - Encrypt and Verify");

    parser.addPositionalArgument("source", "Source file to hash and encrypt/decrypt and verify");
    parser.addPositionalArgument("dest", "Destination of encrypted/decrypted file");

    QCommandLineOption decrypt("d", "Decrypt and verify hash");
    QCommandLineOption encrypt("e", "Encrypt and make hash");
    QCommandLineOption hash(QStringList() << "h" << "hash", "Destination/Source file of hash or hash itself. If not defined for encryption, it will be writtend on stdout",
                            "hash");
    parser.addOptions(QList<QCommandLineOption>() << encrypt << decrypt << hash);

    parser.process(a);

    MainConsole n(&parser);

    return a.exec();
}
