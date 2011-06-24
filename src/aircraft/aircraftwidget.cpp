
/**
	Started apr 2011 by: pete at freeflightsim.org
	then ..

	TODO: Convert QTreeWidget to QTreeView with A filterSortProxy Model

*/



#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QtCore/QDebug>

#include <QtCore/QProcess>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include <QtCore/QFile>


#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QSplitter>
#include <QtGui/QProgressDialog>

#include <QtGui/QToolBar>
#include <QtGui/QToolButton>
#include <QtGui/QAbstractButton>
#include <QtGui/QButtonGroup>

#include <QtGui/QAction>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QTabWidget>

#include <QtGui/QHeaderView>
#include <QtGui/QAbstractItemView>
#include <QtGui/QPixmap>
#include <QtGui/QDoubleValidator>


#include "aircraft/aircraftwidget.h"
#include "aircraft/aircraftimport.h"
#include "xwidgets/xtreewidgetitem.h"
#include "utilities/utilities.h"


AircraftWidget::AircraftWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mOb;
	aircraftImport = new AircraftImport(this, mainObject);
	aircraftImport->rows.clear();

    //* Main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);

    QSplitter *splitter = new QSplitter(this);
    mainLayout->addWidget(splitter);

	//===============================================================================
    //** Left
    QWidget *leftWidget = new QWidget();
    splitter->addWidget(leftWidget);
	splitter->setContentsMargins(10,10,10,10);
    QVBoxLayout *treeLayout = new QVBoxLayout();
    leftWidget->setLayout(treeLayout);
    treeLayout->setContentsMargins(0,0,0,0);
    treeLayout->setSpacing(0);


	//** Top Bar Layout
	QHBoxLayout *treeTopBar = new QHBoxLayout();
	treeLayout->addLayout(treeTopBar);

	//** Use Default Aircraft
	checkBoxUseDefault = new QCheckBox(this);
	checkBoxUseDefault->setText("Use Default");
	treeTopBar->addWidget(checkBoxUseDefault);
	connect(checkBoxUseDefault, SIGNAL(clicked()), this, SLOT(on_use_default_clicked()));

	treeTopBar->addSpacing(10);

	//==========================
	//** Filter tabs
	tabsView = new QTabBar();
	tabsView->addTab(tr("View List"));
	tabsView->addTab(tr("View Nested"));
	treeTopBar->addWidget(tabsView);
	connect(tabsView, SIGNAL(currentChanged(int)), this, SLOT(load_tree()));


	treeTopBar->addStretch(20);

	QToolButton *actionReloadCacheDb = new QToolButton(this);
	actionReloadCacheDb->setText("Import");
	actionReloadCacheDb->setIcon(QIcon(":/icon/import"));
	actionReloadCacheDb->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	actionReloadCacheDb->setAutoRaise(true);
	treeTopBar->addWidget(actionReloadCacheDb);
	connect(actionReloadCacheDb, SIGNAL(clicked()), this, SLOT(on_reload_db_cache()) );

	//===============================================================
    //** Aircraft Tree
	treeWidget = new QTreeWidget(this);
	treeLayout->addWidget(treeWidget);
	treeWidget->setAlternatingRowColors(true);
	treeWidget->setRootIsDecorated(true);
	treeWidget->setUniformRowHeights(true);
	treeWidget->setSortingEnabled(true);	
	treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	treeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

	treeWidget->headerItem()->setText(C_DIR, "Dir");
	treeWidget->headerItem()->setText(C_XML, "xml");
	treeWidget->headerItem()->setText(C_AERO, "Aircraft");
	treeWidget->headerItem()->setText(C_DESCRIPTION, "Description");
	treeWidget->headerItem()->setText(C_FDM, "FDM");
	treeWidget->headerItem()->setText(C_AUTHOR, "Author");
	treeWidget->header()->setStretchLastSection(true);
	treeWidget->setColumnHidden(C_XML, true);
	treeWidget->setColumnWidth(C_DIR, 60);
	treeWidget->setColumnWidth(C_FDM, 60);
	treeWidget->setColumnWidth(C_DESCRIPTION, 200);

	connect( treeWidget,
			 SIGNAL( itemSelectionChanged() ),
			 SLOT( on_tree_selection_changed() )
	);

	statusBarAero = new QStatusBar();
	treeLayout->addWidget(statusBarAero);


    //*************************************************************************************************
    //** Right
    //*************************************************************************************************


    QGroupBox *grpAero = new QGroupBox();
    splitter->addWidget(grpAero);
    grpAero->setDisabled(false);
    grpAero->setTitle(tr("Aircraft Details"));


    QVBoxLayout *aeroLayout = new QVBoxLayout();
    grpAero->setLayout(aeroLayout);
    int aM = 10;
    aeroLayout->setContentsMargins(aM,aM,aM,aM);
    aeroLayout->setSpacing(0);

    //**********************************************8
	//** Aero Panel
	QGroupBox *grpAeroPanel = new QGroupBox();
	grpAeroPanel->setStyleSheet("background-color: black;");
	aeroLayout->addWidget(grpAeroPanel);


	QVBoxLayout *aeroPanelLayout = new QVBoxLayout();
	grpAeroPanel->setLayout(aeroPanelLayout);

	QHBoxLayout *imgBox = new QHBoxLayout();
	aeroPanelLayout->addLayout(imgBox);

	imgBox->addStretch(2);
	aeroImageLabel =  new QLabel(this);
	aeroImageLabel->setFixedWidth(171);
	aeroImageLabel->setFixedHeight(128);
	imgBox->addWidget(aeroImageLabel);
	aeroImageLabel->setAlignment(Qt::AlignHCenter);
	imgBox->addStretch(2);


    //******************************************************8
   //** Tab Widgets
    QTabWidget *aeroTabs = new QTabWidget();
    aeroLayout->addWidget(aeroTabs, 20);

    QWidget *aeroControlWidget = new QWidget();
	aeroTabs->addTab(aeroControlWidget, tr("Nav"));

	QGridLayout *layoutAeroPane = new QGridLayout();
	aeroControlWidget->setLayout(layoutAeroPane);
	int row = 1;

	//* Navs
	layoutAeroPane->addWidget(new QLabel(tr("Nav 1")), row, 0, 1, 1, Qt::AlignRight);
	txtNav1 = new QLineEdit();
	txtNav1->setValidator(new QDoubleValidator(0, 200, 2, this));
	layoutAeroPane->addWidget(txtNav1,row, 1, 1, 1);

	row++;
	layoutAeroPane->addWidget(new QLabel(tr("Nav 2")), row, 0, 1, 1, Qt::AlignRight);
	txtNav2 = new QLineEdit();
	txtNav2->setValidator(new QDoubleValidator(0, 200, 2, this));
	layoutAeroPane->addWidget(txtNav2,row, 1, 1, 1);

	//* ADF
	row++;
	layoutAeroPane->addWidget(new QLabel(tr("Adf")), row, 0, 1, 1, Qt::AlignRight);
	txtAdf = new QLineEdit();
	txtAdf->setValidator(new QDoubleValidator(0, 200,0, this));
	layoutAeroPane->addWidget(txtAdf, row, 1, 1, 1);

	//* Comms
	row++;
	layoutAeroPane->addWidget(new QLabel(tr("Comm 1")), row, 0, 1, 1, Qt::AlignRight);
	txtComm1 = new QLineEdit();
	txtComm1->setValidator(new QDoubleValidator(0, 200, 2, this));
	layoutAeroPane->addWidget(txtComm1,row, 1, 1, 1);

	row++;
	layoutAeroPane->addWidget(new QLabel(tr("Comm 2")), row, 0, 1, 1, Qt::AlignRight);
	txtComm2 = new QLineEdit();
	txtComm2->setValidator(new QDoubleValidator(0, 200, 2, this));
	layoutAeroPane->addWidget(txtComm2,row, 1, 1, 1);

	layoutAeroPane->setRowStretch(row + 1, 20); // stretch end

    //***********************************
    //** Setup

	splitter->setCollapsible(0, false);
	splitter->setCollapsible(1, false);
	splitter->setStretchFactor(0, 50);
	splitter->setStretchFactor(1, 1);

	initialize();
}




