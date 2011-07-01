#ifndef XPROCESS_H
#define XPROCESS_H

#include <QObject>
#include <QProcess>

#include "xobjects/mainobject.h"

class XProcess : public QObject
{
Q_OBJECT
public:
	explicit XProcess(MainObject *mainObject, QObject *parent = 0);

	MainObject *mainObject;
	QProcess *process;

signals:

public slots:

	void start(QString command_line);
	void stop();
	void on_process_state_change(QProcess::ProcessState state);
	void on_handle_process_output();
	void on_handle_process_finished(int code, QProcess::ExitStatus status);
	void on_handle_process_error(QProcess::ProcessError error);

};

#endif // XPROCESS_H
