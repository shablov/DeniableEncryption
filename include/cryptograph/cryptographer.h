#ifndef CRYPTOGRAPHER_H
#define CRYPTOGRAPHER_H

#include <QObject>
#include <QVariant>

class QIODevice;
class KeyCipher;

class Cryptographer : public QObject
{
	Q_OBJECT
public:
	explicit Cryptographer(QObject *parent = 0);

	enum Device
	{
		RealDevice = 0x00,
		FakeDevice = 0x01,
		OutputDevice = 0x02
	};

	void setDevice(QIODevice *d, Cryptographer::Device deviceNumber);
	void setDevice(QByteArray &ba, Cryptographer::Device deviceNumber);

	enum Key
	{
		RealKey = 0x00,
		FakeKey = 0x01
	};

	void setKey(QIODevice *d, Cryptographer::Key keyNumber);
	void setKey(const QByteArray &ba, Cryptographer::Key keyNumber);

public:
	virtual bool encrypt(const QVariant &parameters = QVariant());
	virtual bool decrypt(Cryptographer::Device deviceNumber, Cryptographer::Key keyNumber, const QVariant &parameters);
	virtual QWidget *widget() = 0;
	virtual QString type() = 0;

protected:
	void endEncrypt();
	void endDecrypt(Cryptographer::Device deviceNumber);

	QIODevice *deviceFromNumber(Cryptographer::Device deviceNumber);
	QIODevice *realInputDevice;
	QIODevice *fakeInputDevice;
	QIODevice *outputDevice;

	KeyCipher *keyFromNumber(Cryptographer::Key keyNumber);
	KeyCipher *pRealKey;
	KeyCipher *pFakeKey;
};

class KeyCipher : public QObject
{
	Q_OBJECT
public:
	KeyCipher(QObject *parent = 0);

	virtual bool load(const QByteArray &ba);
	virtual QByteArray data() const = 0;

	bool isValid() const;

protected:
	bool valid;
};

#endif // CRYPTOGRAPHER_H
