#ifndef DICTPRIVATE_H
#define DICTPRIVATE_H

#include <QDomDocument>
#include <QStringList>
#include <QMap>

class DictPrivate
{
public:

	static QString tr(const QString &engName, const QString &contextName = QString("default"));
	static QString trEnum(const QString &engName, const QString &enumEngName, const QString &contextName = QString("default"));
	static QString trEnum(const int &enumValue, const QString &enumEngName, const QString &contextName = QString("default"));
	static QString trEnumName(const QString &engName, const QString &contextName = QString("default"));
	static int enumValue(const QString &engName, const QString &enumEngName, const QString &contextName = QString("default"));
	static QString enumName(const int &enumValue, const QString &enumEngName, const QString &contextName = QString("default"));
	static QStringList enumNames(const QString &enumEngName, const QString &contextName = QString("default"));
	static QMap<QString, QString> enumMap(const QString &enumEngName, const QString &contextName = QString("default"));
	static QMap<int, QString> enumValueMap(const QString &enumEngName, const QString &contextName = QString("default"));

	static void setFileName(QString _fileName);
	static bool isValid();

private:
	static DictPrivate *instance();
	DictPrivate();
	void reload();
	void clearContent();
	void load();
	void check();
	QByteArray dictByteArrayFromFile();
	QDomElement getContextElement(const QString &engName);
	QDomElement getStringElement(const QString &engName, const QString &contextName);
	QDomElement getStringElement(const QString &engName, const QDomElement &contextElement);
	QDomElement getEnumElement(const QString &engName, const QString &contextName);
	QDomElement getEnumElement(const QString &engName, const QDomElement &contextElement);
	QDomElement getEnumArg(const QString &engName, const QString &enumEngName, const QString &contextName);
	QDomElement getEnumArg(const int &value, const QString &enumEngName, const QString &contextName);
	QDomElement getEnumArg(const QString& engName, const QDomElement &enumElement);
	QDomElement getEnumArg(const int& enumValue, const QDomElement &enumElement);

	QDomElement elementByEngName(const QString &engName, const QDomNodeList &nodeList);
	QDomElement elementByValue(const int &value, const QDomNodeList &nodeList);

	static QString ruNameFromElement(const QDomElement &element, const QString defValue = QString());
	static QString engNameFromElement(const QDomElement &element, const int &defValue = -1);
	static QStringList engNamesFromElement(const QDomElement &element,const QStringList defValue = QStringList());
	static QMap<QString, QString> nameMapFromElement(const QDomElement &element,
													  const QMap<QString, QString> defValue = QMap<QString, QString>());
	static QMap<int, QString> valueMapFromElement(const QDomElement &element,
													  const QMap<int, QString> defValue = QMap<int, QString>());
	static int valueFromElement(const QDomElement &element, const QString &defValue = QString());

	bool duplicated();
	bool duplicatedContexts();
	bool duplicatedDataIntoContexts(const QDomNodeList &contextElements);
	bool duplicatedDataIntoContext(const QDomElement &contextElement);
	bool duplicatedStrings(const QDomElement &contextElement);
	bool duplicatedEnums(const QDomElement &contextElement);
	bool duplicatedEnum(const QDomElement &enumElement);
	bool duplicatedDataIntoEnums(const QDomNodeList &enumElements);
	bool duplicatedArguments(const QDomElement &enumElement);
	bool duplicatedValuesArguments(const QDomElement &enumElement);
	bool duplicatedNamesArguments(const QDomElement &enumElement);
	bool duplicatedValues(const QDomNodeList &elementList);
	bool duplicatedNames(const QDomNodeList &elementList);

	void writeDuplicatePlaceToSyslog(const QDomElement &element);
	static DictPrivate *pInstance;
	QString fileName;
	QDomDocument mDoc;
	QDomElement mRoot;
};

#endif // DICTPRIVATE_H