//==========================================================================
// Aircraft Selected
//==========================================================================
void AircraftWidget::on_tree_selection_changed(){

	QTreeWidgetItem *item = treeWidget->currentItem();
	if(!item){
            outLog("on_tree_selection_changed: no selected item");
		return;
	}

	//** Check there is item and its a model
	if(item->text(C_AERO).length() == 0){
                outLog("on_tree_selection_changed: no C_AERO item");
                aeroImageLabel->clear();
		emit set_arg("update", "--aircraft=", "");
		return;
	}


	QString thumb_file = QString("%1/%2/%3/thumbnail.jpg").arg( mainObject->settings->aircraft_path(),
                                                                    item->text(C_DIR),
                                                                    item->text(C_AERO));
	if(QFile::exists(thumb_file)){
		QPixmap aeroImage(thumb_file);
		if(!aeroImage.isNull()){
                    outLog("on_tree_selection_changed: Loaded thumb "+thumb_file);
                        aeroImageLabel->setPixmap(aeroImage);
                } else
                    outLog("on_tree_selection_changed: NO thumb load "+thumb_file);

	}else{
            outLog("on_tree_selection_changed: NO thumb "+thumb_file);
                aeroImageLabel->clear();
	}
}







//=============================================================
// Save Settings
void AircraftWidget::save_settings(){
	QTreeWidgetItem *item = treeWidget->currentItem();
	if(item && item->text(C_AERO).length() > 0){
		mainObject->settings->setValue("aircraft", item->text(C_AERO) );
	}
	mainObject->settings->setValue("nav1", txtNav1->text());
	mainObject->settings->setValue("nav2", txtNav2->text());
	mainObject->settings->setValue("adf", txtAdf->text());
	mainObject->settings->setValue("comm1", txtComm1->text());
	mainObject->settings->setValue("comm2", txtComm2->text());
	mainObject->settings->setValue("aircraft_use_default", checkBoxUseDefault->isChecked());
	mainObject->settings->sync();
}


