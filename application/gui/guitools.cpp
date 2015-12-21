#include "guitools.h"
#include "gui/widgets/checkedcombobox.h"
#include "gui/widgets/fileselectwidget.h"

#include "common/dict.h"

#include <QApplication>
#include <QScopedPointer>
#include <QSharedPointer>

#include <QMessageBox>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QAbstractItemView>
#include <QDir>

#include <QAbstractItemView>

#include <QMenu>

#include <QDebug>

void GuiTools::information(QWidget *parent, const QString &title, const QString &text,
						const QString &detailedText)
{
#if QT_VERSION >= 0x040600
	QScopedPointer<QMessageBox> messageBox(new QMessageBox(parent));
#else
	QSharedPointer<QMessageBox> messageBox(new QMessageBox(parent));
#endif
	if (parent)
	{
		messageBox->setWindowModality(Qt::WindowModal);
	}
	messageBox->setWindowTitle(Dict::tr(title));
	messageBox->setText(Dict::tr(text));
	if (!detailedText.isEmpty())
	{
		messageBox->setInformativeText(Dict::tr(detailedText));
	}
	messageBox->setIcon(QMessageBox::Information);
	messageBox->addButton(QMessageBox::Ok);
	messageBox->exec();
}

void GuiTools::warning(QWidget *parent, const QString &title, const QString &text,
					const QString &detailedText)
{
#if QT_VERSION >= 0x040600
	QScopedPointer<QMessageBox> messageBox(new QMessageBox(parent));
#else
	QSharedPointer<QMessageBox> messageBox(new QMessageBox(parent));
#endif
	if (parent)
	{
		messageBox->setWindowModality(Qt::WindowModal);
	}
	messageBox->setWindowTitle(Dict::tr(title));
	messageBox->setText(Dict::tr(text));
	if (!detailedText.isEmpty())
	{
		messageBox->setInformativeText(Dict::tr(detailedText));
	}
	messageBox->setIcon(QMessageBox::Warning);
	messageBox->addButton(QMessageBox::Ok);
	messageBox->exec();
}

bool GuiTools::question(QWidget *parent, const QString &title, const QString &text,
					 const QString &detailedText)
{
#if QT_VERSION >= 0x040600
	QScopedPointer<QMessageBox> messageBox(new QMessageBox(parent));
#else
	QSharedPointer<QMessageBox> messageBox(new QMessageBox(parent));
#endif
	if (parent)
	{
		messageBox->setWindowModality(Qt::WindowModal);
	}
	messageBox->setWindowTitle(Dict::tr(title));
	messageBox->setText(Dict::tr(text));
	if (!detailedText.isEmpty())
	{
		messageBox->setInformativeText(Dict::tr(detailedText));
	}
	messageBox->setIcon(QMessageBox::Question);
	QAbstractButton *yesButton = messageBox->addButton(QMessageBox::Yes);

	messageBox->addButton(QMessageBox::No);
	messageBox->setDefaultButton(qobject_cast<QPushButton*>(yesButton));
	messageBox->exec();
	return messageBox->clickedButton() == yesButton;
}

QHBoxLayout *GuiTools::dialogButtonLayout(QDialog *parent)
{
	QPushButton *okButton = new QPushButton(QIcon(":accept"), Dict::tr("accept"));
	okButton->setObjectName("accept_button");
	parent->connect(okButton, SIGNAL(clicked()), parent, SLOT(accept()));
	QPushButton *cancelButton = new QPushButton(QIcon(":cancel"), Dict::tr("cancel"));
	cancelButton->setObjectName("cancel_button");
	parent->connect(cancelButton, SIGNAL(clicked()), parent, SLOT(reject()));
	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch();
	buttonLayout->addWidget(okButton);
	buttonLayout->addWidget(cancelButton);
	return buttonLayout;
}

QComboBox *GuiTools::comboBoxForEnum(const QString &enumName, const QString &context)
{
	QComboBox *cb = new QComboBox;
	QMap<int, QString> valueMap = Dict::enumValueMap(enumName, context);

	QMapIterator<int, QString> i(valueMap);
	while (i.hasNext())
	{
		i.next();
		cb->addItem(i.value(), i.key());
	}

	cb->view()->setTextElideMode(Qt::ElideRight);

	return cb;
}

CheckedComboBox *GuiTools::checkedComboBoxForEnum(const QString &enumName, const QString &context)
{
	return new CheckedComboBox(Dict::enumValueMap(enumName, context));
}

QWidget *GuiTools::separator(Qt::Orientation orientation, const int &size)
{
	QFrame *line = new QFrame();
	line->setFrameShape(orientation == Qt::Horizontal ? QFrame::HLine : QFrame::VLine);
	line->setFrameShadow(QFrame::Sunken);
	if (size != 0)
	{
		if (orientation == Qt::Horizontal)
		{
			line->setFixedHeight(size);
		}
		else
		{
			line->setFixedWidth(size);
		}
	}
	return line;
}

QRegExpValidator *GuiTools::latitudeValidator()
{
	return new QRegExpValidator( QRegExp("[NnSs][0-9]{1,2}(\\.[0-9]{1,2}){2}"));
}

QRegExpValidator *GuiTools::longitudeValidator()
{
	return new QRegExpValidator( QRegExp("[EeWw][0-9]{1,3}(\\.[0-9]{1,2}){2}"));
}

QColor GuiTools::color(const GuiTools::Colors &colorName)
{
	switch(colorName)
	{
		case GuiTools::Red:			return QColor(255, 80, 80);
		case GuiTools::Green:		return QColor(80, 255, 80);
		case GuiTools::Blue:		return QColor(150, 210, 255);
		case GuiTools::Yellow:		return QColor(255, 255, 80);
		case GuiTools::LightGreen:	return QColor(180, 255, 180);
		case GuiTools::Gray:		return QColor(210, 210, 210);
		default: break;
	}

	return QColor(Qt::transparent);
}

QFileInfoList GuiTools::directoryFiles(const QString &dir, const QStringList &fileTypes)
{
	QFileInfoList result;

	QDir d(dir+"/");
	d.setNameFilters( fileTypes );
	foreach(QFileInfo info, d.entryInfoList( QDir::Files))
	{
		result << info;
	}

	return result;
}

QWidget *GuiTools::fileSelectWidget(const QString &filename)
{
	return new FileSelectWidget(filename);
}

QMenu *GuiTools::menuForEnum(const QString &enumName, const QString &context)
{
	QMenu *menu = new QMenu(Dict::trEnumName(enumName, context));
	QActionGroup *enumGroup = new QActionGroup(menu);
	enumGroup->setExclusive(true);
	QMap<int, QString> enumValueMap = Dict::enumValueMap(enumName, context);
	QMapIterator<int, QString> i(enumValueMap);
	while (i.hasNext())
	{
		i.next();
		const QString &rusName = i.value();
		const int &enumValue = i.key();
		QAction *action = menu->addAction(rusName);
		action->setParent(menu);
		action->setData(enumValue);
		action->setCheckable(true);
		action->setChecked(false);
		enumGroup->addAction( action );
	}
	return menu;
}
