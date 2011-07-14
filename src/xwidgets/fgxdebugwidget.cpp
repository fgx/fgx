
#include <QTreeView>
#include <QTabWidget>
#include <QVBoxLayout>

#include "xwidgets/fgxdebugwidget.h"



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
	QTreeView *treeDebugModel = new QTreeView();
	treeDebugModel->setRootIsDecorated(false);
	tabWidget->addTab(treeDebugModel,QIcon(":/icon/debug"), "Settings Model");
	treeDebugModel->setModel(mainObject->S);



}
