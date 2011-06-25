
#include <QDebug>

#include <QtCore/QCoreApplication>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

#include <QtGui/QGroupBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>


#include "dbsettingswidget.h"
#include "utilities/utilities.h"


DbSettingsWidget::DbSettingsWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{
	mainObject = mOb;



	QVBoxLayout *mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	QGroupBox *groupBoxServer = new QGroupBox(tr("Database Settings"));
	mainLayout->addWidget(groupBoxServer);

	QGridLayout *layoutServer = new QGridLayout();
	groupBoxServer->setLayout(layoutServer);

	//* Server Engine
	int row = 0;
	layoutServer->addWidget(new QLabel(tr("Database Engine")), row, 0, 1, 1, Qt::AlignRight);

	comboDbType = new QComboBox();
	comboDbType->addItem("QSQLITE", "QSQLITE");
	comboDbType->addItem("QMYSQL", "QMYSQL");
	layoutServer->addWidget(comboDbType, row, 1);
	connect(comboDbType, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combo_engine()));


	//* Host
	row++;
	layoutServer->addWidget(new QLabel(tr("Host Server")), row, 0, 1, 1, Qt::AlignRight);

	txtHost = new QLineEdit();
	layoutServer->addWidget(txtHost, row, 1);


	//* User
	row++;
	layoutServer->addWidget(new QLabel(tr("User")), row, 0, 1, 1, Qt::AlignRight);

	txtUser = new QLineEdit();
	layoutServer->addWidget(txtUser, row, 1);


	//* Pass
	row++;
	layoutServer->addWidget(new QLabel(tr("Password")), row, 0, 1, 1, Qt::AlignRight);

	txtPass = new QLineEdit();
	layoutServer->addWidget(txtPass, row, 1);


	//* Database
	row++;
	layoutServer->addWidget(new QLabel(tr("Database")), row, 0, 1, 1, Qt::AlignRight);

	txtDatabase = new QLineEdit();
	layoutServer->addWidget(txtDatabase, row, 1);

	//*  Test
	row++;
	buttonTest = new QPushButton();
	buttonTest->setText(tr("Test Connection"));
	layoutServer->addWidget(buttonTest, row, 1);
	connect(buttonTest, SIGNAL(clicked()), this, SLOT(on_test_connection()));

	//** Status Bar
	row++;
	statusBar = new QStatusBar();
	statusBar->setSizeGripEnabled(false);
	layoutServer->addWidget(statusBar, row, 0, 1, 2);

	on_combo_engine();
}


//=======================================================
//* Save Settings
void DbSettingsWidget::save_settings(){
	mainObject->settings->setValue("db_engine", comboDbType->currentText());
	outLog("DB Engine saved: " + comboDbType->currentText());
	mainObject->settings->setValue("db_host", txtHost->text());
	mainObject->settings->setValue("db_user", txtUser->text());
	mainObject->settings->setValue("db_pass", txtPass->text());
	mainObject->settings->setValue("db_database", txtDatabase->text());
}


//=======================================================
//* Load Settings
void DbSettingsWidget::load_settings(){
	qDebug() << mainObject->settings->value("db_engine").toString();
	outLog("String empty means: There are NO settings at all! Maybe your first start of FGx ?");
	comboDbType->setCurrentIndex( comboDbType->findData(mainObject->settings->value("db_engine").toString()));
	txtHost->setText(mainObject->settings->value("db_host").toString());
	txtUser->setText(mainObject->settings->value("db_user").toString());
	txtPass->setText(mainObject->settings->value("db_pass").toString());
	txtDatabase->setText(mainObject->settings->value("db_database").toString());
}



//=======================================================
//* On Combo Engine changed
void DbSettingsWidget::on_combo_engine(){
	bool enabled = comboDbType->currentIndex() == 1;
	txtHost->setEnabled(enabled);
	txtUser->setEnabled(enabled);
	txtPass->setEnabled(enabled);
	txtDatabase->setEnabled(enabled);
	buttonTest->setEnabled(enabled);
}


//=======================================================
//* On Test Connection
void DbSettingsWidget::on_test_connection(){

	buttonTest->setDisabled(true);
	//statusBar->showMessage("Connecting: please wait");
	QCoreApplication::processEvents();

	//********************************************************************
	//** SQL database problem
	// I dont want to connect until required
	QSqlDatabase dbtest = QSqlDatabase::addDatabase("QMYSQL");
	//dbtest = QSqlDatabase::addDatabase("QMYSQL");
	dbtest.setHostName(txtHost->text());
	dbtest.setUserName(txtUser->text());
	dbtest.setPassword(txtPass->text());
	dbtest.setDatabaseName(txtDatabase->text());

	if( !dbtest.open() ){
		statusBar->showMessage(dbtest.lastError().text());

	}else{
		statusBar->showMessage("Connected Ok");
	}
	dbtest.close();
	buttonTest->setDisabled(false);
}
