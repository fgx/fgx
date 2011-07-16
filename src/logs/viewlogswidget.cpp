

#include <QHBoxLayout>

#include <QGroupBox>
#include <QFile>


#include "logs/viewlogswidget.h"
#include "utilities/utilities.h"

ViewLogsWidget::ViewLogsWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mOb;

	setWindowIcon(QIcon(":/icon/log"));
	setWindowTitle(tr("Log"));
	setMinimumWidth(600);
	setMinimumHeight(600);
	setProperty("settings_namespace", QVariant("view_logs_widget"));
	mainObject->settings->restoreWindow(this);

	QHBoxLayout *mainLayout = new QHBoxLayout();

	setLayout(mainLayout);


	tabWidget = new QTabWidget();
	mainLayout->addWidget(tabWidget);

	//================================================
	//== FgFs Log
	txtFgFsLog = new QPlainTextEdit();
	tabWidget->addTab(txtFgFsLog, QIcon(":/icon/log"), "FlightGear log");


	//================================================
	//== Terrasync Log
	txtTerraSyncLog = new QPlainTextEdit();
	tabWidget->addTab(txtTerraSyncLog, QIcon(":/icon/log"), "TerraSync log");

	//================================================
	//== FgCom Log
	txtFgComLog = new QPlainTextEdit();
	tabWidget->addTab(txtFgComLog, QIcon(":/icon/log"), "Fgcom log");


	//==============================================
	//== Fgx Log
	QGroupBox *groupBox = new QGroupBox(tr("Log Output"));
	tabWidget->addTab(groupBox, QIcon(":/icon/log"), "FGx log");


	QHBoxLayout *layoutBox = new QHBoxLayout();
	groupBox->setLayout(layoutBox);

	txtFgxLog= new QPlainTextEdit();
	layoutBox->addWidget(txtFgxLog);


	QVBoxLayout *layoutButtons = new QVBoxLayout();
	layoutBox->addLayout(layoutButtons);

	buttonFgxRefresh = new QPushButton();
	buttonFgxRefresh->setText(tr("Refresh"));
	buttonFgxRefresh->setIcon(QIcon(":/icon/refresh"));
	connect(buttonFgxRefresh, SIGNAL(clicked()), this, SLOT(on_load_fgx_log_file()));

	layoutButtons->addWidget(buttonFgxRefresh);


	layoutButtons->addStretch(20);


	on_load_fgx_log_file();
}


//=========================================================
//== FGx log
void ViewLogsWidget::on_load_fgx_log_file(){

	QString file_name = mainObject->log_file_path();
	QFile file(file_name);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		return;
	}
	txtFgxLog->setPlainText( file.readAll());
}






//=========================================================
//==  clear log
void ViewLogsWidget::clear_log(QString log_name)
{
	if(log_name == "fgfs"){
		txtFgFsLog->clear();
        if (stgFgFsLog.length()) {
            outLog("clear_log:"+log_name+":"+stgFgFsLog);
            stgFgFsLog.clear();
        }
	}else if(log_name == "fgx"){
		txtFgxLog->clear();
        if (stgFgxLog.length()) {
            outLog("clean_log:"+log_name+":"+stgFgxLog);
            stgFgxLog.clear();
        }
	}else if(log_name == "fgcom"){
		txtFgComLog->clear();
        if (stgFgComLog.length()) {
            outLog("clear_log:"+log_name+":"+stgFgComLog);
            stgFgComLog.clear();
        }

	}else if(log_name == "terrasync"){
		txtTerraSyncLog->clear();
        if (stgTerraSyncLog.length()) {
            outLog("clear_log:"+log_name+":"+stgTerraSyncLog);
            stgTerraSyncLog.clear();
        }
    }
}

//=========================================================
//==  add log
void ViewLogsWidget::add_log(QString log_name, QString line)
{
    bool hasle = line.contains(QChar('\n'));
    QString trim = line.trimmed();
    int len = trim.length();
	if(log_name == "fgfs"){
        txtFgFsLog->insertPlainText(line);
        txtFgFsLog->moveCursor(QTextCursor::End);
        if (len) {
            if (hasle)
                stgFgFsLog.append(trim);
            else
                stgFgFsLog.append(line);
        }
        if (hasle && stgFgFsLog.length()) {
            outLog("add_log:"+log_name+":"+stgFgFsLog);
            stgFgFsLog.clear();
        }

	}else if(log_name == "fgx"){
		txtFgxLog->insertPlainText(line);
		txtFgxLog->moveCursor(QTextCursor::End);
        if (len) {
            if (hasle)
                stgFgxLog.append(trim);
            else
                stgFgxLog.append(line);
        }
        if (hasle && stgFgxLog.length()) {
            outLog("add_log:"+log_name+":"+stgFgxLog);
            stgFgxLog.clear();
        }

	}else if(log_name == "fgcom"){
		txtFgComLog->insertPlainText(line);
		txtFgComLog->moveCursor(QTextCursor::End);
        if (len) {
            if (hasle)
                stgFgComLog.append(trim);
            else
                stgFgComLog.append(line);
        }
        if (hasle && stgFgComLog.length()) {
            outLog("add_log:"+log_name+":"+stgFgComLog);
            stgFgComLog.clear();
        }

	}else if(log_name == "terrasync"){
		txtTerraSyncLog->insertPlainText(line);
		txtTerraSyncLog->moveCursor(QTextCursor::End);
        if (len) {
            if (hasle)
                stgTerraSyncLog.append(trim);
            else
                stgTerraSyncLog.append(line);
        }
        if (hasle && stgTerraSyncLog.length()) {
            outLog("add_log:"+log_name+":"+stgTerraSyncLog);
            stgTerraSyncLog.clear();
        }
    } else {
        if (len)
            outLog("add_log:"+log_name+"????:"+trim);
    }
}

//=========================================================
//==  CloseEvent
void ViewLogsWidget::closeEvent(QCloseEvent *event){
    clear_log("fgfs");
    clear_log("fgx");
    clear_log("fgcom");
    clear_log("terrasync");
	mainObject->settings->saveWindow(this);
	mainObject->settings->sync();
	event->accept();
}

ViewLogsWidget::~ViewLogsWidget()
{
    clear_log("fgfs");
    clear_log("fgx");
    clear_log("fgcom");
    clear_log("terrasync");
}