//=============================================================
// Load Settings
void AircraftWidget::load_settings(){

	select_node(mainObject->settings->value("aircraft").toString());

	txtNav1->setText(mainObject->settings->value("nav1").toString());
	txtNav2->setText(mainObject->settings->value("nav2").toString());
	txtAdf->setText(mainObject->settings->value("adf").toString());
	txtComm1->setText(mainObject->settings->value("comm1").toString());
	txtComm2->setText(mainObject->settings->value("comm2").toString());
	checkBoxUseDefault->setChecked(mainObject->settings->value("aircraft_use_default", false).toBool());
	on_use_default_clicked();
}

void AircraftWidget::select_node(QString aero){

	QList<QTreeWidgetItem*> items = treeWidget->findItems(aero, Qt::MatchExactly | Qt::MatchRecursive, C_AERO);
	if(items.length() > 0){
		treeWidget->setCurrentItem(items[0]);
		treeWidget->scrollToItem(items[0], QAbstractItemView::PositionAtCenter);
	}
}
QString AircraftWidget::selected_aircraft(){
	QTreeWidgetItem *item = treeWidget->currentItem();
	if(!item or item->text(C_AERO).length() == 0){
		return "";
	}
	return item->text(C_AERO);
}

//=============================================================
// Validate
QString AircraftWidget::validate(){
	if(!treeWidget->currentItem() && !checkBoxUseDefault->isChecked()){
		return QString("Validation failed!");
	}
	return QString();
}


//=============================================================
// Load Airraft To Tree

void AircraftWidget::on_reload_db_cache(){
	treeWidget->model()->removeRows(0, treeWidget->model()->rowCount());

	//* scan Airaft dirs and save in db
        // AeroTools * aeroTool = new AeroTools(this, mainObject);
	aircraftImport->scan_xml_sets();

	load_tree();
}

