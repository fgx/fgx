#ifndef EXECONTROLS_H
#define EXECONTROLS_H

#include <QtCore/QString>
#include <QtCore/QStringList>

#include <QtGui/QWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <QtGui/QStatusBar>
#include <QProcess>

class ExeControls : public QGroupBox
{
Q_OBJECT
public:
	explicit ExeControls(QString title, QString exeNam, QWidget *parent = 0);
	
	QProcess *P;


	QString exe_name;
	QStatusBar *statusBar;
	QPushButton *buttonStart;
	QPushButton *buttonStop;
	//QPushButton *buttonRefresh;
        QStringList user_env;
        QString runtime;
	//** Start is called from fgx
	void start(QString command_line);
	int get_pid();
	void kill_pid();


signals:

public slots:
	//* on_start() is connected in fgx, stop too, but state of buttons changed here
	void on_stop_clicked();

	void readError();
	void readOutput();
};

#endif // EXECONTROLS_H
