#include "hashcryptographer.h"

#include <QCryptographicHash>
#include <QDataStream>
#include <QDebug>

#include <tools.h>


HashCryptographer::HashCryptographer(QObject *parent) : Cryptographer(parent)
{
	pRealKey = new HashKeyCipher(this);
	pFakeKey = new HashKeyCipher(this);
}

bool HashCryptographer::encrypt(const QVariant &parameters)
{
	if (!Cryptographer::encrypt(parameters))
	{
		return false;
	}
	HashKeyCipher *realKey = qobject_cast<HashKeyCipher *>(pRealKey);
	HashKeyCipher *fakeKey = qobject_cast<HashKeyCipher *>(pFakeKey);
	if (!realKey || !fakeKey || !parameters.canConvert<QCryptographicHash::Algorithm>())
	{
		qDebug() << Q_FUNC_INFO << "keys or parameters is not valid";
		return false;
	}
	QCryptographicHash::Algorithm algorithm = parameters.value<QCryptographicHash::Algorithm>();
	qsrand(time(0));
	while (!realInputDevice->atEnd() || !fakeInputDevice->atEnd())
	{
		unsigned char realInputData = 0x00;
		unsigned char fakeInputData = 0x00;
		if (!realInputDevice->atEnd())
		{
			realInputDevice->read((char*)&realInputData, 1);
		}
		if (!fakeInputDevice->atEnd())
		{
			fakeInputDevice->read((char*)&fakeInputData, 1);
		}
		quint32 randomNumber = 0;
		QByteArray randomKey;
		QDataStream in(&randomKey, QIODevice::WriteOnly);
		do
		{
			in.device()->seek(0);
			randomNumber = qrand();
			in << randomNumber;

			big_int hash1 = Tools::big_intFromByteArray<quint64>(QCryptographicHash::hash(realKey->data() + randomKey, algorithm));
			if ((unsigned char)((hash1 % 256)) != realInputData)
			{
				continue;
			}

			big_int hash2 = Tools::big_intFromByteArray<quint64>(QCryptographicHash::hash(fakeKey->data() + randomKey, algorithm));
			if ((unsigned char)((hash2 % 256)) != fakeInputData)
			{
				continue;
			}
			outputDevice->write(randomKey.mid(0, 4));
			break;
		} while (true);
	}
	return true;
}

bool HashCryptographer::decrypt(Device deviceNumber, Key keyNumber, const QVariant &parameters)
{
	if (!Cryptographer::decrypt(deviceNumber, keyNumber, parameters))
	{
		return false;
	}
	HashKeyCipher *key = qobject_cast<HashKeyCipher *>(keyFromNumber(keyNumber));
	if (!key || !parameters.canConvert<QCryptographicHash::Algorithm>())
	{
		qDebug() << Q_FUNC_INFO << "key or parameters is not valid";
		return false;
	}
	QCryptographicHash::Algorithm algorithm = parameters.value<QCryptographicHash::Algorithm>();
	QIODevice *device = deviceFromNumber(deviceNumber);
	while (!device->atEnd())
	{
		big_int hash = Tools::big_intFromByteArray<quint64>(QCryptographicHash::hash(key->data() + device->read(4), algorithm));
		unsigned char data = static_cast<unsigned char>(hash % 256);
		outputDevice->write((const  char*)(&data), 1);
	}
	return true;
}

HashKeyCipher::HashKeyCipher(QObject *parent) : KeyCipher(parent)
{
}

bool HashKeyCipher::load(const QByteArray &ba)
{
	if (!KeyCipher::load(ba))
	{
		return false;
	}
	mData = ba;
	return true;
}

QByteArray HashKeyCipher::data()
{
	return mData;
}

Q_DECLARE_METATYPE(QCryptographicHash::Algorithm)
