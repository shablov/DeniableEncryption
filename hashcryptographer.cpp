#include "hashcryptographer.h"

#include <QCryptographicHash>
#include <QDataStream>
#include <QDebug>

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wreorder"
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"

#include <big_int.hpp>

#pragma GCC diagnostic pop

using namespace Arageli;

template <class T>
big_int big_intFromByteArray(const QByteArray &ba)
{
	size_t size = sizeof(T);
	if (ba.size() % size != 0)
	{
		switch (size)
		{
			case 8: return big_intFromByteArray<quint32>(ba);
			case 4: return big_intFromByteArray<quint16>(ba);
			case 2: return big_intFromByteArray<quint8>(ba);
			default: break;
		}
	}
	static int s = 0;
	if (s != ba.size())
	{
		s = ba.size();
	}
	big_int result = 0;
	T data;
	QDataStream out(ba);
	while (!out.atEnd())
	{
		out >> data;
		result <<= size;
		result += data;
	}
	return result;
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
		big_int hash = big_intFromByteArray<quint64>(QCryptographicHash::hash(key->data() + device->read(4), algorithm));
		unsigned char data = static_cast<unsigned char>(hash % quint8(-1));
		outputDevice->write((const  char*)(&data), 1);
	}
	return true;
}

HashCryptographer::HashCryptographer(QObject *parent) : Cryptographer(parent)
{
	pRealKey = new HashKeyCipher(this);
	pFakeKey = new HashKeyCipher(this);
}

bool HashCryptographer::crypt(const QVariant &parameters)
{
	if (!Cryptographer::crypt(parameters))
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

			big_int hash1 = big_intFromByteArray<quint64>(QCryptographicHash::hash(realKey->data() + randomKey, algorithm));
			if ((unsigned char)((hash1 % quint8(-1))) != realInputData)
			{
				continue;
			}

			big_int hash2 = big_intFromByteArray<quint64>(QCryptographicHash::hash(fakeKey->data() + randomKey, algorithm));
			if ((unsigned char)((hash2 % quint8(-1))) != fakeInputData)
			{
				continue;
			}
			outputDevice->write(randomKey.mid(0, 4));
			break;
		} while (true);
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
