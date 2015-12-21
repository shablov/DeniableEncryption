#include "datadevice.h"

#include <QDataStream>

#include <time.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <QDebug>

RandomDataDevice::RandomDataDevice(QObject *parent) : QIODevice(parent)
{
	qsrand(time(0));
}

bool RandomDataDevice::atEnd() const
{
	return false;
}

qint64 RandomDataDevice::readData(char *data, qint64 maxlen)
{
	std::ifstream urandom("/dev/urandom", std::ios::in|std::ios::binary);
	urandom.read(data, maxlen);
	urandom.close();
	return maxlen;
}

qint64 RandomDataDevice::writeData(const char *data, qint64 len)
{
	Q_UNUSED(data);
	Q_UNUSED(len);
	return -1;
}

NullDataDevice::NullDataDevice(QObject *parent) : QIODevice(parent)
{
}

bool NullDataDevice::atEnd() const
{
	return false;
}

qint64 NullDataDevice::readData(char *data, qint64 maxlen)
{
	for (int i = 0; i < maxlen; ++i)
	{
		*(data + i) = quint8(0x00);
	}
	return maxlen;
}

qint64 NullDataDevice::writeData(const char *data, qint64 len)
{
	Q_UNUSED(data);
	Q_UNUSED(len);
	return -1;
}

UnitDataDevice::UnitDataDevice(QObject *parent) : QIODevice(parent)
{
}

bool UnitDataDevice::atEnd() const
{
	return false;
}

qint64 UnitDataDevice::readData(char *data, qint64 maxlen)
{
	for (int i = 0; i < maxlen; ++i)
	{
		*(data + i) = quint8(0xFF);
	}
	return maxlen;
}

qint64 UnitDataDevice::writeData(const char *data, qint64 len)
{
	Q_UNUSED(data);
	Q_UNUSED(len);
	return -1;
}

SeriesDataDevice::SeriesDataDevice(QObject *parent) : QIODevice(parent)
{
}

bool SeriesDataDevice::atEnd() const
{
	return false;
}

qint64 SeriesDataDevice::readData(char *data, qint64 maxlen)
{
	for (int i = 0; i < maxlen; ++i)
	{
		*(data + i) = quint8(0xAA);
	}
	return maxlen;
}

qint64 SeriesDataDevice::writeData(const char *data, qint64 len)
{
	Q_UNUSED(data);
	Q_UNUSED(len);
	return -1;
}

StdUIntOutputDevice::StdUIntOutputDevice(QObject *parent) : QIODevice(parent)
{

}

qint64 StdUIntOutputDevice::readData(char *data, qint64 maxlen)
{
	Q_UNUSED(data);
	Q_UNUSED(maxlen);
	return -1;
}

qint64 StdUIntOutputDevice::writeData(const char *data, qint64 len)
{
	mData.append(data, len);
	QDataStream out(mData);
	char output[4];
	for (int i = 0; i < mData.size(); i += 4)
	{
		out.readRawData(&*output, 4);
		std::cout << output;
	}
	mData.remove(0, (mData.count() / 4) * 4);
	return len;
}

QIODevice *DataDeviceFactory::device(InputDevice deviceNumber)
{
	switch (deviceNumber)
	{
		case RandomDevice: return new RandomDataDevice;
		case NullDevice: return new NullDataDevice;
		case UnitDevice: return new UnitDataDevice;
		case SeriesDevice: return new SeriesDataDevice;
		default: return 0;
	}
}
