
//#include <QDebug>

#include <QtCore/QString>
#include <QtCore/QStringList>

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>

#include <QtGui/QRadioButton>

#include "advancedoptionswidget.h"
#include "utilities/helpers.h"

AdvancedOptionsWidget::AdvancedOptionsWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{
	mainObject = mOb;


	QVBoxLayout *mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	//================================================================
	//== Additional Args
	XGroupVBox *groupBoxArgs = new XGroupVBox(tr("Additional Command Arguments"));
	mainLayout->addWidget(groupBoxArgs);

	txtExtraArgs = new QPlainTextEdit();
	groupBoxArgs->addWidget(txtExtraArgs);

	//================================================================
	//== Environment Variables
	XGroupVBox *groupBoxEnv = new XGroupVBox(tr("Additional Environment Variables"));
	mainLayout->addWidget(groupBoxEnv);
	
	txtExtraEnv = new QPlainTextEdit();
	groupBoxEnv->addWidget(txtExtraEnv);
	

	//================================================================
	//** Runtime Directory
	
	// TODO Would also be nice to have a button [...] to aid in setting this directory.
	
	XGroupHBox *groupBoxRun = new XGroupHBox(tr("Set a specific runtime directory"));
	mainLayout->addWidget(groupBoxRun);
	
	txtRuntime = new QLineEdit("");
	groupBoxRun->addWidget(txtRuntime);


	//================================================================
	//== Write Log
	groupBoxWriteLog = new XGroupVBox(tr("FlightGear Loglevel"));
	mainLayout->addWidget(groupBoxWriteLog);

	buttonGroupLogOptions = new QButtonGroup();
	buttonGroupLogOptions->setExclusive(true);

	QStringList values;
	values << "none" << "warn" << "info" << "debug" << "bulk" << "alert";
	QStringList labels;
	labels << tr("None") << tr("Warn") << tr("Info") << tr("Debug") << tr("Bulk") << tr("Alert");

	for(int i = 0; i < values.length(); i++){
		QRadioButton *butt = new QRadioButton();
		butt->setText(labels.at(i));
		butt->setProperty("value", values.at(i));
		groupBoxWriteLog->addWidget(butt);
		buttonGroupLogOptions->addButton(butt, i);
	}

}


//========================================================
//== Get Args
QStringList AdvancedOptionsWidget::get_args(){

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
	
	args << "--log-level=" + values[buttonGroupLogOptions->checkedId()];
	
	return args;
}

//========================================================
//** Get Env
QStringList AdvancedOptionsWidget::get_env(){

        QStringList args;
		//=  Additonal args in text box..

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

//========================================================
//== Get runtime
QString AdvancedOptionsWidget::get_runtime(){
    QString rt = txtRuntime->text().trimmed();
    return rt;
}


//========================================================
//==* Load Settings
void AdvancedOptionsWidget::load_settings(){

	txtExtraArgs->setPlainText(mainObject->settings->value("extra_args").toString());
	txtExtraEnv->setPlainText(mainObject->settings->value("extra_env").toString());
	txtRuntime->setText(mainObject->settings->value("runtime_dir").toString());
	Helpers::select_radio(buttonGroupLogOptions, mainObject->settings->value("log_level","none").toString() );
}


//========================================================
//** Save Settings
void AdvancedOptionsWidget::save_settings(){

	mainObject->settings->setValue("extra_args", txtExtraArgs->toPlainText());
	mainObject->settings->setValue("extra_env", txtExtraEnv->toPlainText());
	mainObject->settings->setValue("runtime_dir", txtRuntime->text());
	mainObject->settings->setValue("log_level", buttonGroupLogOptions->checkedButton()->property("value").toString());
}
