

//#include <QDebug>

#include <QtCore/QProcess>

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>


#include "panes/outputpreviewwidget.h"

OutputPreviewWidget::OutputPreviewWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{
	mainObject = mOb;

	QHBoxLayout *mainLayout = new QHBoxLayout();
	setLayout(mainLayout);


	QGroupBox *groupBox = new QGroupBox(tr("Debug Output"));
	mainLayout->addWidget(groupBox);

	QHBoxLayout *layoutBox = new QHBoxLayout();
	groupBox->setLayout(layoutBox);


	txtPreviewOutput = new QPlainTextEdit();
	layoutBox->addWidget(txtPreviewOutput);


	QVBoxLayout *layoutButtons = new QVBoxLayout();
	layoutBox->addLayout(layoutButtons);


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
	buttonCommandPreview->setText(tr("Show Command"));
	layoutButtons->addWidget(buttonCommandPreview);
	connect(buttonCommandPreview, SIGNAL(clicked()), this, SLOT(preview()));

	buttonCommandHelp = new QPushButton();
	buttonCommandHelp->setText(tr("Options Help"));
	layoutButtons->addWidget(buttonCommandHelp);
	connect(buttonCommandHelp, SIGNAL(clicked()), this, SLOT(on_command_help()));

	layoutButtons->addStretch(20);

	buttonGroup->button(mainObject->settings->value("preview_type", "1").toInt())->setChecked(true);
}



void OutputPreviewWidget::on_command_help(){
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


void OutputPreviewWidget::preview(){
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
