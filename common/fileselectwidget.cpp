#include "fileselectwidget.h"

#include <QFile>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>

#include <dict.h>

FileSelectWidget::FileSelectWidget(const QString &filename, bool highlightWrongFile, QWidget *parent)
	: QWidget(parent), mHighlightWrongFile(highlightWrongFile), mMode(QFileDialog::ExistingFile), mRelMode(false)
{
	pFilename = new QLineEdit;
	pFilename->setPlaceholderText(Dict::tr("filename"));
	connect(pFilename, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));

	QPushButton *selectFile = new QPushButton(QIcon(":open_file"), Dict::tr("browse"));
	selectFile->setToolTip(Dict::tr("browse"));
	connect(selectFile, SIGNAL(clicked()), this, SLOT(chooseFile()));

	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(pFilename);
	mainLayout->setSpacing(0);
	mainLayout->addWidget(selectFile);
	setFilename(filename);
}

QString FileSelectWidget::filename()
{
	return pFilename->text();
}

bool FileSelectWidget::isCorrectFilename()
{
	return QFile::exists(pFilename->text());
}

void FileSelectWidget::setFileMode(const QFileDialog::FileMode &mode)
{
	mMode = mode;
}

void FileSelectWidget::setRelativePathMode(bool relMode)
{
	mRelMode = relMode;

	checkRelPath();
}

void FileSelectWidget::setFilename(const QString &filename)
{
	pFilename->setText(filename);

	checkRelPath();
}

void FileSelectWidget::checkRelPath()
{
	QString filename = pFilename->text();
	if(filename.isEmpty())
	{
		return;
	}

	if(mRelMode && QDir::isAbsolutePath(filename))
	{
		filename = QDir::current().relativeFilePath(filename);
		pFilename->setText(filename);
		emit filenameChanged(filename);
	}
}

void FileSelectWidget::onTextChanged(const QString &filename)
{
	switch(mMode)
	{
	case QFileDialog::AnyFile:
		pFilename->setStyleSheet("background-color:white");
		emit filenameChanged(filename);
		break;
	case QFileDialog::Directory:
		if (!QDir(filename).exists() && mHighlightWrongFile)
		{
			pFilename->setStyleSheet("background-color:#ff5050");
		}
		else
		{
			pFilename->setStyleSheet("background-color:white");
			emit filenameChanged(filename);
		}
		break;
	case QFileDialog::ExistingFile:
	case QFileDialog::ExistingFiles:
	default:
		if (!QFile::exists(filename) && mHighlightWrongFile)
		{
			pFilename->setStyleSheet("background-color:#ff5050");
		}
		else
		{
			pFilename->setStyleSheet("background-color:white");
			emit filenameChanged(filename);
		}
		break;
	}
}

void FileSelectWidget::chooseFile()
{
	QString filename;

	switch(mMode)
	{
	case QFileDialog::AnyFile:
		filename = QFileDialog::getSaveFileName(this);
		break;
	case QFileDialog::Directory:
		filename = QFileDialog::getExistingDirectory(this);
		break;
	case QFileDialog::ExistingFiles:
	case QFileDialog::ExistingFile:
	default:
		filename = QFileDialog::getOpenFileName(this);
		break;
	}

	if (!filename.isEmpty())
	{
		setFilename(filename);
	}
}
