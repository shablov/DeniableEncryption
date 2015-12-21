#include "datasourcewidget.h"
#include "fileselectwidget.h"

#include <common/dict.h>

#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>

#include <gui/guitools.h>

#include <datadevice.h>

DataSourceWidget::DataSourceWidget(QWidget *parent) : QWidget(parent)
{
	fromFileCheckBox = new QCheckBox(Dict::tr("from_file"), this);
	fileInput = new FileSelectWidget;
	fileInput->setParent(this);

	generatorCheckBox = new QCheckBox(Dict::tr("from_generator"), this);
	generatorInput = GuiTools::comboBoxForEnum("GeneratorType");

	QButtonGroup *buttonGroup = new QButtonGroup(this);
	buttonGroup->addButton(fromFileCheckBox);
	buttonGroup->addButton(generatorCheckBox);
	buttonGroup->setExclusive(true);
	fromFileCheckBox->setChecked(true);

	QFormLayout *mainLayout = new QFormLayout(this);
	mainLayout->addRow(fromFileCheckBox, fileInput);
	mainLayout->addRow(generatorCheckBox, generatorInput);
}

QIODevice *DataSourceWidget::device()
{
	if (fromFileCheckBox->isChecked())
	{
		if (QFile::exists(fileInput->filename()))
		{
			return new QFile(fileInput->filename());
		}
	}
	else if (generatorCheckBox->isChecked())
	{
		InputDevice number = static_cast<InputDevice>(generatorInput->currentData(Qt::UserRole).toUInt());
		return DataDeviceFactory::device(number);
	}
	return 0;
}

