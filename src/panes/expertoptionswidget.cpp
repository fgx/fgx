

//#include <QDebug>

#include <QtCore/QProcess>

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>

#include "xwidgets/xgroupboxes.h"
#include "panes/expertoptionswidget.h"
#include "utilities/helpers.h"

ExpertOptionsWidget::ExpertOptionsWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{
	mainObject = mOb;

	//===============================================================================
	//= Create Layouts
	//===============================================================================
	QHBoxLayout *mainLayout = new QHBoxLayout();
	setLayout(mainLayout);

	QVBoxLayout *leftLayout = new QVBoxLayout();
	mainLayout->addLayout(leftLayout);

	QVBoxLayout *rightLayout = new QVBoxLayout();
	mainLayout->addLayout(rightLayout);


	//===============================================================================
	//= Left is additional args
	//===============================================================================

	//======================
	//== Additional Args
	XGroupVBox *groupBoxArgs = new XGroupVBox(tr("Additional Command Arguments"));
	leftLayout->addWidget(groupBoxArgs);

	txtExtraArgs = new QPlainTextEdit();
	groupBoxArgs->addWidget(txtExtraArgs);


	//======================
	//== Environment Variables
	XGroupVBox *groupBoxEnv = new XGroupVBox(tr("Additional Environment Variables"));
	leftLayout->addWidget(groupBoxEnv);

	txtExtraEnv = new QPlainTextEdit();
	groupBoxEnv->addWidget(txtExtraEnv);


	//======================
	//== Runtime Directory
	// *TBD* Would also be nice to have a button [...] to aid in setting this directory.
	XGroupHBox *groupBoxRun = new XGroupHBox(tr("Set a specific runtime directory"));
	leftLayout->addWidget(groupBoxRun);

	txtRuntime = new QLineEdit("");
	groupBoxRun->addWidget(txtRuntime);


	//======================
	//== Write Log
	groupBoxWriteLog = new XGroupVBox(tr("FlightGear Loglevel"));
	leftLayout->addWidget(groupBoxWriteLog);

	comboLogLevels = new QComboBox();
	groupBoxWriteLog->addWidget(comboLogLevels);

	//TODO - these need to go in "DD"
	QStringList values;
	values << "none" << "warn" << "info" << "debug" << "bulk" << "alert";
	QStringList labels;
	labels << tr("None") << tr("Warn") << tr("Info") << tr("Debug") << tr("Bulk") << tr("Alert");

	for(int i = 0; i < values.length(); i++){
		comboLogLevels->addItem(labels.at(i), values.at(i));
	}



	//===============================================================================
	//= Right is Output preview and help
	//===============================================================================

	XGroupVBox *groupBox = new XGroupVBox(tr("Command Output"));
	rightLayout->addWidget(groupBox);


	txtPreviewOutput = new QPlainTextEdit();
	groupBox->addWidget(txtPreviewOutput);
	txtPreviewOutput->setReadOnly(true);


	QHBoxLayout *layoutButtons = new QHBoxLayout();
	groupBox->addLayout(layoutButtons);


	//=============================================================
	buttonGroup = new QButtonGroup(this);
	buttonGroup->setExclusive(true);
	connect(buttonGroup, SIGNAL(buttonClicked(int)) , this, SLOT(preview()));

	QRadioButton *radioString = new QRadioButton();
	radioString->setText("Single line");
	radioString->setProperty("value", "string");
	layoutButtons->addWidget(radioString);
	buttonGroup->addButton(radioString, 0);

	QRadioButton *radioLines = new QRadioButton();
	radioLines->setText("Separate lines");
	radioLines->setProperty("value", "lines");
	layoutButtons->addWidget(radioLines);
	buttonGroup->addButton(radioLines, 1);

	QRadioButton *radioShell = new QRadioButton();
	radioShell->setText("Shell lines");
	radioShell->setProperty("value", "shell");
	layoutButtons->addWidget(radioShell);
	buttonGroup->addButton(radioShell, 2);


	//=============================================================
	buttonCommandPreview = new QPushButton();
	buttonCommandPreview->setText(tr("Preview Command"));
	layoutButtons->addWidget(buttonCommandPreview);
	connect(buttonCommandPreview, SIGNAL(clicked()), this, SLOT(preview()));

	buttonCommandHelp = new QPushButton();
	buttonCommandHelp->setText(tr("fgfs -h -v"));
	layoutButtons->addWidget(buttonCommandHelp);
	connect(buttonCommandHelp, SIGNAL(clicked()), this, SLOT(on_command_help()));

	layoutButtons->addStretch(20);

	buttonGroup->button(mainObject->settings->value("preview_type", "1").toInt())->setChecked(true);
}



