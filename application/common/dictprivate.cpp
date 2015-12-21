#include "dictprivate.h"

#include <QFile>

#include <QDebug>

DictPrivate *DictPrivate::pInstance = 0;

DictPrivate *DictPrivate::instance()
{
	if (pInstance == 0)
	{
		pInstance = new DictPrivate();
	}

	return pInstance;
}

DictPrivate::DictPrivate() : fileName("../dat/dict.xml")
{
	reload();
}

void DictPrivate::reload()
{
	clearContent();
	load();

	if ( !mDoc.isNull() )
	{
		check();
	}
}

void DictPrivate::clearContent()
{
	mDoc.clear();
	mRoot.clear();
}

void DictPrivate::load()
{
	QString errorStr;
	int errorLine = 0;
	int errorColumn = 0;
	QByteArray dictByteArray = dictByteArrayFromFile();
	if (!(mDoc.setContent(dictByteArray, true, &errorStr, &errorLine, &errorColumn)))
	{
		clearContent();
		QString error = "В QDomDocument не установлен контент с ошибкой = " + errorStr + " в строке = " +
				QString::number(errorLine) + " в колонке = "  + QString::number(errorColumn);
		qDebug() << error;
		return;
	}

	mRoot = mDoc.documentElement();
	if (mRoot.tagName() != "dict")
	{
		clearContent();
		qDebug() << "Корневой тэг xml документа неправильный.";
		return;
	}
}

void DictPrivate::check()
{
	if (duplicated())
	{
		mDoc.clear();
		mRoot.clear();
		qDebug() << "Xml документ неправильный - есть дублирующиеся тэги.";
	}
}

QByteArray DictPrivate::dictByteArrayFromFile()
{
	QFile file(fileName);
	if (!file.exists())
	{
		qDebug() << "Файл словаря не существует.";
		return QByteArray();
	}
	if (!file.open(QIODevice::ReadOnly))
	{

		qDebug() << "Файл словаря не может быть открыт. Ошибка = " + file.errorString();
		return QByteArray();
	}
	QByteArray fileContent = file.readAll();
	file.close();
	return fileContent;
}

QString DictPrivate::tr(const QString &engName, const QString &contextName)
{
	DictPrivate *dict = DictPrivate::instance();
	QDomElement stringElement = dict->getStringElement(engName, contextName);
	return ruNameFromElement(stringElement, engName);
}

QString DictPrivate::trEnum(const QString &engName, const QString &enumEngName, const QString &contextName)
{
	DictPrivate *dict = DictPrivate::instance();
	QDomElement argElement = dict->getEnumArg(engName, enumEngName, contextName);
	return ruNameFromElement(argElement, engName);
}

QString DictPrivate::trEnum(const int &enumValue, const QString &enumEngName, const QString &contextName)
{
	DictPrivate *dict = DictPrivate::instance();
	QDomElement argElement = dict->getEnumArg(enumValue, enumEngName, contextName);
	return ruNameFromElement(argElement, QString::number(enumValue));
}

QString DictPrivate::trEnumName(const QString &engName, const QString &contextName)
{
	DictPrivate *dict = DictPrivate::instance();
	QDomElement enumElement = dict->getEnumElement(engName, contextName);
	return ruNameFromElement(enumElement, engName);
}

int DictPrivate::enumValue(const QString &engName, const QString &enumEngName, const QString &contextName)
{
	DictPrivate *dict = DictPrivate::instance();
	QDomElement argElement = dict->getEnumArg(engName, enumEngName, contextName);
	return valueFromElement(argElement, engName);
}

QString DictPrivate::enumName(const int &enumValue, const QString &enumEngName, const QString &contextName)
{
	DictPrivate *dict = DictPrivate::instance();
	QDomElement argElement = dict->getEnumArg(enumValue, enumEngName, contextName);
	return engNameFromElement(argElement);
}

QStringList DictPrivate::enumNames(const QString &enumEngName, const QString &contextName)
{
	DictPrivate *dict = DictPrivate::instance();
	QDomElement enumElement = dict->getEnumElement(enumEngName, contextName);
	return engNamesFromElement(enumElement);
}

QMap<QString, QString> DictPrivate::enumMap(const QString &enumEngName, const QString &contextName)
{
	DictPrivate *dict = DictPrivate::instance();
	QDomElement enumElement = dict->getEnumElement(enumEngName, contextName);
	return nameMapFromElement(enumElement);
}

