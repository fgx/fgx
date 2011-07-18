
#include <QVBoxLayout>
#include <QTreeView>
#include <QModelIndex>
#include <QIcon>

#include <QTreeView>
#include <QHeaderView>
#include <QToolBar>

#include "xwidgets/xdebugtreewidget.h"
#include "xobjects/xsettingsmodel.h"

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
		filters << "All" << "Enabled" << "Disabled";
		connect(groupFilter, SIGNAL(triggered(QAction*)), this, SLOT(on_filter(QAction*)) );
		for(int fidx = 0; fidx < filters.size(); fidx++ )
		{
			QAction *act = toolbar->addAction(filters.at(fidx)) ;
			act->setCheckable(true);
			act->setChecked(filters.at(fidx) == "All");
			act->setProperty("filter", filters.at(fidx));
			groupFilter->addAction( act );

		}

		// ======================
		// == Models
		proxyModel = new QSortFilterProxyModel(this);
		proxyModel->setSourceModel(mainObject->X);
		proxyModel->setFilterKeyColumn(XSettingsModel::C_ENABLED);



		//=============================================================
		// TEMP DEBUG TREE
		tree = new QTreeView();
		tree->setRootIsDecorated(false);
		mainLayout->addWidget(tree);

		tree->setModel(proxyModel);

		tree->setSelectionBehavior(QAbstractItemView::SelectRows);
		tree->setSortingEnabled(true);
		tree->sortByColumn(XSettingsModel::C_OPTION, Qt::AscendingOrder);
		tree->setUniformRowHeights(true);
		tree->header()->setStretchLastSection(true);

		tree->setColumnHidden(XSettingsModel::C_LEVEL, true);
		tree->setColumnHidden(XSettingsModel::C_AREA, true);

		tree->setColumnWidth(XSettingsModel::C_ENABLED, 40);
		tree->setColumnWidth(XSettingsModel::C_OPTION, 200);
		tree->setColumnWidth(XSettingsModel::C_VALUE, 200);

		connect(mainObject->X, SIGNAL(upx(QString,bool,QString)), this, SLOT(on_select_a_node(QString,bool,QString)));

	}


void XDebugTreeWidget::on_filter(QAction *act)
{
	QString f = act->property("filter").toString();
	if(f == "All"){
		proxyModel->setFilterRegExp("");

	}else if(f == "Enabled"){
		proxyModel->setFilterRegExp("1");

	}else{
		proxyModel->setFilterRegExp("0");
	}
}


void XDebugTreeWidget::on_select_a_node(QString option, bool enabled, QString value)
{
	return; // TODO
	Q_UNUSED(enabled);
	Q_UNUSED(value);
	tree->selectionModel()->setCurrentIndex( proxyModel->mapFromSource(mainObject->X->get_index(option)), QItemSelectionModel::Rows );
	tree->selectionModel()->select( proxyModel->mapFromSource(mainObject->X->get_index(option)), QItemSelectionModel::Rows );
	//qDebug() << "select" << option << " " << mainObject->X->get_index(option).row();
}
