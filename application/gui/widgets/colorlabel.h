#ifndef COLORLABEL_H
#define COLORLABEL_H

#include <QLabel>
#include <QMap>
#include <QVariant>

class ColorLabel : public QLabel
{
	Q_OBJECT
public:
	explicit ColorLabel(QString text = QString(), QWidget *parent = 0);
	void setColor(QColor color);
	void setColor(QString colorName);

	void setData(QVariant data, Qt::ItemDataRole role = Qt::UserRole);
	QVariant data(Qt::ItemDataRole role = Qt::UserRole);
signals:

private:
	QMap<Qt::ItemDataRole, QVariant> mData;

};

#endif // COLORLABEL_H
