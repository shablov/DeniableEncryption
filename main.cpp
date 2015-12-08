#include "hashcryptographer.h"
#include "mainwindow.h"
#include <QApplication>

#include <QCryptographicHash>
#include <QDebug>

int main(int argc, char *argv[])
{
	Q_UNUSED(argc);
	Q_UNUSED(argv);

	HashCryptographer *cryptographer = new HashCryptographer;
	QByteArray first = "hello worldewir";
	QByteArray second = "world hello ";
	cryptographer->setDevice(first, Cryptographer::RealDevice);
	cryptographer->setDevice(second, Cryptographer::FakeDevice);
	QByteArray data(qMax(first.count(), second.count()), 0x00);
	cryptographer->setDevice(data, Cryptographer::OutputDevice);
	cryptographer->setKey(QByteArray("real key"), Cryptographer::RealKey);
	cryptographer->setKey(QByteArray("fake key"), Cryptographer::FakeKey);
	QVariant algorithm = QVariant::fromValue<QCryptographicHash::Algorithm>(QCryptographicHash::Md4);
	cryptographer->crypt(algorithm);
	qDebug() << data.toHex();

	QByteArray decryptData(qMax(first.count(), second.count()), 0x00);
	cryptographer->setDevice(data, Cryptographer::RealDevice);
	cryptographer->setDevice(data, Cryptographer::FakeDevice);
	cryptographer->setDevice(decryptData, Cryptographer::OutputDevice);
	cryptographer->decrypt(Cryptographer::RealDevice, Cryptographer::RealKey, algorithm);
	qDebug() << decryptData;
	cryptographer->decrypt(Cryptographer::FakeDevice, Cryptographer::FakeKey, algorithm);
	qDebug() << decryptData;
	return 0;
}

Q_DECLARE_METATYPE(QCryptographicHash::Algorithm)
