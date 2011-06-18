
#include <QDebug>

#include <QtCore/QString>
#include <QtCore/QStringList>
//#include <QtCore/QVariant>

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QRadioButton>

#include "advancedoptionswidget.h"
#include "xwidgets/xgroupboxes.h"

AdvancedOptionsWidget::AdvancedOptionsWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{
	mainObject = mOb;


	QVBoxLayout *mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	//================================================================
	//** Additional Args
	QGroupBox *groupBoxArgs = new QGroupBox(tr("Additional Command Arguments"));
	mainLayout->addWidget(groupBoxArgs);

	QVBoxLayout *layoutBoxArgs = new QVBoxLayout();
	groupBoxArgs->setLayout(layoutBoxArgs);
	txtExtraArgs = new QPlainTextEdit();
	layoutBoxArgs->addWidget(txtExtraArgs);

	//================================================================
	//** Environment Variables
	QGroupBox *groupBoxEnv = new QGroupBox(tr("Additional Environment Variables"));
	mainLayout->addWidget(groupBoxEnv);
	
	QHBoxLayout *layoutBoxEnv = new QHBoxLayout();
	groupBoxEnv->setLayout(layoutBoxEnv);
	txtExtraEnv = new QPlainTextEdit();
	layoutBoxEnv->addWidget(txtExtraEnv);
	

	//================================================================
	//** Runtime Directory
	
	// *TBD* Would also be nice to have a button [...] to aid in setting this directory.
	
	QGroupBox *groupBoxRun = new QGroupBox(tr("Set a specific runtime directory"));
	mainLayout->addWidget(groupBoxRun);
	
	QHBoxLayout *layoutBoxRun = new QHBoxLayout();
	groupBoxRun->setLayout(layoutBoxRun);
	txtRuntime = new QLineEdit("");
	layoutBoxRun->addWidget(txtRuntime);


	//================================================================
	//** Write Log
	groupBoxWriteLog = new QGroupBox(tr("FlightGear Loglevel"));
	mainLayout->addWidget(groupBoxWriteLog);

	QVBoxLayout *layoutBoxLog = new QVBoxLayout();
	groupBoxWriteLog->setLayout(layoutBoxLog);

	buttonGroupLogOptions = new QButtonGroup();
	buttonGroupLogOptions->setExclusive(true);

	QStringList values;
	values << "warn" << "info" << "debug" << "bulk" << "alert";
	QStringList labels;
	labels << tr("Warn") << tr("Info") << tr("Debug") << tr("Bulk") << tr("Alert");

	for(int i = 0; i < values.length(); i++){
		QRadioButton *butt = new QRadioButton();
		butt->setText(labels.at(i));
		butt->setProperty("value", values.at(i));
		layoutBoxLog->addWidget(butt);
		buttonGroupLogOptions->addButton(butt, i);
		buttonGroupLogOptions->setId(butt, i);
	}

}


//========================================================
//** Get Args
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

//========================================================
//** Get runtime
QString AdvancedOptionsWidget::get_runtime(){
    QString rt = txtRuntime->text().trimmed();
    return rt;
}


//========================================================
//** Load Settings
void AdvancedOptionsWidget::load_settings(){

	txtExtraArgs->setPlainText(mainObject->settings->value("extra_args").toString());
	txtExtraEnv->setPlainText(mainObject->settings->value("extra_env").toString());
	txtRuntime->setText(mainObject->settings->value("runtime_dir").toString());
	buttonGroupLogOptions->button( mainObject->settings->value("log_level", 0).toInt() )->setChecked(true);
}


//========================================================
//** Save Settings
void AdvancedOptionsWidget::save_settings(){

	mainObject->settings->setValue("extra_args", txtExtraArgs->toPlainText());
	mainObject->settings->setValue("extra_env", txtExtraEnv->toPlainText());
	mainObject->settings->setValue("runtime_dir", txtRuntime->text());
	mainObject->settings->setValue("log_level", buttonGroupLogOptions->checkedId());
}