QMap<int, QString> DictPrivate::enumValueMap(const QString &enumEngName, const QString &contextName)
{
	DictPrivate *dict = DictPrivate::instance();
	QDomElement enumElement = dict->getEnumElement(enumEngName, contextName);
	return valueMapFromElement(enumElement);
}

void DictPrivate::setFileName(QString _fileName)
{
	DictPrivate *dict = DictPrivate::instance();
	if (dict->fileName == _fileName)
	{
		return;
	}

	dict->fileName = _fileName;
	dict->reload();
}

bool DictPrivate::isValid()
{
	DictPrivate *dict = DictPrivate::instance();
	return (!(dict->mDoc.isNull()));
}

QDomElement DictPrivate::getStringElement(const QString &engName, const QString &contextName)
{
	QDomElement contextElement = getContextElement(contextName);
	QDomElement stringElement = getStringElement(engName, contextElement);
	return stringElement;
}

QDomElement DictPrivate::getContextElement(const QString &engName)
{
	QDomNodeList contextElements = mRoot.elementsByTagName("context");
	return elementByEngName(engName, contextElements);
}

QDomElement DictPrivate::getStringElement(const QString &engName, const QDomElement &contextElement)
{
	QDomNodeList stringElements = contextElement.elementsByTagName("string");
	QDomElement stringElement = elementByEngName(engName, stringElements);
	return stringElement;
}

QDomElement DictPrivate::getEnumElement(const QString &engName, const QString &contextName)
{
	QDomElement contextElement = getContextElement(contextName);
	QDomElement enumElement = getEnumElement(engName, contextElement);
	return enumElement;
}

QDomElement DictPrivate::getEnumElement(const QString &engName, const QDomElement &contextElement)
{
	QDomNodeList enumElements = contextElement.elementsByTagName("enum");
	QDomElement enumElement = elementByEngName(engName, enumElements);
	return enumElement;
}

QDomElement DictPrivate::getEnumArg(const QString &engName, const QString &enumEngName, const QString &contextName)
{
	QDomElement enumElement = getEnumElement(enumEngName, contextName);
	QDomElement argElement = getEnumArg(engName, enumElement);
	return argElement;
}

QDomElement DictPrivate::getEnumArg(const int &value, const QString &enumEngName, const QString &contextName)
{
	QDomElement enumElement = getEnumElement(enumEngName, contextName);
	QDomElement argElement = getEnumArg(value, enumElement);
	return argElement;
}

QDomElement DictPrivate::getEnumArg(const QString &engName, const QDomElement &enumElement)
{
	QDomNodeList argElements = enumElement.elementsByTagName("arg");
	QDomElement argElement = elementByEngName(engName, argElements);
	return argElement;
}

QDomElement DictPrivate::getEnumArg(const int &enumValue, const QDomElement &enumElement)
{
	QDomNodeList argElements = enumElement.elementsByTagName("arg");
	QDomElement argElement = elementByValue(enumValue, argElements);
	return argElement;
}

QDomElement DictPrivate::elementByEngName(const QString &engName, const QDomNodeList &nodeList)
{
	for (int i = 0; i < nodeList.size(); i++)
	{
		QDomElement element = nodeList.at(i).toElement();
		if (element.attribute("name_eng") == engName)
		{
			return element;
		}
	}
	return QDomElement();
}

QDomElement DictPrivate::elementByValue(const int &value, const QDomNodeList &nodeList)
{
	for (int i = 0; i < nodeList.size(); i++)
	{
		QDomElement element = nodeList.at(i).toElement();
		if (element.attribute("value") == QString::number(value))
		{
			return element;
		}
	}
	return QDomElement();
}

QString DictPrivate::ruNameFromElement(const QDomElement &element, const QString defValue)
{
	return element.attribute("name_rus", defValue);
}

QString DictPrivate::engNameFromElement(const QDomElement &element, const int &defValue)
{
	return element.attribute("name_eng", QString::number(defValue));
}

QStringList DictPrivate::engNamesFromElement(const QDomElement &element, const QStringList defValue)
{
	QStringList result;
	QDomNodeList childs = element.childNodes();
	for (int i = 0; i < childs.count(); i++)
	{
		QDomElement child = childs.at(i).toElement();
		result << engNameFromElement(child);
	}
	return result.isEmpty() ? defValue : result;
}

