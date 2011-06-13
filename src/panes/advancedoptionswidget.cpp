
#include <QDebug>

#include <QtCore/QString>
#include <QtCore/QStringList>
//#include <QtCore/QVariant>

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QRadioButton>


#include "advancedoptionswidget.h"

AdvancedOptionsWidget::AdvancedOptionsWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{
	mainObject = mOb;


	QHBoxLayout *mainLayout = new QHBoxLayout();
	setLayout(mainLayout);

	//================================================================
	//** Additional Args
	QGroupBox *groupBoxArgs = new QGroupBox(tr("Additional Command Arguments"));
	mainLayout->addWidget(groupBoxArgs);

	QVBoxLayout *layoutBoxArgs = new QVBoxLayout();
	groupBoxArgs->setLayout(layoutBoxArgs);


	txtExtraArgs = new QPlainTextEdit();
	layoutBoxArgs->addWidget(txtExtraArgs);

        // *TBD* These need their own title
        // so perhaps need QGroupBox() for each!
        // but for now...
        // try adding some labels - hmmm, look ok...
        QLabel * labelEnv = new QLabel(tr("Additional Environment Variables (if any)"));
        layoutBoxArgs->addWidget(labelEnv);
        txtExtraEnv = new QPlainTextEdit();
        layoutBoxArgs->addWidget(txtExtraEnv);

        QLabel * labelRT = new QLabel(tr("Set a specific runtime directory. (if any needed)"));
        layoutBoxArgs->addWidget(labelRT);
        // *TBD* Would also be nice to have a button [...] to aid in setting this directory.
        txtRuntime = new QLineEdit("");
        layoutBoxArgs->addWidget(txtRuntime);


	//================================================================
	//** Write Log
	groupBoxWriteLog = new QGroupBox(tr("Write Log Files"));
	groupBoxWriteLog->setCheckable(true);
	groupBoxWriteLog->setChecked(false);
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
	}
	layoutBoxLog->addStretch(100); //* add spacer at end

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

	//* Log Level - Redirect stdout and stderr to logfile MUST be last argument
	// TODO
	/*
	if(checkBoxLogEnabled->isChecked()){
		args_string.append("--log-level=warn ");
		args_string.append("&>").append(QDir::currentPath()).append("/fgfslog.txt");
	}
	*/

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
        groupBoxWriteLog->setChecked(mainObject->settings->value("log_enabled").toBool());
	buttonGroupLogOptions->button( mainObject->settings->value("log_level", 0).toInt() )->setChecked(true);
}


//========================================================
//** Save Settings
void AdvancedOptionsWidget::save_settings(){

	mainObject->settings->setValue("extra_args", txtExtraArgs->toPlainText());
        mainObject->settings->setValue("extra_env", txtExtraEnv->toPlainText());
        mainObject->settings->setValue("runtime_dir", txtRuntime->text());
        mainObject->settings->setValue("log_enabled", groupBoxWriteLog->isChecked());
	mainObject->settings->setValue("log_level", buttonGroupLogOptions->checkedId());
}
