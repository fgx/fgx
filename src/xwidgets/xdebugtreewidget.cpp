
#include <QVBoxLayout>
#include <QTreeView>
#include <QIcon>

#include <QTreeView>
#include <QHeaderView>
#include <QToolBar>

#include "xdebugtreewidget.h"

XDebugTreeWidget::XDebugTreeWidget(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mob;

	QVBoxLayout *mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	QToolBar *toolbar = new QToolBar();
	mainLayout->addWidget(toolbar);

	groupFilter = new QActionGroup(this);
	groupFilter->setExclusive(true);
	QStringList filters;
	filters << "None" << "Enabled" << "Disabled";

	for(int fidx = 0; fidx < filters.size(); fidx++ )
	{
		QAction *act = toolbar->addAction(filters.at(fidx)) ;
		act->setCheckable(true);
		act->setChecked(filters.at(fidx) == "None");
		groupFilter->addAction( act );

	}

	// ======================
	// == Models
	proxyModel = new QSortFilterProxyModel(this);
	proxyModel->setSourceModel(mainObject->X);




	//=============================================================
	// TEMP DEBUG TREE
	QTreeView *tree = new QTreeView();
	tree->setRootIsDecorated(false);
	mainLayout->addWidget(tree);

	tree->setModel(proxyModel);


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