QMap<QString, QString> DictPrivate::nameMapFromElement(const QDomElement &element, const QMap<QString, QString> defValue)
{
	QMap<QString, QString> result;
	QDomNodeList childs = element.childNodes();
	for (int i = 0; i < childs.count(); i++)
	{
		QDomElement child = childs.at(i).toElement();
		result[engNameFromElement(child)] = ruNameFromElement(child);
	}
	return result.isEmpty() ? defValue : result;
}

QMap<int, QString> DictPrivate::valueMapFromElement(const QDomElement &element, const QMap<int, QString> defValue)
{
	QMap<int, QString> result;
	QDomNodeList childs = element.childNodes();
	for (int i = 0; i < childs.count(); i++)
	{
		QDomElement child = childs.at(i).toElement();
		result[valueFromElement(child)] = ruNameFromElement(child);
	}
	return result.isEmpty() ? defValue : result;
}

int DictPrivate::valueFromElement(const QDomElement &element, const QString &defValue)
{
	return static_cast<int>(element.attribute("value", defValue).toInt());
}

bool DictPrivate::duplicated()
{
	return duplicatedContexts();
}

bool DictPrivate::duplicatedContexts()
{
	QDomNodeList contextElements = mRoot.elementsByTagName("context");
	if (!duplicatedNames(contextElements))
	{
		return duplicatedDataIntoContexts(contextElements);
	}
	return true;
}

bool DictPrivate::duplicatedDataIntoContexts(const QDomNodeList &contextElements)
{
	for (int i = 0; i < contextElements.size(); i++)
	{
		QDomElement contextElement = contextElements.at(i).toElement();
		if (duplicatedDataIntoContext(contextElement))
		{
			return true;
		}
	}
	return false;
}

bool DictPrivate::duplicatedDataIntoContext(const QDomElement &contextElement)
{
	return (duplicatedEnums(contextElement) || (duplicatedStrings(contextElement)));
}

bool DictPrivate::duplicatedStrings(const QDomElement &contextElement)
{
	QDomNodeList stringElements = contextElement.elementsByTagName("string");
	return duplicatedNames(stringElements);
}

bool DictPrivate::duplicatedEnums(const QDomElement &contextElement)
{
	QDomNodeList enumElements = contextElement.elementsByTagName("enum");
	if (!duplicatedNames(enumElements))
	{
		return duplicatedDataIntoEnums(enumElements);
	}
	return true;
}

bool DictPrivate::duplicatedDataIntoEnums(const QDomNodeList &enumElements)
{
	for (int i = 0; i < enumElements.size(); i++)
	{
		QDomElement enumElement = enumElements.at(i).toElement();
		if (duplicatedEnum(enumElement))
		{
			return true;
		}
	}
	return false;
}

bool DictPrivate::duplicatedEnum(const QDomElement &enumElement)
{
	if (!duplicatedValuesArguments(enumElement))
	{
		return duplicatedNamesArguments(enumElement);
	}
	return false;
}

bool DictPrivate::duplicatedValuesArguments(const QDomElement &enumElement)
{
	QDomNodeList argElements = enumElement.elementsByTagName("arg");
	return duplicatedValues(argElements);
}

bool DictPrivate::duplicatedNamesArguments(const QDomElement &enumElement)
{
	QDomNodeList argElements = enumElement.elementsByTagName("arg");
	return duplicatedNames(argElements);
}

bool DictPrivate::duplicatedValues(const QDomNodeList &elementList)
{
	QList<int> elementsValues;

	for (int i = 0; i < elementList.size(); i++)
	{
		QDomElement element = elementList.at(i).toElement();
		int value = valueFromElement(element);
		if (elementsValues.contains(value))
		{
			writeDuplicatePlaceToSyslog(element);
			return true;
		}
		elementsValues.append(value);
	}
	return false;
}

bool DictPrivate::duplicatedNames(const QDomNodeList &elementList)
{
	QList<QString> elementsNames;

	for (int i = 0; i < elementList.size(); i++)
	{
		QDomElement element = elementList.at(i).toElement();
		QString name = engNameFromElement(element);
		name = name.simplified();
		if (elementsNames.contains(name))
		{
			writeDuplicatePlaceToSyslog(element);
			return true;
		}
		elementsNames.append(name);
	}
	return false;
}

void DictPrivate::writeDuplicatePlaceToSyslog(const QDomElement &element)
{
	qDebug() << "Дублируйщий код в строке = " +
				QString::number(element.lineNumber()) + " в колонке = " +
				QString::number(element.columnNumber());
}
