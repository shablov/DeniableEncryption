#include "aescryptographer.h"

#include <QDebug>

#include "tools.h"

AESCryptographer::AESCryptographer(QObject *parent) : Cryptographer(parent)
{
	pRealKey = new AESKeyCipher(this);
	pFakeKey = new AESKeyCipher(this);
}

bool AESCryptographer::encrypt(const QVariant &parameters)
{

	if (!Cryptographer::encrypt(parameters))
	{
		return false;
	}
	AESKeyCipher *realKey = qobject_cast<AESKeyCipher *>(pRealKey);
	AESKeyCipher *fakeKey = qobject_cast<AESKeyCipher *>(pFakeKey);
	if (!realKey || !fakeKey)
	{
		qDebug() << Q_FUNC_INFO << "keys or parameters is not valid";
		return false;
	}
	while (!realInputDevice->atEnd() || !fakeInputDevice->atEnd())
	{
		quint8 realInputData = 0x00;
		quint8 fakeInputData = 0x00;
		if (!realInputDevice->atEnd())
		{
			realInputDevice->read((char*)&realInputData, 1);
		}
		if (!fakeInputDevice->atEnd())
		{
			fakeInputDevice->read((char*)&fakeInputData, 1);
		}

		quint32 randomNumber = 0;
		QByteArray randomKey(16, 0x00);
		QDataStream in(&randomKey, QIODevice::WriteOnly);
		do
		{
			in.device()->seek(12);
			randomNumber = qrand();
			in << randomNumber;

			QByteArray cipherText(16, 0x00);
			intel_AES_enc128((UCHAR*)randomKey.data(), (UCHAR*)cipherText.data(), realKey->data(), 1);
			if ((quint8)(big_intFromByteArray<quint64>(cipherText) % 256) != realInputData)
			{
				continue;
			}

			intel_AES_enc128((UCHAR*)randomKey.data(), (UCHAR*)cipherText.data(), fakeKey->data(), 1);
			if ((quint8)(big_intFromByteArray<quint64>(cipherText) % 256) != fakeInputData)
			{
				continue;
			}

			outputDevice->write(randomKey.mid(12, 4));
			break;
		} while (true);
	}
	return true;
}

bool AESCryptographer::decrypt(Device deviceNumber, Key keyNumber, const QVariant &parameters)
{
	if (!Cryptographer::decrypt(deviceNumber, keyNumber, parameters))
	{
		return false;
	}
	AESKeyCipher *key = qobject_cast<AESKeyCipher *>(keyFromNumber(keyNumber));
	if (!key)
	{
		qDebug() << Q_FUNC_INFO << "key or parameters is not valid";
		return false;
	}
	QIODevice *device = deviceFromNumber(deviceNumber);
	QByteArray decryptionText(16, 0x00);
	QByteArray cipherData(16, 0x00);
	while (!device->atEnd())
	{
		device->read(cipherData.data() + 12, 4);
		intel_AES_enc128((UCHAR*)cipherData.data(), (UCHAR*)decryptionText.data(), key->data(), 1);
		quint8 data = static_cast<quint8>(big_intFromByteArray<quint64>(decryptionText) % 256);
		outputDevice->write((const  char*)(&data), 1);
	}
	return true;
}

AESKeyCipher::AESKeyCipher(QObject *parent) : KeyCipher(parent)
{

}

bool AESKeyCipher::load(const QByteArray &ba)
{
	if (!KeyCipher::load(ba))
	{
		return false;
	}
	if (ba.size() != 16 && ba.size() != 24 && ba.size() != 32)
	{
		qDebug() << Q_FUNC_INFO << "size of key is not compatible";
		valid = false;
		return false;
	}
	mData = ba;
}

UCHAR *AESKeyCipher::data()
{
	return (UCHAR*)mData.data();
}

int AESKeyCipher::keySize()
{
	return mData.size();
}
