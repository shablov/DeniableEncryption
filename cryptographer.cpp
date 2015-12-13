#include "cryptographer.h"

#include <QBuffer>
#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <QIODevice>

Cryptographer::Cryptographer(QObject *parent) : QObject(parent),
	realInputDevice(0), fakeInputDevice(0), outputDevice(0),
	pRealKey(0), pFakeKey(0)
{

}

void Cryptographer::setDevice(QIODevice *d, Device deviceNumber)
{
	QIODevice *device = deviceFromNumber(deviceNumber);
	if (device)
	{
		device->deleteLater();
		device = 0;
	}
	switch (deviceNumber)
	{
		case Cryptographer::RealDevice:
		{
			realInputDevice = d;
			break;
		}
		case Cryptographer::FakeDevice:
		{
			fakeInputDevice = d;
			break;
		}
		case Cryptographer::OutputDevice:
		{
			outputDevice = d;
			break;
		}
	}
	d->setParent(this);
}

void Cryptographer::setDevice(QByteArray &ba, Device deviceNumber)
{
	setDevice(new QBuffer(&ba, this), deviceNumber);
}

void Cryptographer::setKey(QIODevice *d, Cryptographer::Key keyNumber)
{
	if (!d->open(QIODevice::ReadOnly))
	{
		qDebug() << Q_FUNC_INFO << "device not opened";
	}
	setKey(d->readAll(), keyNumber);
}

void Cryptographer::setKey(const QByteArray &ba, Cryptographer::Key keyNumber)
{
	KeyCipher *key = keyFromNumber(keyNumber);
	Q_ASSERT(key);
	key->load(ba);
}

bool Cryptographer::encrypt(const QVariant &parameters)
{
	Q_UNUSED(parameters);
	if (!realInputDevice || !realInputDevice->open(QIODevice::ReadOnly) ||
		!fakeInputDevice || !fakeInputDevice->open(QIODevice::ReadOnly) ||
		!outputDevice || !outputDevice->open(QIODevice::WriteOnly) ||
		!pRealKey || !pRealKey->isValid() || !pFakeKey || !pFakeKey->isValid())
	{
		qDebug() << Q_FUNC_INFO << "cryptographer not complete for encryprion";
		return false;
	}
	return true;
}

bool Cryptographer::decrypt(Cryptographer::Device deviceNumber, Cryptographer::Key keyNumber, const QVariant &parameters)
{
	Q_UNUSED(parameters);
	QIODevice *device = deviceFromNumber(deviceNumber);
	KeyCipher *key = keyFromNumber(keyNumber);
	if (deviceNumber == OutputDevice || !device || !device->open(QIODevice::ReadOnly) ||
		!outputDevice || !outputDevice->open(QIODevice::WriteOnly) ||
		!key || !key->isValid())
	{
		return false;
	}
	return true;
}

QIODevice *Cryptographer::deviceFromNumber(Cryptographer::Device deviceNumber)
{
	switch (deviceNumber)
	{
		case Cryptographer::RealDevice: return realInputDevice;
		case Cryptographer::FakeDevice: return fakeInputDevice;
		case Cryptographer::OutputDevice: return outputDevice;
		default: return 0;
	}
}

KeyCipher *Cryptographer::keyFromNumber(Cryptographer::Key keyNumber)
{
	switch (keyNumber)
	{
		case Cryptographer::RealKey: return pRealKey;
		case Cryptographer::FakeKey: return pFakeKey;
		default: return 0;
	}
}

KeyCipher::KeyCipher(QObject *parent) :
	QObject(parent), valid(false)
{

}

bool KeyCipher::load(const QByteArray &ba)
{
	return valid = !ba.isEmpty();
}

bool KeyCipher::isValid() const
{
	return valid;
}
