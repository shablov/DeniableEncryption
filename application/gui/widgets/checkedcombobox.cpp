#include "gui/widgets/checkedcombobox.h"
#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QStylePainter>

#include <QDebug>
#include <QListView>
CheckedComboBox::CheckedComboBox(QMap<int, QString> items, QWidget *parent)
	: QComboBox(parent)
{
	QStandardItemModel *model = new QStandardItemModel(this);
	setModel(model);
	view()->setTextElideMode(Qt::ElideRight);
	QMapIterator<int, QString> i(items);
	while (i.hasNext())
	{
		i.next();
		QStandardItem *item = new QStandardItem( i.value());
		item->setData(i.key(), Qt::UserRole);
		item->setData(Qt::Checked, Qt::CheckStateRole);
		item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
		model->appendRow(item);
	}

	model->insertRow(0, new QStandardItem());
	setCurrentIndex(0);
	updateVisibleItem(items);
	connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(onDataChanged()));
	setView(new QListView);
}

QMap<int, QString> CheckedComboBox::checkedItems()
{
	QMap<int, QString> data;
	QStandardItemModel *standardModel = qobject_cast<QStandardItemModel*>(model());
	for (int row = 1; row < standardModel->rowCount(); row++)
	{
		QStandardItem *	item = standardModel->item(row);
		Q_ASSERT(item);
		if (item->data(Qt::CheckStateRole).toInt() == Qt::Checked)
		{
			data.insert(item->data(Qt::UserRole).toInt(), item->text());
		}
	}
	return data;
}

void CheckedComboBox::setCheckedItems(QList<int> checkedItems)
{
	QStandardItemModel *standardModel = qobject_cast<QStandardItemModel*>(model());
	for (int row = 1; row < standardModel->rowCount(); row++)
	{
		QStandardItem *	item = standardModel->item(row);
		Q_ASSERT(item);
		int id = item->data(Qt::UserRole).toInt();
		item->setData(checkedItems.contains(id) ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
	}
}

void CheckedComboBox::onDataChanged()
{
	QMap<int, QString> data = checkedItems();
	updateVisibleItem(data);
	emit dataChanged(data.keys());
}

QStandardItem *CheckedComboBox::visibleItem()
{
	QStandardItemModel *standardModel = qobject_cast<QStandardItemModel*>(model());
	Q_ASSERT_X(standardModel != 0, Q_FUNC_INFO,  "no model");
	return standardModel ? standardModel->item(0) : 0;
}

void CheckedComboBox::updateVisibleItem(const QMap<int, QString> &data)
{
	QStandardItem *item = visibleItem();
	Q_ASSERT_X(item != 0, Q_FUNC_INFO,  "no item");
	if (item)
	{
		item->setText(QStringList(data.values()).join("; "));
	}
}

void CheckedComboBox::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	QStylePainter painter(this);
	painter.setPen(palette().color(QPalette::Text));

	QStyleOptionComboBox opt;
	initStyleOption(&opt);
	opt.currentText = opt.fontMetrics.elidedText(opt.currentText, Qt::ElideRight, maximumWidth() - 20);

	painter.drawComplexControl(QStyle::CC_ComboBox, opt);
	painter.drawControl(QStyle::CE_ComboBoxLabel, opt);
}
