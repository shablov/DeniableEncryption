#include "mainwindow.h"
#include <QApplication>
#include <QCryptographicHash>
#include <QDebug>

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wreorder"
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"

#include <QElapsedTimer>
#include <big_int.hpp>

#pragma GCC diagnostic pop

using namespace Arageli;

template <class T>
big_int big_intFromByteArray(const QByteArray &ba)
{
	size_t size = sizeof(T);
	if (ba.size() % size != 0)
	{
		switch (size)
		{
			case 8: return big_intFromByteArray<quint32>(ba);
			case 4: return big_intFromByteArray<quint16>(ba);
			case 2: return big_intFromByteArray<quint8>(ba);
			default: break;
		}
	}
	static int s = 0;
	if (s != ba.size())
	{
		s = ba.size();
	}
	big_int result = 0;
	T data;
	QDataStream out(ba);
	while (!out.atEnd())
	{
		out >> data;
		result <<= size;
		result += data;
	}
	return result;
}

QByteArray decrypt(const QByteArray &cryptogram, const QByteArray &key, QCryptographicHash::Algorithm algorithm)
{
	QByteArray result;

	int j = 0;
	QByteArray exKey;
	QDataStream in(&exKey, QIODevice::WriteOnly);
	for (int i = 0; i < cryptogram.count(); i += 4)
	{
		in.device()->seek(0);
		in.writeRawData(key.constData(), key.count());
		in.writeRawData(cryptogram.mid(i, 4), 4);
		in << j;
		big_int hash = big_intFromByteArray<quint64>(QCryptographicHash::hash(exKey, algorithm));
		result.append((unsigned char)((hash % quint8(-1))));
		j++;
	}
	return result;
}

void completeData(QByteArray &data1, QByteArray &data2)
{
	int diffSize = data1.count() - data2.count();
	QByteArray exInputText(abs(diffSize), 0x00);
	if (diffSize < 0)
	{
		data1.append(exInputText);
	}
	else if (diffSize > 0)
	{
		data2.append(exInputText);
	}
}

int main(int argc, char *argv[])
{
	Q_UNUSED(argc);
	Q_UNUSED(argv);

	QByteArray inputText1 = "input 1 text 1";
	QByteArray inputText2 = "input 2 text 2";
	QByteArray key1 = "k1";
	QByteArray key2 = "k2";
	completeData(inputText1, inputText2);

	while (true)
	{
		for (QCryptographicHash::Algorithm algorithm = QCryptographicHash::Md4; algorithm <= QCryptographicHash::Sha3_512;
				 algorithm  = static_cast<QCryptographicHash::Algorithm>(algorithm + 1))
		{
			qsrand(time(0));
			QElapsedTimer timer;
			timer.start();
			QByteArray cryptogram;
			for (int i = 0; i < inputText1.count(); i++)
			{
				quint32 randomNumber = 0;
				QByteArray randomKey;
				QDataStream in(&randomKey, QIODevice::WriteOnly);
				do
				{
					in.device()->seek(0);
					randomNumber = qrand();
					in << randomNumber;
					in << i;

					big_int hash1 = big_intFromByteArray<quint64>(QCryptographicHash::hash(key1 + randomKey, algorithm));
					if ((unsigned char)((hash1 % quint8(-1))) != static_cast<unsigned char>(inputText1.at(i)))
					{
						continue;
					}

					big_int hash2 = big_intFromByteArray<quint64>(QCryptographicHash::hash(key2 + randomKey, algorithm));
					if ((unsigned char)((hash2 % quint8(-1))) != static_cast<unsigned char>(inputText2.at(i)))
					{
						continue;
					}
					cryptogram.append(randomKey.mid(0, 4));
					std::cout << randomNumber;
					break;
				} while (true);
			}
			if ((decrypt(cryptogram, key1, algorithm) == inputText1) &&
				(decrypt(cryptogram, key2, algorithm) == inputText2))
			{
				qDebug() << "success encryption with algorithm =" << algorithm << "and cryptogram =" << cryptogram.toHex()
						 << " and speed =" << (cryptogram.size() / 4.0) / timer.elapsed() * 1000 << "bytes/s";
			}
			else
			{
				qDebug() << "cryptogram incorrect with algorithm =" << algorithm << " and time =" << timer.elapsed();
			}
		}
	}
	return 0;
}
