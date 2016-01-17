#include "mainwindow.h"

#include <common/dict.h>
#include <gui/guitools.h>

#include <QComboBox>
#include <QCryptographicHash>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setMinimumSize(600, 400);
	Dict::setFileName("application.dict");

	QTabWidget *widget = new QTabWidget(this);
	setCentralWidget(widget);

	widget->addTab(new QWidget(), Dict::tr("type_and_keys"));
	widget->addTab(new QWidget(), Dict::tr("information"));
}

MainWindow::~MainWindow()
{

}
