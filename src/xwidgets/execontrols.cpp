
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QtCore/QProcess>
#include <QtCore/QTimer>

#include <QtGui/QHBoxLayout>

#include "execontrols.h"
//#include "panes/advancedoptionswidget.h"
//#include "launcher/launcherwindow.h"
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
	QHBoxLayout *buttlay = new QHBoxLayout();
	layout->addLayout(buttlay);

	//** Stop Button
	buttonStop = new QPushButton();
	buttonStop->setText(tr(" Stop "));
	buttlay->addWidget(buttonStop);
	buttonStop->setEnabled(false);
	connect(buttonStop, SIGNAL(clicked()), this, SLOT(on_stop_clicked()));

	//** Start Button
	buttonStart = new QPushButton();
	buttonStart->setText(tr(" Start "));
	buttlay->addWidget(buttonStart);
	//* connection is done in fgx not here..
	



	//** Bottom layout
	QHBoxLayout *bottlay = new QHBoxLayout();
	layout->addLayout(bottlay);
	
	//* Status Bar
	
	statusBar = new QStatusBar();
	statusBar->setSizeGripEnabled(false);
	statusBar->setMinimumWidth(200);
	statusBar->setMinimumHeight(30);
	statusBar->setContentsMargins(0,0,0,0);
	bottlay->addWidget(statusBar);

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
	
	// show some lines in statusbar as feedback
	statusBar->showMessage(fgxoutput, 6000);

	// write output to fgxlog
        outLog(ExeControls::title()+": "+fgxoutput);
}


// Read standard error
void ExeControls::readError()

{
	QString fgxerror = P->readAllStandardError();
	
	// show some lines in statusbar as feedback
	statusBar->showMessage(fgxerror, 6000);
	
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
	
	//Geoff says we need a OSG or LD_LIBRARY_PATH option once!
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
        outLog("Starting: "+command_line);
 	P->start( QString(command_line));
	
	int res = P->waitForStarted();
		if (res) {
			buttonStart->setEnabled(false);
			buttonStop->setEnabled(true);
		}
	
	
	}



//==========================================================================
// Stop Executable
// 
//==========================================================================
void ExeControls::on_stop_clicked(){
	//danger!danger!
	P->kill();
	buttonStop->setEnabled(false);
	buttonStart->setEnabled(true);
}

//** Refresh Button clicked
void ExeControls::on_refresh_clicked(){
	//update_pid();
}


