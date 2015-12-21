#ifndef AESCRYPTOGRAPHER_H
#define AESCRYPTOGRAPHER_H

#include "cryptographer.h"

#include <tools.h>

class AESKeyCipher;
class AESChineseKeyCiper;

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
	static int blockSize();
	void generateInitialValue();
	void setInitialValue(const QByteArray &initial);
	QByteArray initialValue() const;

	// Cryptographer interface
public:
	virtual bool encrypt(const QVariant &parameters = QVariant());
	virtual bool decrypt(Cryptographer::Device deviceNumber, Cryptographer::Key keyNumber,
						 const QVariant &parameters = QVariant());

protected:
	QByteArray encryptAES(const QByteArray &inputData, AESKeyCipher *key,
						  EncryptionType type, const QByteArray &initial = QByteArray());
	QByteArray decryptAES(const QByteArray &data, AESKeyCipher *key,
						  EncryptionType type, const QByteArray &initial = QByteArray());

	QByteArray mInitialValue;
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

	KeyCipher *secondRealKey();
	KeyCipher *secondFakeKey();

	void generateSecondKeys();
	void setSecondRealKey(const QByteArray &ba);
	void setSecondFakeKey(const QByteArray &ba);

private:
	AESChineseKeyCiper *mSecondRealKey;
	AESChineseKeyCiper *mSecondFakeKey;

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
	QByteArray data() const;
	quint8 *value() const;
	int keySize() const;

private:
	QByteArray mData;
};

class AESChineseKeyCiper : public KeyCipher
{
	Q_OBJECT
public:
	AESChineseKeyCiper(QObject *parent = 0);

	// KeyCipher interface
public:
	virtual bool load(const QByteArray &ba);
	QByteArray data() const;
	big_int value() const;
	int keySize() const;

private:
	big_int mData;
};
#endif // AESCRYPTOGRAPHER_H
