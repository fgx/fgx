#ifndef XPROCESS_H
#define XPROCESS_H

#include <QObject>
#include <QProcess>
#include <QStringList>

#include "xobjects/mainobject.h"

class XProcess : public QObject
{
Q_OBJECT
public:
	explicit XProcess(MainObject *mainObject, QString log_type_ki, QObject *parent = 0);

	MainObject *mainObject;
	QProcess *process;
	QString log_name;

	int get_pid();

signals:
	void running(bool);

public slots:

	void start(QString command_line, QStringList user_env);
	void stop();

	void on_process_state_change(QProcess::ProcessState state);
	void on_process_output();
	void on_process_finished(int code, QProcess::ExitStatus status);
	void on_process_error(QProcess::ProcessError error);

};

#endif // XPROCESS_H
