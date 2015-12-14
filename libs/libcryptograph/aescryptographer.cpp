#include "aescryptographer.h"

#include <QDebug>

/** AES default (check keys and parameters) **/
/** -------------------------------------------------------------------------------------------------------------------------- **/

AESCryptographer::AESCryptographer(QObject *parent) : Cryptographer(parent)
{
	pRealKey = new AESKeyCipher(this);
	pFakeKey = new AESKeyCipher(this);
}

int AESCryptographer::blockSize()
{
	return 16;
}

bool AESCryptographer::encrypt(const QVariant &parameters)
{
	if (!Cryptographer::encrypt(parameters))
	{
		return false;
	}
	AESKeyCipher *realKey = qobject_cast<AESKeyCipher *>(pRealKey);
	AESKeyCipher *fakeKey = qobject_cast<AESKeyCipher *>(pFakeKey);
	AESKeySize keySize = static_cast<AESKeySize>(parameters.toInt() / 8 * 8);
	if (!realKey || !fakeKey || realKey->keySize() != keySize || fakeKey->keySize() != keySize)
	{
		qDebug() << Q_FUNC_INFO << "keys or parameters is not valid";
		return false;
	}
	return true;
}

bool AESCryptographer::decrypt(Cryptographer::Device deviceNumber, Cryptographer::Key keyNumber, const QVariant &parameters)
{
	if (!Cryptographer::decrypt(deviceNumber, keyNumber, parameters))
	{
		return false;
	}
	AESKeyCipher *key = qobject_cast<AESKeyCipher *>(keyFromNumber(keyNumber));
	AESKeySize keySize = static_cast<AESKeySize>(parameters.toInt() / 8 * 8);
	if (!key || key->keySize() != keySize)
	{
		qDebug() << Q_FUNC_INFO << "key or parameters is not valid";
		return false;
	}
	return true;
}

QByteArray AESCryptographer::encryptAES(const QByteArray &data, AESKeyCipher *key, EncryptionType type)
{
	QByteArray cipherData(16, 0x00);
	Q_UNUSED(type)
	switch (key->keySize())
	{
		case AES_128:
		{
			intel_AES_enc128((UCHAR*)data.data(), (UCHAR*)cipherData.data(), key->value(), 1);
			break;
		}
		case AES_192:
		{
			intel_AES_enc192((UCHAR*)data.data(), (UCHAR*)cipherData.data(), key->value(), 1);
			break;
		}
		case AES_256:
		{
			intel_AES_enc256((UCHAR*)data.data(), (UCHAR*)cipherData.data(), key->value(), 1);
			break;
		}
	}
	return cipherData;
}

QByteArray AESCryptographer::decryptAES(const QByteArray &cipherData, AESKeyCipher *key, AESCryptographer::EncryptionType type)
{
	QByteArray decryptData(16, 0x00);
	Q_UNUSED(type)
	switch (key->keySize())
	{
		case AES_128:
		{
			intel_AES_dec128((UCHAR*)cipherData.data(), (UCHAR*)decryptData.data(), key->value(), 1);
			break;
		}
		case AES_192:
		{
			intel_AES_dec192((UCHAR*)cipherData.data(), (UCHAR*)decryptData.data(), key->value(), 1);
			break;
		}
		case AES_256:
		{
			intel_AES_dec256((UCHAR*)cipherData.data(), (UCHAR*)decryptData.data(), key->value(), 1);
			break;
		}
	}
	return decryptData;
}

/** AES as Hash (division by modulo) **/
/** -------------------------------------------------------------------------------------------------------------------------- **/

AESCryptographerHash::AESCryptographerHash(QObject *parent) : AESCryptographer(parent)
{
}

