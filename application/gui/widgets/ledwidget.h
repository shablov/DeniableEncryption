#ifndef LEDWIDGET_H
#define LEDWIDGET_H

#include <QWidget>
#include <QVariant>

class QLabel;

class LedWidget : public QWidget
{
	Q_OBJECT
public:
	/**
	 * @brief LedWidget
	 * @param text
	 * @param iconName
	 * @param parent
	 */
	explicit LedWidget(QString text, QString iconName = ":question", QWidget *parent = 0);

	/**
	 * @brief setText
	 * @param text
	 */
	void setText(QString text);

	/**
	 * @brief text
	 * @return
	 */
	QString text() { return mText; }

	/**
	 * @brief setIcon
	 * @param iconName
	 */
	void setIcon(QString iconName);

	/**
	 * @brief icon
	 * @return
	 */
	QString icon() { return mIconName; }

	/**
	 * @brief setData - Функция устанавливает дополнительные данные объекта
	 * @param data - данные
	 */
	void setData( QVariant data ) { mData = data; }

	/**
	 * @brief data Фкнция получает дополнительные данные объекта
	 * @return
	 */
	QVariant data() { return mData; }

signals:

public slots:

private:
	QString mIconName;		/// Название иконки
	QString mText;			/// Текст
	QLabel *pIconLbl;		/// Указатель на лейбл для иконки
	QLabel *pTextLbl;		/// Указатель на лейбл для текста
	QVariant mData;			/// Дополнительные данные объекта
};

#endif // LEDWIDGET_H
