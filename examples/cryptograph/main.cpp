#include "cryptograph/aescryptographer.h"
#include "cryptograph/hashcryptographer.h"
#include <QCoreApplication>

#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <datadevice.h>

int main(int argc, char *argv[])
{
	Q_UNUSED(argc);
	Q_UNUSED(argv);

	QByteArray realData(1*1024, 0x00);
	QByteArray fakeData(1*1024, 0xFF);
	QByteArray cipherData;
	QByteArray decryptData;
	QByteArray realKey = "real key";
	QByteArray fakeKey = "fake key";
	QVariant algorithm = QVariant::fromValue<QCryptographicHash::Algorithm>(QCryptographicHash::Md4);
	QVariant parameters = QVariant::fromValue(AESCryptographer::AES_256);
//	qDebug() << "--------------- HASH Md4 ---------------";
//	qDebug() << "Input data";
//	qDebug() << "Real data:" << realData.toHex();
//	qDebug() << "Fake data:" << fakeData.toHex();
//	qDebug() << "Real key:" << realKey;
//	qDebug() << "Fake key:" << fakeKey;
//	HashCryptographer *cryptographer = new HashCryptographer;
//	cryptographer->setDevice(realData, Cryptographer::RealDevice);
//	cryptographer->setDevice(fakeData, Cryptographer::FakeDevice);
//	cryptographer->setDevice(cipherData, Cryptographer::OutputDevice);
//	cryptographer->setKey(realKey, Cryptographer::RealKey);
//	cryptographer->setKey(fakeKey, Cryptographer::FakeKey);
//	qDebug() << "Begin encrypt in:" << QTime::currentTime();
//	cryptographer->encrypt(algorithm);
//	qDebug() << "End encrypt in:" << QTime::currentTime();
//	qDebug() << "Cryptogram in hex:" << cipherData.toHex();

//	cryptographer->setDevice(cipherData, Cryptographer::RealDevice);
//	cryptographer->setDevice(cipherData, Cryptographer::FakeDevice);
//	cryptographer->setDevice(decryptData, Cryptographer::OutputDevice);
//	qDebug() << "Begin decrypt real data in:" << QTime::currentTime();
//	cryptographer->decrypt(Cryptographer::RealDevice, Cryptographer::RealKey, algorithm);
//	qDebug() << "End decrypt real data in:" << QTime::currentTime();
//	qDebug() << "Decrypted real data" << decryptData.toHex();
//	qDebug() << "Begin decrypt fake data in:" << QTime::currentTime();
//	cryptographer->decrypt(Cryptographer::FakeDevice, Cryptographer::FakeKey, algorithm);
//	qDebug() << "End decrypt fake data in:" << QTime::currentTime();
//	qDebug() << "Decrypted fake data" << decryptData.toHex();
//	qDebug() << "\n";


//	cipherData.clear();
	realKey = "0123456789ABCDEF0123456789ABCDEF";
	fakeKey = "FEDCBA9876543210FEDCBA9876543210";
//	qDebug() << "--------------- AES 256 ---------------";
//	qDebug() << "Input data";
//	qDebug() << "Real data:" << realData.toHex();
//	qDebug() << "Fake data:" << fakeData.toHex();
//	qDebug() << "Real key:" << realKey;
//	qDebug() << "Fake key:" << fakeKey;
//	AESCryptographerHash *crypt = new AESCryptographerHash;
//	crypt->setDevice(realData, Cryptographer::RealDevice);
//	crypt->setDevice(fakeData, Cryptographer::FakeDevice);
//	crypt->setDevice(cipherData, Cryptographer::OutputDevice);
//	crypt->setKey(realKey, Cryptographer::RealKey);
//	crypt->setKey(fakeKey, Cryptographer::FakeKey);
//	qDebug() << "Begin encrypt in:" << QTime::currentTime();
//	crypt->encrypt(parameters);
//	qDebug() << "End encrypt in:" << QTime::currentTime();
//	qDebug() << "Cryptogram in hex:" << cipherData.toHex();

//	crypt->setDevice(cipherData, Cryptographer::RealDevice);
//	crypt->setDevice(cipherData, Cryptographer::FakeDevice);
//	crypt->setDevice(decryptData, Cryptographer::OutputDevice);
//	qDebug() << "Begin decrypt real data in:" << QTime::currentTime();
//	crypt->decrypt(Cryptographer::RealDevice, Cryptographer::RealKey, parameters);
//	qDebug() << "End decrypt real data in:" << QTime::currentTime();
//	qDebug() << "Decrypted real data" << decryptData.toHex();
//	qDebug() << "Begin decrypt fake data in:" << QTime::currentTime();
//	crypt->decrypt(Cryptographer::FakeDevice, Cryptographer::FakeKey, parameters);
//	qDebug() << "End decrypt fake data in:" << QTime::currentTime();
//	qDebug() << "Decrypted fake data" << decryptData.toHex();
//	qDebug() << "\n";


	cipherData.clear();
//	QFile realFile("./../generator/random1.bin");
//	QFile fakeFile("./../generator/random2.bin");
	QFile cipherFile("cipher.bin");
	realKey = "sfiojfijo309*(#9f3o23&*@!)ncdlsd";
	fakeKey = "ojasd(&25d&^adh&D&Ajfe0)LSD!SKCs";
//	qDebug() << "--------------- AES 256 ---------------";
//	qDebug() << "Input data";
//	qDebug() << "Real data:" << realData.toHex();
//	qDebug() << "Fake data:" << fakeData.toHex();
//	qDebug() << "Real key:" << realKey;
//	qDebug() << "Fake key:" << fakeKey;
	AESCryptographer *cryptChinese = new AESCryptographerHash;
	cryptChinese->setDevice(DataDeviceFactory::device(RandomDevice), Cryptographer::RealDevice);
	cryptChinese->setDevice(DataDeviceFactory::device(RandomDevice), Cryptographer::FakeDevice);
	cryptChinese->setDevice(new StdUIntOutputDevice, Cryptographer::OutputDevice);
	cryptChinese->setKey(realKey, Cryptographer::RealKey);
	cryptChinese->setKey(fakeKey, Cryptographer::FakeKey);
//	cryptChinese->generateSecondKeys();
	cryptChinese->generateInitialValue();
//	KeyCipher *secondRealKey = cryptChinese->secondRealKey();
//	KeyCipher *secondFakeKey = cryptChinese->secondFakeKey();
//	qDebug() << "Begin encrypt in:" << QTime::currentTime();
	cryptChinese->encrypt(parameters);
//	qDebug() << "End encrypt in:" << QTime::currentTime();
//	qDebug() << "Cryptogram in hex:" << cipherData.toHex();

//	QFile realDecryptFile("randomdecrypt1.bin");
//	QFile fakeDecryptFile("randomdecrypt2.bin");
//	cryptChinese->setDevice(&cipherFile, Cryptographer::RealDevice);
//	cryptChinese->setDevice(&cipherFile, Cryptographer::FakeDevice);
//	cryptChinese->setDevice(&realDecryptFile, Cryptographer::OutputDevice);
//	cryptChinese->setSecondRealKey(secondRealKey->data());
//	cryptChinese->setSecondFakeKey(secondFakeKey->data());
//	qDebug() << "Begin decrypt real data in:" << QTime::currentTime();
//	cryptChinese->decrypt(Cryptographer::RealDevice, Cryptographer::RealKey, parameters);
//	qDebug() << "End decrypt real data in:" << QTime::currentTime();
//	realDecryptFile.open(QIODevice::ReadOnly);
//	realFile.open(QIODevice::ReadOnly);
//	QByteArray ba = realDecryptFile.readAll();
//	qDebug() << (ba == QByteArray(ba.size(), 0xAA));
//	realDecryptFile.close();
//	realFile.close();
//	qDebug() << "Decrypted real data" << decryptData.toHex();
//	qDebug() << "Begin decrypt fake data in:" << QTime::currentTime();
//	cryptChinese->setDevice(&fakeDecryptFile, Cryptographer::OutputDevice);
//	cryptChinese->decrypt(Cryptographer::FakeDevice, Cryptographer::FakeKey, parameters);
//	fakeDecryptFile.open(QIODevice::ReadOnly);
//	fakeFile.open(QIODevice::ReadOnly);
//	ba = fakeDecryptFile.readAll();
//	qDebug() << (ba == QByteArray(ba.size(), 0xFF));
//	fakeDecryptFile.close();
//	fakeFile.close();
//	qDebug() << "End decrypt fake data in:" << QTime::currentTime();
//	qDebug() << "Decrypted fake data" << decryptData.toHex();
//	qDebug() << "\n";

	return 0;
}

Q_DECLARE_METATYPE(QCryptographicHash::Algorithm)
Q_DECLARE_METATYPE(AESCryptographer::AESKeySize)
