

#include <QHBoxLayout>

#include <QGroupBox>
#include <QFile>


#include "logs/viewlogswidget.h"

ViewLogsWidget::ViewLogsWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mOb;

	setWindowIcon(QIcon(":/icon/log"));
	setWindowTitle(tr("Log"));
	setMinimumWidth(600);
	setMinimumHeight(600);

	QHBoxLayout *mainLayout = new QHBoxLayout();

	setLayout(mainLayout);


	tabWidget = new QTabWidget();
	mainLayout->addWidget(tabWidget);

	//================================================
	//== Fg Process Log

	txtFgFsLog = new QPlainTextEdit();
	tabWidget->addTab(txtFgFsLog, QIcon(":/icon/log"), "FlightGear log");


	//================================================
	//== Terrasync Log

	txtTerraSyncLog = new QPlainTextEdit();
	tabWidget->addTab(txtTerraSyncLog, QIcon(":/icon/log"), "TerraSync log");


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

	QString file_name = mainObject->settings->log_file_path();
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

	}else if(log_name == "fgx"){
		txtFgxLog->clear();

	}else if(log_name == "terrasync"){
		txtTerraSyncLog->clear();
	}
}

//=========================================================
//==  add log
void ViewLogsWidget::add_log(QString log_name, QString line)
{
	if(log_name == "fgfs"){
		txtFgFsLog->insertPlainText(line);
		txtFgFsLog->moveCursor(QTextCursor::End);

	}else if(log_name == "fgx"){
		txtFgxLog->insertPlainText(line);
		txtFgxLog->moveCursor(QTextCursor::End);

	}else if(log_name == "terrasync"){
		txtTerraSyncLog->insertPlainText(line);
		txtTerraSyncLog->moveCursor(QTextCursor::End);
	}
}
