#ifndef TOOLS_H
#define TOOLS_H

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wreorder"
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"

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

#endif // TOOLS_H