//=============================================================================================================//
//== Advanced Handlers
//=============================================================================================================//

//=================================
//= Get Args
QStringList ExpertOptionsWidget::get_args(){

	QStringList args;
	//*  Additonal args in text box..

	QString extra = txtExtraArgs->toPlainText().trimmed();
	if (extra.length() > 0) {
		QStringList parts = extra.split("\n");
		if(parts.count() > 0){
			for(int i=0; i < parts.count(); i++){
				QString part = parts.at(i).trimmed();
				if(part.length() > 0){
					args << part;
				}
			}
		}
	}

	//* get fgfs log level argument

	QStringList values;
	values << "warn" << "info" << "debug" << "bulk" << "alert";

	args << "--log-level=" + comboLogLevels->itemData(comboLogLevels->currentIndex()).toString();

	return args;
}

//=================================
//== Get Env
QStringList ExpertOptionsWidget::get_env(){

	QStringList args;
	//*  Additonal args in text box..

	QString extra = txtExtraEnv->toPlainText().trimmed();
	if (extra.length() > 0) {
		QStringList parts = extra.split("\n");
		if(parts.count() > 0){
			for(int i=0; i < parts.count(); i++){
				QString part = parts.at(i).trimmed();
				if(part.length() > 0){
					args << part;
				}
			}
		}
	}
	return args;
}

//=================================
//== Get runtime
QString ExpertOptionsWidget::get_runtime(){
	return txtRuntime->text().trimmed();
}





//=============================================================================================================//
//== Command Preview Handlers
//=============================================================================================================//


void ExpertOptionsWidget::on_command_help(){
	QProcess process;
	QStringList args;
	args << "-h" << "-v" << QString("--fg-root=").append(mainObject->settings->fgroot());
	process.start(mainObject->settings->fgfs_path(), args, QIODevice::ReadOnly);
	if(process.waitForStarted()){
		process.waitForFinished(10000);
		QString ok_result = process.readAllStandardOutput();
		txtPreviewOutput->setPlainText(ok_result);
	}
}


void ExpertOptionsWidget::preview(){
	mainObject->settings->setValue("preview_type", buttonGroup->checkedId());
	QString delim("");
	QString type = buttonGroup->checkedButton()->property("value").toString();
	if(type == "lines"){
		delim.append("\n");
	}else if(type == "shell"){
		delim.append(" \\\n");
	}else{
		delim.append(" ");
	}
	QString cmd = mainObject->settings->fgfs_path().append(delim);
	cmd.append( mainObject->get_fgfs_args().join(delim));
	txtPreviewOutput->setPlainText(cmd);
}



//========================================================================================================== ;-)

//=====================
//== Load Settings
void ExpertOptionsWidget::load_settings(){

	txtExtraArgs->setPlainText(mainObject->settings->value("extra_args").toString());
	txtExtraEnv->setPlainText(mainObject->settings->value("extra_env").toString());
	txtRuntime->setText(mainObject->settings->value("runtime_dir").toString());
	Helpers::select_combo(comboLogLevels, mainObject->settings->value("log_level","none").toString() );
}


//=====================
//== Save Settings
void ExpertOptionsWidget::save_settings(){

	mainObject->settings->setValue("extra_args", txtExtraArgs->toPlainText());
	mainObject->settings->setValue("extra_env", txtExtraEnv->toPlainText());
	mainObject->settings->setValue("runtime_dir", txtRuntime->text());
	mainObject->settings->setValue("log_level", comboLogLevels->itemData(comboLogLevels->currentIndex()));
}
