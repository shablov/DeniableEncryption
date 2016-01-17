#ifndef DATASOURCEWIDGET_H
#define DATASOURCEWIDGET_H

#include <QWidget>

class QCheckBox;
class FileSelectWidget;
class QComboBox;

class DataSourceWidget : public QWidget
{
	Q_OBJECT
public:
	explicit DataSourceWidget(QWidget *parent = 0);

	QIODevice *device();

private:
	QCheckBox *fromFileCheckBox;
	QCheckBox *generatorCheckBox;
	FileSelectWidget *fileInput;
	QComboBox *generatorInput;
};

#endif // DATASOURCEWIDGET_H
