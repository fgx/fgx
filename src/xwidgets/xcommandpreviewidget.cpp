
#include <QProcess>

#include "xwidgets/xcommandpreviewidget.h"

#include "utilities/utilities.h"

XCommandPrevieWidget::XCommandPrevieWidget(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mob;

	QVBoxLayout *mainLayout = new QVBoxLayout();

	setLayout(mainLayout);


	txtPreviewOutput = new QPlainTextEdit();
	mainLayout->addWidget(txtPreviewOutput);
	txtPreviewOutput->setReadOnly(true);


	QHBoxLayout *layoutButtons = new QHBoxLayout();
	mainLayout->addLayout(layoutButtons);


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
	QPushButton *buttonCommandPreview = new QPushButton();
	buttonCommandPreview->setText(tr("Preview"));
	layoutButtons->addWidget(buttonCommandPreview);
	connect(buttonCommandPreview, SIGNAL(clicked()), this, SLOT(preview()));

	QPushButton *buttonCommandHelp = new QPushButton();
	buttonCommandHelp->setText(tr("Help"));
	layoutButtons->addWidget(buttonCommandHelp);
	connect(buttonCommandHelp, SIGNAL(clicked()), this, SLOT(on_command_help()));

	QPushButton* buttonCommandVersion = new QPushButton();
	buttonCommandVersion->setText(tr("Version"));
	layoutButtons->addWidget(buttonCommandVersion);
	connect(buttonCommandVersion, SIGNAL(clicked()), this, SLOT(on_command_version()));

	layoutButtons->addStretch(20);

	buttonGroup->button(mainObject->settings->value("preview_type", "1").toInt())->setChecked(true);


	// Connections
	connect(mainObject->X, SIGNAL(updated(QStringList)), this, SLOT(preview()));

}



//=============================================================================================================//
//== Command Preview Handlers
//=============================================================================================================//


void XCommandPrevieWidget::on_command_help(){
	QProcess process;
	QStringList args;
	args << "-h" << "-v" << QString("--fg-root=").append(mainObject->X->fgroot());
	process.start(mainObject->X->fgfs_path(), args, QIODevice::ReadOnly);
	if(process.waitForStarted()){
		process.waitForFinished(10000);
		QString ok_result = process.readAllStandardOutput();
		txtPreviewOutput->setPlainText(ok_result);
	}
}

void XCommandPrevieWidget::on_command_version(){
	QProcess process;
	QStringList args;
	args << "-h" << "--version" << QString("--fg-root=").append(mainObject->X->fgroot());
	process.start(mainObject->X->fgfs_path(), args, QIODevice::ReadOnly);
	if(process.waitForStarted()){
		process.waitForFinished(10000);
		QString ok_result = process.readAllStandardError();
		outLog(process.readAllStandardError());
		txtPreviewOutput->setPlainText(ok_result);
	}
}


void XCommandPrevieWidget::preview(){
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

	txtPreviewOutput->setPlainText(mainObject->X->get_fgfs_list().join(delim));
}

