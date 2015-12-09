#include "aescryptographer.h"
#include "hashcryptographer.h"
#include "mainwindow.h"
#include <QApplication>

#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>

int main(int argc, char *argv[])
{
	Q_UNUSED(argc);
	Q_UNUSED(argv);

	QByteArray realData(16, 0x00);
	QByteArray fakeData(16, 0xFF);
	QByteArray cipherData;
	QByteArray decryptData;
	QByteArray realKey = "real key";
	QByteArray fakeKey = "fake key";
	qDebug() << "--------------- HASH Md4 ---------------";
	qDebug() << "Input data:";
	qDebug() << "Real data:" << realData.toHex();
	qDebug() << "Fake data:" << fakeData.toHex();
	qDebug() << "Real key:" << realKey;
	qDebug() << "Fake key:" << fakeKey;
	HashCryptographer *cryptographer = new HashCryptographer;
	cryptographer->setDevice(realData, Cryptographer::RealDevice);
	cryptographer->setDevice(fakeData, Cryptographer::FakeDevice);
	cryptographer->setDevice(cipherData, Cryptographer::OutputDevice);
	cryptographer->setKey(realKey, Cryptographer::RealKey);
	cryptographer->setKey(fakeKey, Cryptographer::FakeKey);
	QVariant algorithm = QVariant::fromValue<QCryptographicHash::Algorithm>(QCryptographicHash::Md4);
	qDebug() << "Begin encrypt in:" << QTime::currentTime();
	cryptographer->encrypt(algorithm);
	qDebug() << "End encrypt in:" << QTime::currentTime();
	qDebug() << "Cryptogram in hex:" << cipherData.toHex();

	cryptographer->setDevice(cipherData, Cryptographer::RealDevice);
	cryptographer->setDevice(cipherData, Cryptographer::FakeDevice);
	cryptographer->setDevice(decryptData, Cryptographer::OutputDevice);
	qDebug() << "Begin decrypt real data in:" << QTime::currentTime();
	cryptographer->decrypt(Cryptographer::RealDevice, Cryptographer::RealKey, algorithm);
	qDebug() << "End decrypt real data in:" << QTime::currentTime();
	qDebug() << "Decrypted real data" << decryptData.toHex();
	qDebug() << "Begin decrypt fake data in:" << QTime::currentTime();
	cryptographer->decrypt(Cryptographer::FakeDevice, Cryptographer::FakeKey, algorithm);
	qDebug() << "End decrypt fake data in:" << QTime::currentTime();
	qDebug() << "Decrypted fake data" << decryptData.toHex();
	qDebug() << "\n";


	realKey = "0123456789ABCDEF";
	fakeKey = "FEDCBA9876543210";
	qDebug() << "--------------- AES 128 ---------------";
	qDebug() << "Input data:";
	qDebug() << "Real data:" << realData.toHex();
	qDebug() << "Fake data:" << fakeData.toHex();
	qDebug() << "Real key:" << realKey;
	qDebug() << "Fake key:" << fakeKey;
	AESCryptographer *crypt = new AESCryptographer;
	crypt->setDevice(realData, Cryptographer::RealDevice);
	crypt->setDevice(fakeData, Cryptographer::FakeDevice);
	crypt->setDevice(cipherData, Cryptographer::OutputDevice);
	crypt->setKey(fakeData, Cryptographer::RealKey);
	crypt->setKey(realData, Cryptographer::FakeKey);
	qDebug() << "Begin encrypt in:" << QTime::currentTime();
	crypt->encrypt();
	qDebug() << "End encrypt in:" << QTime::currentTime();
	qDebug() << "Cryptogram in hex:" << cipherData.toHex();

	crypt->setDevice(cipherData, Cryptographer::RealDevice);
	crypt->setDevice(cipherData, Cryptographer::FakeDevice);
	crypt->setDevice(decryptData, Cryptographer::OutputDevice);
	qDebug() << "Begin decrypt real data in:" << QTime::currentTime();
	crypt->decrypt(Cryptographer::RealDevice, Cryptographer::RealKey);
	qDebug() << "End decrypt real data in:" << QTime::currentTime();
	qDebug() << "Decrypted real data" << decryptData.toHex();
	qDebug() << "Begin decrypt fake data in:" << QTime::currentTime();
	crypt->decrypt(Cryptographer::FakeDevice, Cryptographer::FakeKey);
	qDebug() << "End decrypt fake data in:" << QTime::currentTime();
	qDebug() << "Decrypted fake data" << decryptData.toHex();
	qDebug() << "\n";
	return 0;
}

Q_DECLARE_METATYPE(QCryptographicHash::Algorithm)
