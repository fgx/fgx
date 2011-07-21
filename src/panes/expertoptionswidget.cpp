

//#include <QDebug>

#include <QtCore/QProcess>

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>

#include "xwidgets/xgroupboxes.h"
#include "panes/expertoptionswidget.h"
#include "utilities/helpers.h"
#include "utilities/utilities.h"

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
	groupBoxArgs = new XGroupVBox(tr("Additional Command Arguments"));
	groupBoxArgs->setCheckable(true);
	leftLayout->addWidget(groupBoxArgs);
	connect(groupBoxArgs, SIGNAL(toggled(bool)), this, SLOT(on_extra()));

	txtExtraArgs = new QPlainTextEdit();
	groupBoxArgs->addWidget(txtExtraArgs);
	connect(txtExtraArgs, SIGNAL(textChanged()), this, SLOT(on_extra()));


	//======================
	//== Environment Variables
	groupBoxEnv = new XGroupVBox(tr("Additional Environment Variables"));
	groupBoxEnv->setCheckable(true);
	leftLayout->addWidget(groupBoxEnv);
	connect(groupBoxEnv, SIGNAL(toggled(bool)), this, SLOT(on_env()));

	txtExtraEnv = new QPlainTextEdit();
	groupBoxEnv->addWidget(txtExtraEnv);
	connect(txtExtraEnv, SIGNAL(textChanged()), this, SLOT(on_env()));


	//======================
	//== Runtime Directory
	// *TBD* Would also be nice to have a button [...] to aid in setting this directory.
	XGroupHBox *groupBoxRun = new XGroupHBox(tr("Set a specific runtime directory"));
	leftLayout->addWidget(groupBoxRun);

	txtRuntime = new QLineEdit("");
	groupBoxRun->addWidget(txtRuntime);
	connect(txtRuntime, SIGNAL(textChanged(QString)), this, SLOT(on_runtime()));


	//======================
	//== Write Log
	groupBoxWriteLog = new XGroupVBox(tr("FlightGear Loglevel"));
	leftLayout->addWidget(groupBoxWriteLog);

	comboLogLevels = new QComboBox();
	groupBoxWriteLog->addWidget(comboLogLevels);
	connect(comboLogLevels, SIGNAL(currentIndexChanged(int)), this, SLOT(on_log_level()));

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

	commandPreviewWidget = new XCommandPrevieWidget(mainObject);
	groupBox->addWidget(commandPreviewWidget);


	connect(this, SIGNAL(setx(QString,bool,QString)), mainObject->X, SLOT(set_option(QString,bool,QString)) );
	connect(mainObject->X, SIGNAL(upx(QString,bool,QString)), this, SLOT(on_upx(QString,bool,QString)));

}




//==========================================================
//= Events
//==========================================================

//= Extras Options changed
void ExpertOptionsWidget::on_extra()
{
	emit setx("extra_args", groupBoxArgs->isChecked(), txtExtraArgs->toPlainText().trimmed());
}

//= Extra Env changed
void ExpertOptionsWidget::on_env()
{
	emit setx("extra_env", groupBoxEnv->isChecked(), txtExtraEnv->toPlainText().trimmed());
}

//= Log level changed
void ExpertOptionsWidget::on_log_level()
{
	QString log = comboLogLevels->itemData(comboLogLevels->currentIndex()).toString();
	emit setx("--log-level=", log != "none",  log);
}

//= Runtime changed
void ExpertOptionsWidget::on_runtime()
{
	emit setx("runtime", true, txtRuntime->text().trimmed());
}



//==========================================================
//= Update
//==========================================================
// NOTE: QPlainTextEfit need only be "set" when
//       text is different to void recursive loop

void ExpertOptionsWidget::on_upx(QString option, bool enabled, QString value)
{
	Q_UNUSED(enabled);



	if(option == "extra_args"){
		if(value != txtExtraArgs->toPlainText()){
			txtExtraArgs->setPlainText(value);
		}

	}else if(option == "extra_env"){
		if(value != txtExtraEnv->toPlainText()){
			txtExtraEnv->setPlainText(value);
		}

	}else if(option == "runtime"){
		txtRuntime->setText(value);

	}else if(option == "--log-level="){
		Helpers::select_combo(comboLogLevels, value);
	}
}


