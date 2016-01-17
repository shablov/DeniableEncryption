#ifndef DICT_H
#define DICT_H

#include <QString>
#include <QMap>

class Dict
{
public:
	static QString tr(const QString &engName, const QString &contextName = QString("default"));
	static QString trEnum(const QString &engName, const QString &enumEngName, const QString &contextName = QString("default"));
	static QString trEnum(const int &enumValue, const QString &enumEngName, const QString &contextName = QString("default"));
	static QString trEnumName(const QString &engName, const QString &contextName = QString("default"));
	static int enumValue(const QString &engName, const QString &enumEngName, const QString &contextName = QString("default"));

	/*!
	 * \brief Возвращает английское название элемента перечисления.
	 * \param enumValue Значение перечисления.
	 * \param enumEngName Английское название перечисления.
	 * \param contextName Название контекста.
	 */
	static QString enumName(const int &enumValue, const QString &enumEngName, const QString &contextName = QString("default"));

	/*!
	 * \brief Возвращает английские названия значений перечисления
	 * \param enumEngName Название перечисления.
	 * \param contextName Название контекста. По умолчанию - default.
	 */
	static QStringList enumNames(const QString &enumEngName, const QString &contextName = QString("default"));
	static QMap<QString, QString> enumMap(const QString &enumEngName, const QString &contextName = QString("default"));
	static QMap<int, QString> enumValueMap(const QString &enumEngName, const QString &contextName = QString("default"));

	static void setFileName(QString fileName);
	static bool isValid();
};

#endif // DICT_H
