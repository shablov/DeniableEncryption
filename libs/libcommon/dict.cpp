#include "dict.h"
#include "dictprivate.h"

QString Dict::tr(const QString &engName, const QString &contextName)
{
	return DictPrivate::tr(engName, contextName);
}

QString Dict::trEnum(const QString &engName, const QString &enumEngName, const QString &contextName)
{
	return DictPrivate::trEnum(engName, enumEngName, contextName);
}

QString Dict::trEnum(const int &enumValue, const QString &enumEngName, const QString &contextName)
{
	return DictPrivate::trEnum(enumValue, enumEngName, contextName);
}

QString Dict::trEnumName(const QString &engName, const QString &contextName)
{
	return DictPrivate::trEnumName(engName, contextName);
}

int Dict::enumValue(const QString &engName, const QString &enumEngName, const QString &contextName)
{
	return DictPrivate::enumValue(engName, enumEngName, contextName);
}

QString Dict::enumName(const int &enumValue, const QString &enumEngName, const QString &contextName)
{
	return DictPrivate::enumName(enumValue, enumEngName, contextName);
}

QStringList Dict::enumNames(const QString &enumEngName, const QString &contextName)
{
	return DictPrivate::enumNames(enumEngName, contextName);
}

QMap<QString, QString> Dict::enumMap(const QString &enumEngName, const QString &contextName)
{
	return DictPrivate::enumMap(enumEngName, contextName);
}

QMap<int, QString> Dict::enumValueMap(const QString &enumEngName, const QString &contextName)
{
	return DictPrivate::enumValueMap(enumEngName, contextName);
}

void Dict::setFileName(QString fileName)
{
	return DictPrivate::setFileName(fileName);
}

bool Dict::isValid()
{
	return DictPrivate::isValid();
}
