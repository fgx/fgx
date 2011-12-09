
#include <QTabWidget>
#include <QVBoxLayout>

#include "xwidgets/fgxdebugwidget.h"


#include "xobjects/xsettingsmodel.h"

FgxDebugWidget::FgxDebugWidget(MainObject *mob, QWidget *parent) :
	QWidget(parent)
{

	mainObject = mob;

	setMinimumWidth(800);
	setMinimumHeight(600);

	setProperty("settings_namespace", QVariant("fgxdebug_window"));
	mainObject->settings->restoreWindow(this);
    setWindowTitle("FGx Debug");

	QVBoxLayout *mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	QTabWidget *tabWidget = new QTabWidget();
	mainLayout->addWidget(tabWidget);

	//== Debug Tree == Coeden in welsh
	debugTreeWidget = new XDebugTreeWidget(mainObject);
	tabWidget->addTab(debugTreeWidget, QIcon(":/icon/debug"), "Model View");

	//== Command Preview and Output
	commandPreviewWidget = new XCommandPrevieWidget(mainObject);
	tabWidget->addTab(commandPreviewWidget, QIcon(":/icon/debug"), "Command Line");


	//========================================================
	//= Bottom TEST Buttons
	QHBoxLayout *bottBox = new QHBoxLayout();
	mainLayout->addLayout(bottBox);

	QCheckBox *chkOpenOnStartup = new QCheckBox();
	bottBox->addWidget(chkOpenOnStartup);
	chkOpenOnStartup->setText("Open this windows at startup");



	bottBox->addStretch(10);

	QPushButton *buttLoad = new QPushButton();
	buttLoad->setText("Load Ini");
	bottBox->addWidget(buttLoad);
	connect(buttLoad, SIGNAL(clicked()), mainObject->X, SLOT(read_ini()));

	QPushButton *buttSave = new QPushButton();
	buttSave->setText("Write Ini");
	bottBox->addWidget(buttSave);
	connect(buttSave, SIGNAL(clicked()), mainObject->X, SLOT(write_ini()));


	//========================================================
	//= Restore and Connect
	tabWidget->setCurrentIndex(mainObject->settings->value("fgx_debug_last_tab",0).toInt());
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_tab(int)));

	chkOpenOnStartup->setChecked(mainObject->settings->value("fgx_debug_show_on_startup",0).toBool());
	connect(chkOpenOnStartup, SIGNAL(clicked(bool)), this, SLOT(on_open_at_startup(bool)));

}



//= window close
void FgxDebugWidget::closeEvent(QCloseEvent *event){

	mainObject->settings->saveWindow(this);
	mainObject->settings->sync();
	event->accept();
}

//= On Tab
void FgxDebugWidget::on_tab(int idx)
{
	mainObject->settings->setValue("fgx_debug_last_tab", idx);
}

//= On Chk Startup
void FgxDebugWidget::on_open_at_startup(bool checked)
{
	mainObject->settings->setValue("fgx_debug_show_on_startup", checked);
}

