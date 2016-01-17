#include "gui/widgets/ledwidget.h"

#include <QLabel>
#include <QLayout>

LedWidget::LedWidget(QString text, QString iconName, QWidget *parent) :
	QWidget(parent)
{
	mText = text;
	mIconName = iconName;

	pIconLbl = new QLabel();
	pIconLbl->setPixmap( QPixmap( mIconName ));
	pIconLbl->setMaximumSize(16, 16);
	pIconLbl->setScaledContents( true );

	pTextLbl = new QLabel( mText );

	QHBoxLayout *hlay = new QHBoxLayout(this);
	hlay->addWidget( pIconLbl );
	hlay->addWidget( pTextLbl );
}

void LedWidget::setText(QString text)
{
	mText = text;
	pTextLbl->setText( mText );
}

void LedWidget::setIcon(QString iconName)
{
	mIconName = iconName;
	pIconLbl->setPixmap( QPixmap( mIconName ));
}
