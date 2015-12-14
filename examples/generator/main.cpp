#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QProcess>

int main(int argc, char *argv[])
{
	Q_UNUSED(argc);
	Q_UNUSED(argv);

	QProcess proc;
	QFile file("random_simple.bin");
	proc.setStandardOutputFile("random_simple.bin");
	proc.start("cat", QStringList() << "/dev/urandom");
//	file.open(QIODevice::ReadOnly);
	while (file.size() < 16)
	{
//		qDebug() << file.size();
	}
	proc.close();

	return 0;
}