bool AESCryptographerHash::encrypt(const QVariant &parameters)
{
	if (!AESCryptographer::encrypt(parameters))
	{
		return false;
	}
	EncryptionType type = static_cast<EncryptionType>(parameters.toInt() % 8);
	AESKeyCipher *realKey = qobject_cast<AESKeyCipher *>(pRealKey);
	AESKeyCipher *fakeKey = qobject_cast<AESKeyCipher *>(pFakeKey);
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

			if ((quint8)(Tools::big_intFromByteArray<quint64>(encryptAES(randomKey, realKey, type)) % 256) != realInputData)
			{
				continue;
			}

			if ((quint8)(Tools::big_intFromByteArray<quint64>(encryptAES(randomKey, fakeKey, type)) % 256) != fakeInputData)
			{
				continue;
			}

			outputDevice->write(randomKey.mid(12, 4));
			break;
		} while (true);
	}
	endEncrypt();
	return true;
}

bool AESCryptographerHash::decrypt(Device deviceNumber, Key keyNumber, const QVariant &parameters)
{
	if (!AESCryptographer::decrypt(deviceNumber, keyNumber, parameters))
	{
		return false;
	}
	EncryptionType type = static_cast<EncryptionType>(parameters.toInt() % 8);
	AESKeyCipher *key = qobject_cast<AESKeyCipher *>(keyFromNumber(keyNumber));
	QIODevice *device = deviceFromNumber(deviceNumber);
	QByteArray cipherData(16, 0x00);
	while (!device->atEnd())
	{
		device->read(cipherData.data() + 12, 4);
		quint8 data = static_cast<quint8>(Tools::big_intFromByteArray<quint64>(encryptAES(cipherData, key, type)) % 256);
		outputDevice->write((const  char*)(&data), 1);
	}
	endDecrypt(deviceNumber);
	return true;
}

/** AES Chinese remainder theorem **/
/** -------------------------------------------------------------------------------------------------------------------------- **/

AESCryptographerChinese::AESCryptographerChinese(QObject *parent) : AESCryptographer(parent)
{
	mSecondRealKey = new AESChineseKeyCiper(this);
	mSecondFakeKey = new AESChineseKeyCiper(this);
}

bool AESCryptographerChinese::encrypt(const QVariant &parameters)
{
	if (!AESCryptographer::encrypt(parameters))
	{
		return false;
	}
	if (!mSecondRealKey->isValid() || !mSecondFakeKey->isValid())
	{
		endEncrypt();
		qDebug() << "second keys not valid";
		return false;
	}
	EncryptionType type = static_cast<EncryptionType>(parameters.toInt() % 8);
	AESKeyCipher *realKey = qobject_cast<AESKeyCipher *>(pRealKey);
	AESKeyCipher *fakeKey = qobject_cast<AESKeyCipher *>(pFakeKey);
	intermediateRealKey = mSecondFakeKey->value() * inverse_mod(mSecondFakeKey->value(), mSecondRealKey->value());
	intermediateFakeKey = mSecondRealKey->value() * inverse_mod(mSecondRealKey->value(), mSecondFakeKey->value());
	big_int modulo = mSecondRealKey->value() * mSecondFakeKey->value();
	while (!realInputDevice->atEnd() || !fakeInputDevice->atEnd())
	{
		QByteArray realInputData(16, 0x00);
		QByteArray fakeInputData(16, 0x00);
		if (!realInputDevice->atEnd())
		{
			realInputDevice->read(realInputData.data(), 16);
		}
		if (!fakeInputDevice->atEnd())
		{
			fakeInputDevice->read(fakeInputData.data(), 16);
		}
		big_int C1 = Tools::big_intFromByteArray<quint64>(encryptAES(realInputData, realKey, type));
		big_int C2 = Tools::big_intFromByteArray<quint64>(encryptAES(fakeInputData, fakeKey, type));
		big_int C = mod(C1 * intermediateRealKey + C2 * intermediateFakeKey, modulo);
		QByteArray result = Tools::big_intToByteArray(C);
		result = result.rightJustified(33, 0x00);
		outputDevice->write(result);
	}
	endEncrypt();
	return true;
}
bool AESCryptographerChinese::decrypt(Cryptographer::Device deviceNumber, Cryptographer::Key keyNumber, const QVariant &parameters)
{
	Q_UNUSED(deviceNumber);
	Q_UNUSED(keyNumber);
	Q_UNUSED(parameters);

	if (!AESCryptographer::decrypt(deviceNumber, keyNumber, parameters))
	{
		return false;
	}
	AESChineseKeyCiper *secondKey = keyNumber == Cryptographer::RealKey ? mSecondRealKey : mSecondFakeKey;
	if (!secondKey->isValid())
	{
		endDecrypt(deviceNumber);
		qDebug() << "second key not valid";
		return false;
	}
	EncryptionType type = static_cast<EncryptionType>(parameters.toInt() % 8);
	AESKeyCipher *key = qobject_cast<AESKeyCipher *>(keyFromNumber(keyNumber));
	QIODevice *device = deviceFromNumber(deviceNumber);
	QByteArray cipherData(33, 0x00);
	while (!device->atEnd())
	{
		device->read(cipherData.data(), 33);
		big_int C = mod(Tools::big_intFromByteArray<quint64>(cipherData), secondKey->value());
		QByteArray cipher = Tools::big_intToByteArray(C);
		cipher = cipher.rightJustified(16, 0x00);
		outputDevice->write(decryptAES(cipher, key, type));
	}
	endDecrypt(deviceNumber);
	return true;
}

