#ifndef CHECKEDCOMBOBOX_H
#define CHECKEDCOMBOBOX_H

#include <QComboBox>

class QStandardItem;

class CheckedComboBox : public QComboBox
{
	Q_OBJECT
public:
	CheckedComboBox(QMap<int, QString> items, QWidget *parent = 0);

	QMap<int, QString> checkedItems();
	void setCheckedItems(QList<int> checkedItems);

signals:
	void dataChanged(QList<int> userData);

private slots:
	void onDataChanged();
private:
	QStandardItem *visibleItem();
	void updateVisibleItem(const QMap<int, QString>  &text);

	// QWidget interface
protected:
	virtual void paintEvent(QPaintEvent *);
};

#endif // CHECKEDCOMBOBOX_H
