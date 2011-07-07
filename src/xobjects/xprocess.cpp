

#include "xprocess.h"
#ifdef Q_OS_WIN
#include <process.h>
#endif
#include "utilities/utilities.h"

XProcess::XProcess(MainObject *mob, QString logger_name, QObject *parent) :
    QObject(parent)
{

	mainObject = mob;
	log_name = logger_name;

	process = new QProcess(this);
	process->setProcessChannelMode(QProcess::MergedChannels);
	process->setReadChannel(QProcess::StandardOutput);

	connect(process, SIGNAL(stateChanged(QProcess::ProcessState)),
			this, SLOT(on_process_state_change(QProcess::ProcessState)));

	connect(process, SIGNAL(readyRead()),
			this, SLOT(on_process_output()));

	connect(process, SIGNAL(finished(int, QProcess::ExitStatus)),
			this, SLOT(on_process_finished(int, QProcess::ExitStatus)));

	connect(process, SIGNAL(error(QProcess::ProcessError)),
			this, SLOT(on_process_error(QProcess::ProcessError)));
}

//===============================================================================
//== Start
void XProcess::start(QString command_line, QStringList extra_env)
{
	mainObject->clear_log(log_name);

	if (extra_env.size()) {
		//= append new env vars
		QStringList env = QProcess::systemEnvironment();
		env << extra_env;
		process->setEnvironment(env);
	}
    outLog("CMD:"+log_name+": ["+command_line+"]");
	process->start(command_line);
	process->closeWriteChannel();
}

//===============================================================================
//== Stop
void XProcess::stop()
{
	process->kill();
	process->waitForFinished();
}



void XProcess::on_process_state_change(QProcess::ProcessState state)
{
	switch (state) {
		case QProcess::Starting:
			emit running(true);
			break;
		case QProcess::Running:
			emit running(true);
			break;
		case QProcess::NotRunning:
			emit running(false);
			break;
	}
}

void XProcess::on_process_output()
{
	QString line = process->readLine();
	while (!line.isEmpty()) {
		mainObject->add_log(log_name, line);
		line = process->readLine();
	}
}

void XProcess::on_process_finished(int code, QProcess::ExitStatus status)
{
	QString message = QString("process exited with code: %1, status: %2\n")
		.arg(code)
		.arg(status == QProcess::NormalExit ? "QProcess::NormalExit" : "QProcess::CrashExit");
	mainObject->add_log(log_name, message);
}

void XProcess::on_process_error(QProcess::ProcessError error)
{
	QString message("$ error: ");

	switch (error) {
		case QProcess::FailedToStart:
            message += "failed to start; (incorrect path?)\n";
			break;
        case QProcess::Crashed:
            message += "process crashed!\n";
            break;
        case QProcess::Timedout:
            message += "process timedout!\n";
            break;
        case QProcess::WriteError:
            message += "error writing to process\n";
            break;
        case QProcess::ReadError:
            message += "error reading from process\n";
			break;
		case QProcess::UnknownError:
            message += "unknown error\n";
			break;
		default:
            message += "Uncased error!\n";
            break;
	}
	mainObject->add_log(log_name, message);
}


int XProcess::get_pid() {
#ifdef Q_OS_WIN
    return _getpid();
#else
	return process->pid();
#endif
}
