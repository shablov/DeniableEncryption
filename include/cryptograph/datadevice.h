#ifndef DATADEVICE_H
#define DATADEVICE_H

#include <QIODevice>

enum InputDevice
{
	RandomDevice,
	NullDevice,
	UnitDevice,
	SeriesDevice
};

class DataDeviceFactory
{
public:
	static QIODevice *device(InputDevice deviceNumber);
};

class RandomDataDevice : public QIODevice
{
public:
	RandomDataDevice(QObject *parent = 0);

	// QIODevice interface
	virtual bool atEnd() const;
protected:
	virtual qint64 readData(char *data, qint64 maxlen);
	virtual qint64 writeData(const char *data, qint64 len);
};

class NullDataDevice : public QIODevice
{
public:
	NullDataDevice(QObject *parent = 0);

	// QIODevice interface
	bool atEnd() const;
protected:
	virtual qint64 readData(char *data, qint64 maxlen);
	virtual qint64 writeData(const char *data, qint64 len);
};

class UnitDataDevice : public QIODevice
{
public:
	UnitDataDevice(QObject *parent = 0);

	// QIODevice interface
	bool atEnd() const;
protected:
	virtual qint64 readData(char *data, qint64 maxlen);
	virtual qint64 writeData(const char *data, qint64 len);
};

class SeriesDataDevice : public QIODevice
{
public:
	SeriesDataDevice(QObject *parent = 0);

	// QIODevice interface
	bool atEnd() const;
protected:
	virtual qint64 readData(char *data, qint64 maxlen);
	virtual qint64 writeData(const char *data, qint64 len);
};

class StdUIntOutputDevice : public QIODevice
{
public:
	StdUIntOutputDevice(QObject *parent = 0);

	// QIODevice interface
protected:
	virtual qint64 readData(char *data, qint64 maxlen);
	virtual qint64 writeData(const char *data, qint64 len);

private:
	QByteArray mData;
};

#endif // DATADEVICE_H
