

#include "xprocess.h"

XProcess::XProcess(MainObject *mob, QObject *parent) :
    QObject(parent)
{

	mainObject = mob;

	process = new QProcess(this);
	process->setProcessChannelMode(QProcess::MergedChannels);
	process->setReadChannel(QProcess::StandardOutput);

	connect(process, SIGNAL(stateChanged(QProcess::ProcessState)),
			this, SLOT(fgfs_handle_process_state_change(QProcess::ProcessState)));
	connect(process, SIGNAL(readyRead()),
			this, SLOT(fgfs_handle_process_output()));
	connect(process, SIGNAL(finished(int, QProcess::ExitStatus)),
			this, SLOT(fgfs_handle_process_finished(int, QProcess::ExitStatus)));
	connect(process, SIGNAL(error(QProcess::ProcessError)),
			this, SLOT(fgfs_handle_process_error(QProcess::ProcessError)));
}

//===============================================================================
//== fgProcess
void XProcess::start(QString command_line)
{
	viewLogsWidget->clear_fgfs_log();
	process->start(command_line);
	process->closeWriteChannel();
}

void XProcess::fgfs_stop_request()
{
	process->kill();
	process->waitForFinished();
	//#terrasync_stop_request();
}



void XProcess::fgfs_handle_process_state_change(QProcess::ProcessState state)
{
	switch (state) {
		case QProcess::Starting:
			emit fgfs_running(true);
			break;
		case QProcess::Running:
			emit fgfs_running(true);
			break;
		case QProcess::NotRunning:
			emit fgfs_running(false);
			break;
	}
}

void XProcess::fgfs_handle_process_output()
{
	QString line = fgProcess->readLine();
	while (!line.isEmpty()) {
		viewLogsWidget->add_fgfs_log(line);
		line = fgProcess->readLine();
	}
}

void XProcess::fgfs_handle_process_finished(int code, QProcess::ExitStatus status)
{
	QString message = QString("process exited with code: %1, status: %2\n")
		.arg(code)
		.arg(status == QProcess::NormalExit ? "QProcess::NormalExit" : "QProcess::CrashExit");
	viewLogsWidget->add_fgfs_log(message);
}

void XProcess::fgfs_handle_process_error(QProcess::ProcessError error)
{
	QString message("$ error: ");

	switch (error) {
		case QProcess::FailedToStart:
			message += "fgfs failed to start; (incorrect path?)";
			break;
		case QProcess::ReadError:
			message += "there was an error reading from the process";
			break;
		case QProcess::UnknownError:
			message += "unknown error";
			break;
		default:
			return;
	}
	viewLogsWidget->add_fgfs_log(message);
}
