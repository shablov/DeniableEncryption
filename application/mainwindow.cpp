#include "mainwindow.h"

#include <common/dict.h>
#include <gui/guitools.h>

#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	Dict::setFileName("application.dict");

	QWidget *widget = new QWidget;
	setCentralWidget(widget);
	QHBoxLayout *mainLayout = new QHBoxLayout(widget);
}

MainWindow::~MainWindow()
{

}