void AircraftWidget::load_tree(){
	int c =0;

	QString currAero = selected_aircraft();
	treeWidget->setUpdatesEnabled(false);
	treeWidget->model()->removeRows(0, treeWidget->model()->rowCount());
	QString last_dir("");
	QTreeWidgetItem *parentItem;

	int view = tabsView->currentIndex();
	treeWidget->setColumnHidden(C_DIR, view == V_LIST);
	treeWidget->setRootIsDecorated(view == V_NESTED);

	QSqlQuery query("SELECT directory, xml_file, aero, description, fdm, author from aircraft ORDER BY directory, aero ASC", mainObject->db);

		c = 0;
		while(query.next()){

				if(view == V_LIST){
						parentItem = treeWidget->invisibleRootItem();
				}else if(last_dir != query.value(0).toString()){
						parentItem = new XTreeWidgetItem(treeWidget->invisibleRootItem());
						parentItem->setText( C_DIR, query.value(0).toString());
						parentItem->setIcon(C_DIR, QIcon(":/icon/folder"));
						treeWidget->addTopLevelItem(parentItem);
						treeWidget->setFirstItemColumnSpanned(parentItem, true);
						last_dir = query.value(0).toString();
				}

				//directory, xml_file, aero, fdm, description, author
				XTreeWidgetItem *aeroItem = new XTreeWidgetItem(parentItem);
				QString xml_path = QString("%1/%2").arg(query.value(0).toString(), query.value(1).toString());
				aeroItem->setText(C_XML, xml_path);
				aeroItem->setText(C_AERO, query.value(2).toString());
				aeroItem->setIcon(C_AERO, QIcon(":/icon/aircraft"));
				aeroItem->setText(C_DESCRIPTION, query.value(3).toString());
				aeroItem->setText(C_FDM, query.value(4).toString());
				aeroItem->setText(C_AUTHOR, query.value(5).toString());
				c++;
		}
			
		if ( c == 0 ) {
				// try using the backup code
			if (aircraftImport->rows.size())
				outLog("Appears SQL database capability FAILED on your system! Check its installation.");
			else
				outLog("Aircraft database not initialized! Use Aircraft->Import button.");
			
			for (int i = 0; i < aircraftImport->rows.size(); i++) {
				QStringList list = aircraftImport->rows.at(i);
                // list is -
                // 0=aero, 1=directory, 2=xml_file, 3=description, 4=fdm, 5=author
                QString ndir = list.at(1);
                if(view == V_LIST){
                        parentItem = treeWidget->invisibleRootItem();
                }else if(last_dir != ndir){
                        parentItem = new XTreeWidgetItem(treeWidget->invisibleRootItem());
                        parentItem->setText( C_DIR, ndir);
                        parentItem->setIcon(C_DIR, QIcon(":/icon/folder"));
                        treeWidget->addTopLevelItem(parentItem);
                        treeWidget->setFirstItemColumnSpanned(parentItem, true);
                        last_dir = ndir;
                }

                //directory, xml_file, aero, fdm, description, author
                XTreeWidgetItem *aeroItem = new XTreeWidgetItem(parentItem);
                QString xml_path = QString("%1/%2").arg(ndir, list.at(2));
                aeroItem->setText(C_XML, xml_path);
                aeroItem->setText(C_AERO, list.at(0));
                aeroItem->setIcon(C_AERO, QIcon(":/icon/aircraft"));
                aeroItem->setText(C_DESCRIPTION, list.at(3));
                aeroItem->setText(C_FDM, list.at(4));
                aeroItem->setText(C_AUTHOR, list.at(5));
                c++;
            }
		}
        

	 treeWidget->sortByColumn(view == V_NESTED ? C_DIR : C_AERO, Qt::AscendingOrder);
	 treeWidget->setUpdatesEnabled(true);

	 select_node(currAero);
	 QString str = QString("%1 aircraft").arg(c);
	 statusBarAero->showMessage(str);
         outLog("load_tree: with "+str);
}

//=============================================================
// Initialize
void AircraftWidget::initialize(){
	if(1 == 0){
		//TODO assume its never been initialised
		//scan_xml_sets();
	}

	load_tree();
	select_node(mainObject->settings->value("aircraft").toString());
}

//=============================================================
// Get Args
QStringList AircraftWidget::get_args(){

	QStringList args;

	if(checkBoxUseDefault->isChecked()){
			// this is default aricraft, also set by fgfs without --aircraft= command line option
			// set hard here, to prepare FGx to have its own default in any case
			// see also emit ("remove" ...) in case there is a list but no selection
			args << QString("--aircraft=c172p");
	}

	if(txtNav1->text().length() > 0){
		args << QString("--nav1=%1").arg(txtNav1->text());
	}
	if(txtNav2->text().length() > 0){
		args << QString("--nav2=%1").arg(txtNav2->text());
	}
	if(txtAdf->text().length() > 0){
		args << QString("--adf=%1").arg(txtAdf->text());
	}
	if(txtComm1->text().length() > 0){
		args << QString("--com1=%1").arg(txtComm1->text());
	}
	if(txtComm2->text().length() > 0){
		args << QString("--com2=%1").arg(txtComm2->text());
	}

	return args;

}


void AircraftWidget::on_use_default_clicked(){
	treeWidget->setEnabled( !checkBoxUseDefault->isChecked() );
}