KeyCipher *AESCryptographerChinese::secondRealKey()
{
	return mSecondRealKey;
}

KeyCipher *AESCryptographerChinese::secondFakeKey()
{
	return mSecondFakeKey;
}

void AESCryptographerChinese::setSecondRealKey(const QByteArray &ba)
{
	mSecondRealKey->load(ba);
}

void AESCryptographerChinese::setSecondFakeKey(const QByteArray &ba)
{
	mSecondFakeKey->load(ba);
}

void AESCryptographerChinese::generateSecondKeys()
{
	big_int secondRealKey = big_int::random_with_length(128 + 1);
	big_int secondFakeKey;
	do
	{
		secondFakeKey = big_int::random_with_length(128 + 1);
	}
	while (!(euclid<big_int>(secondRealKey, secondFakeKey).is_unit()));
	mSecondRealKey->load(Tools::big_intToByteArray(secondRealKey));
	mSecondFakeKey->load(Tools::big_intToByteArray(secondFakeKey));
}

/** AES key **/
/** -------------------------------------------------------------------------------------------------------------------------- **/

AESKeyCipher::AESKeyCipher(QObject *parent) : KeyCipher(parent)
{
}

bool AESKeyCipher::load(const QByteArray &ba)
{
	if (!KeyCipher::load(ba))
	{
		return false;
	}
	if (ba.size() !=AESCryptographerHash::AES_128 &&
		ba.size() != AESCryptographerHash::AES_192 &&
		ba.size() != AESCryptographerHash::AES_256)
	{
		qDebug() << Q_FUNC_INFO << "size of key is not compatible";
		valid = false;
		return false;
	}
	mData = ba;
	valid = true;
	return true;
}

QByteArray AESKeyCipher::data() const
{
	return mData;
}

UCHAR *AESKeyCipher::value() const
{
	return (UCHAR*)mData.data();
}

int AESKeyCipher::keySize() const
{
	return mData.size();
}

/** AES Chinese key **/
/** -------------------------------------------------------------------------------------------------------------------------- **/

AESChineseKeyCiper::AESChineseKeyCiper(QObject *parent) : KeyCipher(parent)
{
}

bool AESChineseKeyCiper::load(const QByteArray &ba)
{
	if (!KeyCipher::load(ba))
	{
		return false;
	}
	if (ba.size() != keySize())
	{
		qDebug() << Q_FUNC_INFO << "size of key is not compatible";
		valid = false;
		return false;
	}
	mData = Tools::big_intFromByteArray<quint64>(ba);
	valid = true;
	return true;
}

QByteArray AESChineseKeyCiper::data() const
{
	return Tools::big_intToByteArray(mData);
}

big_int AESChineseKeyCiper::value() const
{
	return mData;
}

int AESChineseKeyCiper::keySize() const
{
	return AESCryptographer::blockSize() + 1;
}
