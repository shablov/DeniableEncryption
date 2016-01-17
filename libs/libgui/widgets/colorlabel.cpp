#include "gui/widgets/colorlabel.h"

ColorLabel::ColorLabel(QString text, QWidget *parent) :
	QLabel(text, parent)
{
	setFrameShadow( QFrame::Plain);
	setFrameShape( QFrame::StyledPanel );
	setContentsMargins(2,2,2,2);
	setAlignment(Qt::AlignCenter);
}

void ColorLabel::setColor(QColor color)
{
	setStyleSheet( "background-color: "+color.name() );
}

void ColorLabel::setColor(QString colorName)
{
	setStyleSheet( "background-color: "+colorName );
}

void ColorLabel::setData(QVariant data, Qt::ItemDataRole role)
{
	mData[role] = data;
}

QVariant ColorLabel::data(Qt::ItemDataRole role)
{
	return mData[role];
}
