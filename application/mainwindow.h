#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QComboBox;
class KeyWidget;
class DataSourceWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	enum EncryptionType
	{
		Hash,
		BlockAsHash,
		BlockChineseRemainder
	};

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	QComboBox *encryptionTypeComboBox;
	QComboBox *algorithmComboBox;

	/// Only blocks algorithms
	QComboBox *keySizeComboBox;
	QComboBox *encryptionModeComboBox;
	/// ----------------------

	KeyWidget *realKey;
	KeyWidget *fakeKey;

	DataSourceWidget *realDataSource;
	DataSourceWidget *fakeDataSource;

};

#endif // MAINWINDOW_H
