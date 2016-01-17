#include "aescryptographer.h"

#define bool
#include "iaesni.h"
#undef bool

#include <QDebug>
#include <QFormLayout>
#include <QHBoxLayout>
#include <datadevice.h>

#include <QtWidgets/QComboBox>

/** AES default (check keys and parameters) **/
/** -------------------------------------------------------------------------------------------------------------------------- **/

AESCryptographer::AESCryptographer(QObject *parent) : Cryptographer(parent)
{
	pRealKey = new AESKeyCipher(this);
	pFakeKey = new AESKeyCipher(this);
	qsrand(time(0));
}

int AESCryptographer::blockSize()
{
	return 16;
}

void AESCryptographer::setInitialValue(const QByteArray &initial)
{
	mInitialValue = initial;
}

QByteArray AESCryptographer::initialValue() const
{
	return mInitialValue;
}

void AESCryptographer::generateInitialValue()
{
	srand(time(0));
	mInitialValue = Tools::big_intToByteArray(big_int::random_with_length(blockSize() * 8));
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

QByteArray AESCryptographer::encryptAES(const QByteArray &inputData, AESKeyCipher *key, EncryptionType type, const QByteArray &initial)
{
	QByteArray cipherData(16, 0x00);
	switch (key->keySize() + static_cast<int>(type))
	{
		case AES_128 + ECB:
		{
			intel_AES_enc128((UCHAR*)inputData.data(), (UCHAR*)cipherData.data(),
							 key->value(), inputData.count() / blockSize());
			break;
		}
		case AES_128 + CBC:
		{
			intel_AES_enc128_CBC((UCHAR*)inputData.data(), (UCHAR*)cipherData.data(), key->value(),
								 inputData.count() / blockSize(), (UCHAR*)initial.data());
			break;
		}
		case AES_128 + CTR:
		{
			intel_AES_encdec128_CTR((UCHAR*)inputData.data(), (UCHAR*)cipherData.data(), key->value(),
									inputData.count() / blockSize(), (UCHAR*)initial.data());
			break;
		}
		case AES_192 + ECB:
		{
			intel_AES_enc192((UCHAR*)inputData.data(), (UCHAR*)cipherData.data(),
							 key->value(), inputData.count() / blockSize());
			break;
		}
		case AES_192 + CBC:
		{
			intel_AES_enc192_CBC((UCHAR*)inputData.data(), (UCHAR*)cipherData.data(), key->value(),
								 inputData.count() / blockSize(), (UCHAR*)initial.data());
			break;
		}
		case AES_192 + CTR:
		{
			intel_AES_encdec192_CTR((UCHAR*)inputData.data(), (UCHAR*)cipherData.data(), key->value(),
									inputData.count() / blockSize(), (UCHAR*)initial.data());
			break;
		}
		case AES_256 + ECB:
		{
			intel_AES_enc256((UCHAR*)inputData.data(), (UCHAR*)cipherData.data(),
							 key->value(), inputData.count() / blockSize());
			break;
		}
		case AES_256 + CBC:
		{
			intel_AES_enc256_CBC((UCHAR*)inputData.data(), (UCHAR*)cipherData.data(), key->value(),
								 inputData.count() / blockSize(), (UCHAR*)initial.data());
			break;
		}
		case AES_256 + CTR:
		{
			intel_AES_encdec256_CTR((UCHAR*)inputData.data(), (UCHAR*)cipherData.data(), key->value(),
									inputData.count() / blockSize(), (UCHAR*)initial.data());
			break;
		}
	}
	return cipherData;
}

QByteArray AESCryptographer::decryptAES(const QByteArray &cipherData, AESKeyCipher *key, EncryptionType type, const QByteArray &initial)
{
	QByteArray decryptData(16, 0x00);
	Q_UNUSED(type)
	switch (key->keySize() + static_cast<int>(type))
	{
		case AES_128 + ECB:
		{
			intel_AES_dec128((UCHAR*)cipherData.data(), (UCHAR*)decryptData.data(),
							 key->value(), cipherData.count() / blockSize());
			break;
		}
		case AES_128 + CBC:
		{
			intel_AES_dec128_CBC((UCHAR*)cipherData.data(), (UCHAR*)decryptData.data(), key->value(),
								 cipherData.count() / blockSize(), (UCHAR*)initial.data());
			break;
		}
		case AES_128 + CTR:
		{
			intel_AES_encdec128_CTR((UCHAR*)cipherData.data(), (UCHAR*)decryptData.data(), key->value(),
									cipherData.count() / blockSize(), (UCHAR*)initial.data());
			break;
		}
		case AES_192 + ECB:
		{
			intel_AES_dec192((UCHAR*)cipherData.data(), (UCHAR*)decryptData.data(),
							 key->value(), cipherData.count() / blockSize());
			break;
		}
		case AES_192 + CBC:
		{
			intel_AES_dec192_CBC((UCHAR*)cipherData.data(), (UCHAR*)decryptData.data(), key->value(),
								 cipherData.count() / blockSize(), (UCHAR*)initial.data());
			break;
		}
		case AES_192 + CTR:
		{
			intel_AES_encdec192_CTR((UCHAR*)cipherData.data(), (UCHAR*)decryptData.data(), key->value(),
									cipherData.count() / blockSize(), (UCHAR*)initial.data());
			break;
		}
		case AES_256 + ECB:
		{
			intel_AES_dec256((UCHAR*)cipherData.data(), (UCHAR*)decryptData.data(),
							 key->value(), cipherData.count() / blockSize());
			break;
		}
		case AES_256 + CBC:
		{
			intel_AES_dec256_CBC((UCHAR*)cipherData.data(), (UCHAR*)decryptData.data(), key->value(),
								 cipherData.count() / blockSize(), (UCHAR*)initial.data());
			break;
		}
		case AES_256 + CTR:
		{
			intel_AES_encdec256_CTR((UCHAR*)cipherData.data(), (UCHAR*)decryptData.data(), key->value(),
									cipherData.count() / blockSize(), (UCHAR*)initial.data());
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
	if (type != ECB && mInitialValue.count() != blockSize())
	{
		endEncrypt();
		qDebug() << "initial value not valid";
		return false;
	}
	AESKeyCipher *realKey = qobject_cast<AESKeyCipher *>(pRealKey);
	AESKeyCipher *fakeKey = qobject_cast<AESKeyCipher *>(pFakeKey);
	QByteArray realInitialValue;
	realInitialValue.insert(0, mInitialValue);
	QByteArray fakeInitialValue;
	fakeInitialValue.insert(0, mInitialValue);

	QByteArray randomKey(16, 0x00);
	RandomDataDevice dev;
	dev.open(QIODevice::ReadOnly);
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
		QDataStream in(&randomKey, QIODevice::WriteOnly);
		do
		{
//			in.device()->seek(12);
//			randomNumber = qrand();
//			in << randomNumber;
			dev.read(randomKey.data(), 16);

			QByteArray tmp;
			tmp.insert(0, realInitialValue);
			QByteArray realCipher = encryptAES(randomKey, realKey, type, tmp);
			if ((quint8)(Tools::big_intFromByteArray<quint64>(realCipher) % 256) != realInputData)
			{
				continue;
			}

			tmp.clear();
			tmp.insert(0, fakeInitialValue);
			QByteArray fakeCipher = encryptAES(randomKey, fakeKey, type, tmp);
			if ((quint8)(Tools::big_intFromByteArray<quint64>(fakeCipher) % 256) != fakeInputData)
			{
				continue;
			}
			realInitialValue = realCipher;
			fakeInitialValue = fakeCipher;
			outputDevice->write(randomKey);
			break;
		} while (true);
	}
	dev.close();
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
	if (type != ECB && mInitialValue.count() != blockSize())
	{
		endDecrypt(deviceNumber);
		qDebug() << "initial value not valid";
		return false;
	}
	AESKeyCipher *key = qobject_cast<AESKeyCipher *>(keyFromNumber(keyNumber));
	QIODevice *device = deviceFromNumber(deviceNumber);
	QByteArray cipherData(16, 0x00);
	QByteArray initialValue;
	initialValue.insert(0, mInitialValue);
	while (!device->atEnd())
	{
		device->read(cipherData.data(), 16);
		initialValue = encryptAES(cipherData, key, type, initialValue);
		quint8 data = static_cast<quint8>(Tools::big_intFromByteArray<quint64>(initialValue) % 256);
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
	if (type != ECB && mInitialValue.count() != blockSize())
	{
		endEncrypt();
		qDebug() << "initial value not valid";
		return false;
	}
	AESKeyCipher *realKey = qobject_cast<AESKeyCipher *>(pRealKey);
	AESKeyCipher *fakeKey = qobject_cast<AESKeyCipher *>(pFakeKey);
	intermediateRealKey = mSecondFakeKey->value() * inverse_mod(mSecondFakeKey->value(), mSecondRealKey->value());
	intermediateFakeKey = mSecondRealKey->value() * inverse_mod(mSecondRealKey->value(), mSecondFakeKey->value());
	big_int modulo = mSecondRealKey->value() * mSecondFakeKey->value();
	QByteArray realInitialValue;
	realInitialValue.insert(0, mInitialValue);
	QByteArray fakeInitialValue;
	fakeInitialValue.insert(0, mInitialValue);
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
		realInitialValue = encryptAES(realInputData, realKey, type, realInitialValue);
		fakeInitialValue = encryptAES(fakeInputData, fakeKey, type, fakeInitialValue);
		big_int C1 = Tools::big_intFromByteArray<quint64>(realInitialValue);
		big_int C2 = Tools::big_intFromByteArray<quint64>(fakeInitialValue);
		big_int C = mod(C1 * intermediateRealKey + C2 * intermediateFakeKey, modulo);
		outputDevice->write(Tools::big_intToByteArray(C).rightJustified(33, 0x00));
	}
	endEncrypt();
	return true;
}
bool AESCryptographerChinese::decrypt(Cryptographer::Device deviceNumber, Cryptographer::Key keyNumber, const QVariant &parameters)
{
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
	if (type != ECB && mInitialValue.count() != blockSize())
	{
		endDecrypt(deviceNumber);
		qDebug() << "initial value not valid";
		return false;
	}
	AESKeyCipher *key = qobject_cast<AESKeyCipher *>(keyFromNumber(keyNumber));
	QIODevice *device = deviceFromNumber(deviceNumber);
	QByteArray cipherData(33, 0x00);
	QByteArray initialValue;
	initialValue.insert(0, mInitialValue);
	while (!device->atEnd())
	{
		device->read(cipherData.data(), 33);
		big_int C = mod(Tools::big_intFromByteArray<quint64>(cipherData), secondKey->value());
		QByteArray cipher = Tools::big_intToByteArray(C);
		cipher = cipher.rightJustified(16, 0x00);
		outputDevice->write(decryptAES(cipher, key, type, initialValue));
		initialValue = cipher;
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
	srand(time(0));
	big_int secondRealKey = big_int::random_with_length(128 + 1);
	big_int secondFakeKey;
	do
	{
		secondFakeKey = big_int::random_with_length(128 + 1);
	}
	while(euclid(secondRealKey, secondFakeKey) != 1);
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

quint8 *AESKeyCipher::value() const
{
	return (quint8*)mData.data();
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


QWidget *AESCryptographer::widget()
{
	QWidget *widget = new QWidget();
	QFormLayout *mainLayout = new QFormLayout(widget);
	QComboBox *encryptionType = new QComboBox(widget);
	encryptionType->addItem("Random");
	encryptionType->addItem("ChineseTheoreme");
	QComboBox *encryptionMode = new QComboBox(widget);
	encryptionMode->addItem("ECB", ECB);
	encryptionMode->addItem("CBC", CBC);
	QComboBox *keyType = new QComboBox(widget);
	keyType->addItem("AES_128", AES_128);
	keyType->addItem("AES_192", AES_192);
	keyType->addItem("AES_256", AES_256);
	mainLayout->addRow("Encryption Type", encryptionType);
	mainLayout->addRow("Encryption Mode", encryptionMode);
	mainLayout->addRow("Key Type", keyType);
}

QString AESCryptographer::type()
{
	return "AES";
}
