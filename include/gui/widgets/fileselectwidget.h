#ifndef FILSELECTWIDGET_H
#define FILSELECTWIDGET_H

#include <QWidget>
#include <QFileDialog>

class QLineEdit;

class FileSelectWidget : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QString filename READ filename WRITE setFilename NOTIFY filenameChanged USER true)
public:
	FileSelectWidget(const QString &filename = QString(), bool highlightWrongFile = true, QWidget *parent = 0);

	QString filename();
	bool isCorrectFilename();

	/*!
	 * \brief Устанавливает режим выбора - файла, директория etc.
	 * По умолчанию выбираются существующие файлы.
	 */
	void setFileMode(const QFileDialog::FileMode &mode);

	/**
	 * @brief Устанавливает режим работы с путями.
	 * @param relMode - режим работы с путями. true - абсолютные пути, false - относительные
	 */
	void setRelativePathMode(bool relMode = false);

public slots:
	void setFilename(const QString &filename);

signals:
	void filenameChanged(const QString &filename);

private slots:
	void onTextChanged(const QString &filename);
	void chooseFile();

private:
	/*!
	 * \brief Проверяет, соответствует ли текущий путь выбранному режиму.
	 * Если выбран режим относительных путей, то абсолютный путь будет приведен к относительному виду.
	 */
	void checkRelPath();

	QLineEdit *pFilename;
	bool mHighlightWrongFile;

	QFileDialog::FileMode mMode;
	bool mRelMode;

};

#endif // FILSELECTWIDGET_H
