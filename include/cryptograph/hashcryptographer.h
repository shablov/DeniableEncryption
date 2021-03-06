#ifndef HASHCRYPTOGRAPHER_H
#define HASHCRYPTOGRAPHER_H

#include "cryptographer.h"

#include <QVariant>

class HashCryptographer : public Cryptographer
{
public:
	HashCryptographer(QObject *parent = 0);

	// Cryptographer interface
public:
	virtual bool encrypt(const QVariant &parameters);
	virtual bool decrypt(Cryptographer::Device deviceNumber, Cryptographer::Key keyNumber, const QVariant &parameters);
};

class HashKeyCipher : public KeyCipher
{
	Q_OBJECT
public:
	HashKeyCipher(QObject *parent = 0);

	virtual bool load(const QByteArray &ba);
	virtual QByteArray data() const;
	QByteArray value() const;

private:
	QByteArray mData;
};

#endif // HASHCRYPTOGRAPHER_H
