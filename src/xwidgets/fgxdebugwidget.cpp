
#include <QTreeView>
#include <QHeaderView>
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

	QVBoxLayout *mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	QTabWidget *tabWidget = new QTabWidget();
	mainLayout->addWidget(tabWidget);

	//=============================================================
	// TEMP DEBUG TREE
	QTreeView *tree = new QTreeView();
	tree->setRootIsDecorated(false);
	tabWidget->addTab(tree, QIcon(":/icon/debug"), "Model View");

	tree->setModel(mainObject->X);
	tree->setSortingEnabled(true);
	tree->sortByColumn(XSettingsModel::C_OPTION, Qt::AscendingOrder);
	tree->setUniformRowHeights(true);
	tree->header()->setStretchLastSection(true);

	tree->setColumnHidden(XSettingsModel::C_LEVEL, true);
	tree->setColumnHidden(XSettingsModel::C_AREA, true);

	tree->setColumnWidth(XSettingsModel::C_ENABLED, 40);
	tree->setColumnWidth(XSettingsModel::C_OPTION, 200);
	tree->setColumnWidth(XSettingsModel::C_VALUE, 200);


	//+++ Text Command
	txtCommand = new QPlainTextEdit();
	tabWidget->addTab(txtCommand, QIcon(":/icon/debug"), "Command View");



	QHBoxLayout *bottBox = new QHBoxLayout();
	mainLayout->addLayout(bottBox);
	bottBox->addStretch(10);

	QPushButton *buttLoad = new QPushButton();
	buttLoad->setText("Load Ini");
	bottBox->addWidget(buttLoad);
	connect(buttLoad, SIGNAL(clicked()), mainObject->X, SLOT(read_ini()));

	QPushButton *buttSave = new QPushButton();
	buttSave->setText("Write Ini");
	bottBox->addWidget(buttSave);
	connect(buttSave, SIGNAL(clicked()), mainObject->X, SLOT(write_ini()));

	tabWidget->setCurrentIndex(mainObject->settings->value("fgx_debug_last_tab",0).toInt());
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_tab(int)));

}

void FgxDebugWidget::on_tab(int idx)
{
	mainObject->settings->setValue("fgx_debug_last_tab", idx);
}
