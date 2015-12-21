#ifndef TOOLS_H
#define TOOLS_H

#include <QDataStream>

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wcomment"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wreorder"
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC diagnostic ignored "-Wcomment"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"

#include <big_int.hpp>
#include <prime.hpp>
using namespace Arageli;

#pragma GCC diagnostic pop


class Tools
{
public:
	template <class T>
	static big_int big_intFromByteArray(const QByteArray &ba)
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
			result <<= (size * 8);
			result += data;
		}
		return result;
	}

	static QByteArray big_intToByteArray(const big_int &number)
	{
		QByteArray result = QByteArray();
		unsigned char byte = 0x00;
		for (int i = number.length() - 1; i >=0 ; i--)
		{
			byte <<= 1;
			byte += (number.bit(i) ? 1: 0);
			if ((i) % 8 == 0)
			{
				result.append(byte);
				byte = 0x00;
			}
		}

		return result;
	}
};


#endif // TOOLS_H

