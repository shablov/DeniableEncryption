#ifndef GUITOOLS_H
#define GUITOOLS_H

class QWidget;
class QHBoxLayout;
class QDialog;
class QComboBox;
class QRegExpValidator;
class CheckedComboBox;
class QMenu;

#include <QString>
#include <QColor>
#include <QFileInfoList>

class GuiTools
{
public:
	enum Colors { Red, Green, Blue, Gray, LightGreen, Yellow };

	static void information(QWidget *parent, const QString &title,
			const QString &text, const QString &detailedText=QString());
	static void warning(QWidget *parent, const QString &title,
			const QString &text, const QString &detailedText=QString());
	static bool question(QWidget *parent, const QString &title,
			const QString &text, const QString &detailedText=QString());

	static QHBoxLayout *dialogButtonLayout(QDialog *parent);

	static QComboBox * comboBoxForEnum(const QString &enumName, const QString &context = "default");
	static CheckedComboBox *checkedComboBoxForEnum(const QString &enumName, const QString &context = "default");

	static QWidget *separator(Qt::Orientation orientation, const int &size = 0);

	static QRegExpValidator* latitudeValidator();
	static QRegExpValidator* longitudeValidator();

	static QColor color(const GuiTools::Colors &colorName);

	static QFileInfoList directoryFiles(const QString &dir, const QStringList &fileTypes);

	static QWidget *fileSelectWidget(const QString &filename);

	static QMenu *menuForEnum(const QString &enumName, const QString &context = "default");

};

#endif // GUITOOLS_H
