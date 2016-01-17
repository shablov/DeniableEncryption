#ifndef KEYWIDGET_H
#define KEYWIDGET_H

#include <QWidget>

class QCheckBox;
class QLineEdit;
class FileSelectWidget;

class KeyWidget : public QWidget
{
	Q_OBJECT
public:
	explicit KeyWidget(QWidget *parent = 0);
	QByteArray data() const;

private:
	QCheckBox *fromFileCheckBox;
	QCheckBox *manualInputCheckBox;
	QLineEdit *manualInput;
	FileSelectWidget *fileInput;
};

#endif // KEYWIDGET_H
