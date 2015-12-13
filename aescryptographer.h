#ifndef AESCRYPTOGRAPHER_H
#define AESCRYPTOGRAPHER_H

#include "cryptographer.h"

#define bool
#include "intel_aes_lib/include/iaesni.h"
#undef bool

#include <tools.h>

class AESKeyCipher;

class AESCryptographer : public Cryptographer
{
	Q_OBJECT
public:
	enum AESKeySize
	{
		AES_128 = 16,
		AES_192 = 24,
		AES_256 = 32
	};

	enum EncryptionType
	{
		ECB = 0,
		CBC = 1,
		CTR = 2
	};

	AESCryptographer(QObject *parent = 0);

	// Cryptographer interface
public:
	virtual bool encrypt(const QVariant &parameters = QVariant());
	virtual bool decrypt(Cryptographer::Device deviceNumber, Cryptographer::Key keyNumber,
						 const QVariant &parameters = QVariant());

protected:
	QByteArray encryptAES(const QByteArray &data, AESKeyCipher *key, EncryptionType type);
	QByteArray decryptAES(const QByteArray &data, AESKeyCipher *key, EncryptionType type);
};

class AESCryptographerHash : public AESCryptographer
{
	Q_OBJECT
public:
	AESCryptographerHash(QObject *parent = 0);

	// Cryptographer interface
public:
	virtual bool encrypt(const QVariant &parameters = QVariant());
	virtual bool decrypt(Cryptographer::Device deviceNumber, Cryptographer::Key keyNumber,
						 const QVariant &parameters = QVariant());
};

class AESCryptographerChinese : public AESCryptographer
{
	Q_OBJECT
public:
	AESCryptographerChinese(QObject *parent = 0);

	// Cryptographer interface
public:
	virtual bool encrypt(const QVariant &parameters = QVariant());
	virtual bool decrypt(Cryptographer::Device deviceNumber, Cryptographer::Key keyNumber,
						 const QVariant &parameters = QVariant());

	QByteArray secondPartRealKey();
	QByteArray secondPartFakeKey();

	void setSecondPartRealKey(const QByteArray &ba);
	void setSecondPartFakeKey(const QByteArray &ba);

private:
	void generateSecondaryKeys();
	big_int mSecondPartRealKey;
	big_int mSecondPartFakeKey;

	big_int intermediateRealKey;
	big_int intermediateFakeKey;
};

class AESKeyCipher : public KeyCipher
{
	Q_OBJECT
public:
	AESKeyCipher(QObject *parent = 0);

	// KeyCipher interface
public:
	virtual bool load(const QByteArray &ba);
	UCHAR *data();
	int keySize();


	QByteArray mData;
};

#endif // AESCRYPTOGRAPHER_H
