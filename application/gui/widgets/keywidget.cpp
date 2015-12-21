#include "gui/widgets/keywidget.h"

#include "fileselectwidget.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>

#include <common/dict.h>

KeyWidget::KeyWidget(QWidget *parent) : QWidget(parent)
{
	fromFileCheckBox = new QCheckBox(Dict::tr("from_file"), this);
	fileInput = new FileSelectWidget;
	fileInput->setParent(this);
	manualInputCheckBox = new QCheckBox(Dict::tr("from_manual"), this);
	manualInput = new QLineEdit(this);

	QButtonGroup *buttonGroup = new QButtonGroup(this);
	buttonGroup->addButton(fromFileCheckBox);
	buttonGroup->addButton(manualInputCheckBox);
	buttonGroup->setExclusive(true);
	fromFileCheckBox->setChecked(true);

	QFormLayout *mainLayout = new QFormLayout(this);
	mainLayout->addRow(fromFileCheckBox, fileInput);
	mainLayout->addRow(manualInputCheckBox, manualInput);
}

QByteArray KeyWidget::data() const
{
	if (fromFileCheckBox->isChecked())
	{
		QFile file(fileInput->filename());
		if (file.exists() && file.open(QIODevice::ReadOnly))
		{
			return file.readAll();
		}
	}
	else if (manualInputCheckBox->isChecked())
	{
		return QByteArray::fromHex(manualInput->text().toUtf8());
	}
	return QByteArray();
}

