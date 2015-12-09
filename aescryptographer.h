#ifndef AESCRYPTOGRAPHER_H
#define AESCRYPTOGRAPHER_H

#include "cryptographer.h"

#define bool
#include "intel_aes_lib/include/iaesni.h"
#undef bool

class AESCryptographer : public Cryptographer
{
	Q_OBJECT
public:
	AESCryptographer(QObject *parent = 0);

	// Cryptographer interface
public:
	virtual bool encrypt(const QVariant &parameters = QVariant());
	virtual bool decrypt(Cryptographer::Device deviceNumber, Cryptographer::Key keyNumber,
						 const QVariant &parameters = QVariant());
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
