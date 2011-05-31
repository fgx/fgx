
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QtCore/QProcess>
#include <QtCore/QTimer>

#include <QtGui/QHBoxLayout>

#include "execontrols.h"

/* The idea of this is to have a widget that both launches a "service"..
   But also can check and kill it, on linux or mac.. Windows got no idea
   WANTED: expert advice
*/

ExeControls::ExeControls(QString title, QString exeCmd, QWidget *parent) :
    QGroupBox(parent)
{
	exe_name = exeCmd;
	setTitle(title);

	//** Grid layout just in case..
	QVBoxLayout *layout = new QVBoxLayout();
	setLayout(layout);
	layout->setContentsMargins(5,5,5,5);
	layout->setSpacing(10);

	QString buttStyle("padding: 8px;"); //* make button smaller. this need to go into global style sheet said pedro

	//** Buttons layout
	QHBoxLayout *buttlay = new QHBoxLayout();
	layout->addLayout(buttlay);

	//** Stop Button
	buttonStop = new QPushButton();
	buttonStop->setText(tr(" Stop "));
	buttonStop->setIcon(QIcon(":/icon/stop_disabled"));
	buttonStop->setStyleSheet(buttStyle);
	buttlay->addWidget(buttonStop);
	connect(buttonStop, SIGNAL(clicked()), this, SLOT(on_stop_clicked()));

	//** Start Button
	buttonStart = new QPushButton();
	buttonStart->setText(tr(" Start "));
	buttonStart->setIcon(QIcon(":/icon/start_enabled"));
	buttonStart->setStyleSheet(buttStyle);
	buttlay->addWidget(buttonStart);
	//* connection is done in fgx not here..



	//** Bottom layout
	QHBoxLayout *bottlay = new QHBoxLayout();
	layout->addLayout(bottlay);

	//* Status Bar
	statusBar = new QStatusBar();
	statusBar->setSizeGripEnabled(false);
	statusBar->setContentsMargins(5,5,5,5);
	bottlay->addWidget(statusBar);

}


//==========================================================================
// Start Executable
// Writing recent PIDs to /tmp/exe.pid
//==========================================================================
void ExeControls::start(QString command_line){
	int start = QProcess::startDetached(command_line);
	Q_UNUSED(start);
	if(start){
		statusBar->showMessage("Starting", 2000);
		QTimer::singleShot(2000,this, SLOT(on_refresh_clicked()));
		
		if (command_line.contains("terrasync")) {
			QString whichExe = "terrasync";
			QString OSXGetPid = "ps axc|awk '{if ($5==\"";
			OSXGetPid.append(whichExe);
			OSXGetPid.append("\") print $1}' > /tmp/terrasync.pid");
			system(OSXGetPid.toLatin1());
		}
		
		if (command_line.contains("fgcom")) {
			QString whichExe = "fgcom";
			QString OSXGetPid = "ps axc|awk '{if ($5==\"";
			OSXGetPid.append(whichExe);
			OSXGetPid.append("\") print $1}' > /tmp/fgcom.pid");
			system(OSXGetPid.toLatin1());
		}
		
		if (command_line.contains("fgfs")) {
			QString whichExe = "fgfs";
			QString OSXGetPid = "ps axc|awk '{if ($5==\"";
			OSXGetPid.append(whichExe);
			OSXGetPid.append("\") print $1}' > /tmp/fgfs.pid");
			system(OSXGetPid.toLatin1());
		}

			
	}else{
		statusBar->showMessage("Failed", 4000);
	}
}



//==========================================================================
// Stop Executable
// Get recent PIDs from /tmp/exe.pid and kill
//==========================================================================
void ExeControls::on_stop_clicked(){
	
	QString pid;
	
	QFile pidFile;
	if (ExeControls::exe_name.contains("terrasync")) {
	pidFile.setFileName("/tmp/terrasync.pid");
	}
	if (ExeControls::exe_name.contains("fgfs")) {
		pidFile.setFileName("/tmp/fgfs.pid");
	}
	if (ExeControls::exe_name.contains("fgcom")) {
		pidFile.setFileName("/tmp/fgcom.pid");
	}
	
	
		if (!pidFile.open(QIODevice::ReadOnly | QIODevice::Text))
			return;
	
		while (!pidFile.atEnd()) {
		
			QString line;
			line.append(pidFile.readAll());
			line.remove("\n");
			pid.append(line);	
		}
	
		if(pid > ""){
			QStringList killargs;
			killargs << "-9" << pid;
			QProcess::startDetached("kill", killargs);
		
			statusBar->showMessage("Killed Process", 2000);
		}else{
			statusBar->showMessage("Not found", 2000);
		}
}

//** Refresh Button clicked
void ExeControls::on_refresh_clicked(){
	//update_pid();
}


