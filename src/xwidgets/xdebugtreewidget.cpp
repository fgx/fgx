
#include <QVBoxLayout>
#include <QTreeView>
#include <QIcon>

#include <QTreeView>
#include <QHeaderView>

#include "xdebugtreewidget.h"

XDebugTreeWidget::XDebugTreeWidget(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

	QVBoxLayout *mainLayout = new QVBoxLayout();
	setLayout(mainLayout);


	buttonGroupFilter = new QButtonGroup(this);
	QStringList filters;
	filters << "None" << "Enabled" << "Disabled";



	//=============================================================
	// TEMP DEBUG TREE
	QTreeView *tree = new QTreeView();
	tree->setRootIsDecorated(false);
	mainLayout->addTab(tree);

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

}
