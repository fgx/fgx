
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QtCore/QProcess>
#include <QtCore/QTimer>

#include <QtGui/QHBoxLayout>

#include "execontrols.h"
//#include "panes/advancedoptionswidget.h"
#include "launcher/launcherwindow.h"
#include "utilities/utilities.h"

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


	//** Buttons layout
	QVBoxLayout *buttlay = new QVBoxLayout();
	layout->addLayout(buttlay);

	//** Stop Button
	buttonStop = new QPushButton();
	buttonStop->setText(tr(" Stop "));
	buttlay->addWidget(buttonStop);
	buttonStop->setEnabled(false);
	buttonStop->setMinimumSize(QSize( 100, 20));
	connect(buttonStop, SIGNAL(clicked()), this, SLOT(on_stop_clicked()));

	//** Start Button
	buttonStart = new QPushButton();
	buttonStart->setText(tr(" Start "));
	buttonStart->setMinimumSize(QSize( 100, 20)); 
	buttlay->addWidget(buttonStart);
	//* connection is done in fgx not here..

	//** Bottom layout
	QHBoxLayout *bottlay = new QHBoxLayout();
	layout->addLayout(bottlay);
	

}


// Read standard output
// *TBD* 2011-06-12: Unfortunately this can results in quite 'broken' messages, especially since we are
// adding the 'title' to the head of the output, so we get things like :-
// FlightGear: Initializing Nasal Electrical System
//
// FlightGear: C
// FlightGear: o
// FlightGear: uld not find at least one of the following objects for animation:
//
// An enhancement would be to 'collect' the 'reads', and only output to the LOG
// (and status line) perhaps on a "\n" - same for readError()...
//
// This would also require ensuring the last 'read' as the app ended, or
// when the user pushed [Quit], to get this last to the LOG also...
//
// AND if this collection is done, especially on a "\n" basis, then this
// outLog should NOT add an ADDITIONAL line ending!!!
// ie use outLog(string,0);
//
// And perhaps even at this stage, the 'title' should NOT be added, and
// just depend on line endings being in the stream. This would ONLY
// become 'confusing' when running more than one app, like fgfs and fgcom...
//
void ExeControls::readOutput()

{
	QString fgxoutput = P->readAllStandardOutput();

	// write output to fgxlog
	outLog(ExeControls::title()+": "+fgxoutput);
}


// Read standard error
void ExeControls::readError()

{
	QString fgxerror = P->readAllStandardError();
	
	// write errors to fgxlog
	outLog(ExeControls::title()+": "+fgxerror);
}


//==========================================================================
// Start Executable
// and connecting console output/errors for log
//==========================================================================
void ExeControls::start(QString command_line){
	
	P = new QProcess();
	
	connect( P, SIGNAL(readyReadStandardOutput()),this, SLOT(readOutput()));
	connect( P, SIGNAL(readyReadStandardError()),this, SLOT(readError()));

        // QStringList user_env = LauncherWindow->advancedOptionsWidget->get_env(); // hmm too difficult this way
        // so during QString LauncherWindow::fg_args() set  exeFgfs->user_env = advancedOptionsWidget->get_env();
        // and exeFgfs->runtime = advancedOptionsWidget->get_runtime(); ready for use here...
        if (user_env.size()) {
            QStringList env = QProcess::systemEnvironment();
            // Add like env << "LD_LIBRARY_PATH=/home/geoff/fg/fg15/install/OSG283/lib";
            // and/or env << "OSG_STEREO=ON" << "OSG_STEREO_MODE=ANAGLYPHIC" etc, etc
            env << user_env;
            P->setEnvironment(env); // establish the additional USER environment
            outLog("Added User env ["+user_env.join(" && ")+"]");
        }
        if (runtime.size()) {
            QDir d;
            if (d.exists(runtime)) {
                P->setWorkingDirectory(runtime);
                outLog("Set runtime directory to ["+runtime+"]");
            } else {
                outLog("WARNING: User runtime directory ["+runtime+"] DOES NOT EXIST!");
            }
        }
	

 	P->start( QString(command_line));
	
	//* hm ? 
	int res = P->waitForStarted();
	
	//* we need process state
	int pstate = P->state();
	
		//* command is sent anyway
		if (res) {
			outLog("*** FGx tries to execute: " + command_line + " ***");
			//* but we can check process state
			if (pstate != 0) {
				buttonStart->setEnabled(false);
				buttonStop->setEnabled(true);
				
				// write pid to log
				QString pidstr = QString::number( get_pid() );
				outLog("*** FGx reports: SUCCESS ---> this is now PID " + pidstr + " ***");
				
			} else {
				outLog("*** FGx says: Process not started! ***");
			}

			
		}

	
	
	}

//==========================================================================
// Get PID
// 
//==========================================================================
int ExeControls::get_pid() {
#if !defined (Q_OS_WIN)
	int PID = P->pid();
	return PID;
# endif
	return 0;
}

//==========================================================================
// Stop Executable
// 
//==========================================================================
void ExeControls::on_stop_clicked(){
	buttonStop->setEnabled(false);
	buttonStart->setEnabled(true);
}


//==========================================================================
// Kill process
// 
//==========================================================================
void ExeControls::killproc() {
	P->kill();
}


