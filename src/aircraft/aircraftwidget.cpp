
/**
	Started apr 2011 by: pete at freeflightsim.org
	then ..

	TODO: Convert QTreeWidget to QTreeView with A filterSortProxy Model

*/



//#include <QtCore/QDebug>

#include <QtCore/QProcess>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QFile>


#include <QtXmlPatterns/QXmlQuery>
#include <QtXml/QDomDocument>


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
#include "aircraft/aircraftdata.h"
#include "xwidgets/xtreewidgetitem.h"
#include "utilities/utilities.h"


AircraftWidget::AircraftWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mOb;

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
	checkBoxUseDefault->setText("Use default fuel load");
	treeTopBar->addWidget(checkBoxUseDefault);
	connect(checkBoxUseDefault, SIGNAL(clicked()), this, SLOT(on_use_default_clicked()));

	treeTopBar->addSpacing(10);

	
	//** View nested Checkbox
	tabsView = new QCheckBox();
	tabsView->setText("View nested");
	treeTopBar->addWidget(tabsView);
	connect(tabsView, SIGNAL(clicked()), this, SLOT(load_tree()));


	treeTopBar->addStretch(20);

	//** Reload aircrafts
	QToolButton *actionReloadCacheDb = new QToolButton(this);
	actionReloadCacheDb->setText("Reload aircrafts");
	actionReloadCacheDb->setIcon(QIcon(":/icon/load"));
	actionReloadCacheDb->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	actionReloadCacheDb->setAutoRaise(true);
	treeTopBar->addWidget(actionReloadCacheDb);
	connect(actionReloadCacheDb, SIGNAL(clicked()), this, SLOT(on_reload_cache()) );

	//===============================================================
	//= Aircraft Tree
	treeWidget = new QTreeWidget(this);
	treeLayout->addWidget(treeWidget);
	treeWidget->setAlternatingRowColors(true);
	treeWidget->setRootIsDecorated(true);
	treeWidget->setUniformRowHeights(true);
	treeWidget->setSortingEnabled(true);	
	treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	treeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

	treeWidget->headerItem()->setText(C_DIR, "Dir");
	treeWidget->headerItem()->setText(C_XML_SET, "xml");
	treeWidget->headerItem()->setText(C_AERO, "Aircraft");
	treeWidget->headerItem()->setText(C_DESCRIPTION, "Description");
	treeWidget->headerItem()->setText(C_FDM, "FDM");
	treeWidget->headerItem()->setText(C_AUTHOR, "Author");
	treeWidget->header()->setStretchLastSection(true);
	treeWidget->setColumnHidden(C_XML_SET, true);
	treeWidget->setColumnWidth(C_DIR, 60);
	treeWidget->setColumnWidth(C_FDM, 60);
	treeWidget->setColumnWidth(C_DESCRIPTION, 200);

	connect( treeWidget,
			 SIGNAL( itemSelectionChanged() ),
			 SLOT( on_tree_selection_changed() )
	);

	statusBarAero = new QStatusBar();
	treeLayout->addWidget(statusBarAero);


	//================================================================================================
	//= Right
	//================================================================================================


    QGroupBox *grpAero = new QGroupBox();
    splitter->addWidget(grpAero);
    grpAero->setDisabled(false);
    grpAero->setTitle(tr("Preview, Radio, Fuel"));


    QVBoxLayout *aeroLayout = new QVBoxLayout();
    grpAero->setLayout(aeroLayout);

    //**********************************************8
	//** Aero Panel
	QGroupBox *grpAeroPanel = new QGroupBox();
	grpAeroPanel->setStyleSheet("background-image: url(:/artwork/background-gray-tabbar); border-top: 0px;");
	aeroLayout->addWidget(grpAeroPanel);


	QVBoxLayout *aeroPanelLayout = new QVBoxLayout();
	grpAeroPanel->setLayout(aeroPanelLayout);

	QHBoxLayout *imgBox = new QHBoxLayout();
	aeroPanelLayout->addLayout(imgBox);

	aeroImageLabel =  new QLabel(this);
	aeroImageLabel->setFixedWidth(171);
	aeroImageLabel->setFixedHeight(128);
	imgBox->addWidget(aeroImageLabel);


    //******************************************************8
   //** Tab Widgets
    QTabWidget *aeroTabs = new QTabWidget();
    aeroLayout->addWidget(aeroTabs, 120);

    QWidget *aeroControlWidget = new QWidget();
	QWidget *fuelWidget = new QWidget();
	aeroTabs->addTab(aeroControlWidget, tr("Radio"));
	aeroTabs->addTab(fuelWidget, tr("Fuel"));
	
	//** Fuel Widget
	QGridLayout *layoutFuelPane = new QGridLayout();
	fuelWidget->setLayout(layoutFuelPane);
	int rowFuel = 1;
	
	//** Use Default Aircraft
	checkBoxUseDefaultFuel = new QCheckBox(this);
	checkBoxUseDefaultFuel->setText("Use default");
	layoutFuelPane->addWidget(checkBoxUseDefaultFuel);
	connect(checkBoxUseDefaultFuel, SIGNAL(clicked()), this, SLOT(on_use_default_fuel_clicked()));
	
	// Enable fuel freeze
	rowFuel++;
	checkBoxFuelFreeze = new QCheckBox(this);
	checkBoxFuelFreeze->setText("Enable fuel freeze");
	layoutFuelPane->addWidget(checkBoxFuelFreeze);
	connect(checkBoxFuelFreeze, SIGNAL(clicked()), this, SLOT(on_fuel_freeze_clicked()));
	
	//* Tank 1
	rowFuel++;
	layoutFuelPane->addWidget(new QLabel(tr("Tank 1")), rowFuel, 0, 1, 1, Qt::AlignLeft);
	txtTank1 = new QLineEdit();
	txtTank1->setValidator(new QDoubleValidator(0, 200, 1, this));
	layoutFuelPane->addWidget(txtTank1,rowFuel, 1, 1, 1, Qt::AlignLeft);
	
	//* Tank 2
	rowFuel++;
	layoutFuelPane->addWidget(new QLabel(tr("Tank 2")), rowFuel, 0, 1, 1, Qt::AlignLeft);
	txtTank2 = new QLineEdit();
	txtTank2->setValidator(new QDoubleValidator(0, 200, 1, this));
	layoutFuelPane->addWidget(txtTank2,rowFuel, 1, 1, 1, Qt::AlignLeft);
	
	//* Tank 3
	rowFuel++;
	layoutFuelPane->addWidget(new QLabel(tr("Tank 3")), rowFuel, 0, 1, 1, Qt::AlignLeft);
	txtTank3 = new QLineEdit();
	txtTank3->setValidator(new QDoubleValidator(0, 200, 1, this));
	layoutFuelPane->addWidget(txtTank3, rowFuel, 1, 1, 1, Qt::AlignLeft);
	
	layoutFuelPane->setRowStretch(rowFuel + 1, 20); // stretch end
	
	
	//** Aero Control Widget
	QGridLayout *layoutAeroPane = new QGridLayout();
	aeroControlWidget->setLayout(layoutAeroPane);
	int row = 1;

	//* Navs
	layoutAeroPane->addWidget(new QLabel(tr("NAV1")), row, 0, 1, 1, Qt::AlignRight);
	txtNav1 = new QLineEdit();
	txtNav1->setValidator(new QDoubleValidator(0, 200, 2, this));
	layoutAeroPane->addWidget(txtNav1,row, 1, 1, 1);

	row++;
	layoutAeroPane->addWidget(new QLabel(tr("NAV2")), row, 0, 1, 1, Qt::AlignRight);
	txtNav2 = new QLineEdit();
	txtNav2->setValidator(new QDoubleValidator(0, 200, 2, this));
	layoutAeroPane->addWidget(txtNav2,row, 1, 1, 1);

	//* ADF
	row++;
	layoutAeroPane->addWidget(new QLabel(tr("ADF")), row, 0, 1, 1, Qt::AlignRight);
	txtAdf = new QLineEdit();
	txtAdf->setValidator(new QDoubleValidator(0, 200,0, this));
	layoutAeroPane->addWidget(txtAdf, row, 1, 1, 1);

	//* Comms
	row++;
	layoutAeroPane->addWidget(new QLabel(tr("COM1")), row, 0, 1, 1, Qt::AlignRight);
	txtComm1 = new QLineEdit();
	txtComm1->setValidator(new QDoubleValidator(0, 200, 2, this));
	layoutAeroPane->addWidget(txtComm1,row, 1, 1, 1);

	row++;
	layoutAeroPane->addWidget(new QLabel(tr("COM2")), row, 0, 1, 1, Qt::AlignRight);
	txtComm2 = new QLineEdit();
	txtComm2->setValidator(new QDoubleValidator(0, 200, 2, this));
	layoutAeroPane->addWidget(txtComm2,row, 1, 1, 1);

	layoutAeroPane->setRowStretch(row + 1, 20); // stretch end

	//==================
	//== Setup

	splitter->setCollapsible(0, false);
	splitter->setCollapsible(1, false);
	splitter->setStretchFactor(0, 50);
	splitter->setStretchFactor(1, 1);

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

	//= Check there is item and its a model
	if(item->text(C_AERO).length() == 0){
                outLog("on_tree_selection_changed: no C_AERO item");
                aeroImageLabel->clear();
		emit set_arg("update", "--aircraft=", "");
		return;
	}

	mainObject->settings->setValue("aircraft", item->text(C_AERO) );

	//= Get the thumbnail image
	QString thumb_file = QString("%1/%2/%3/thumbnail.jpg").arg( mainObject->settings->aircraft_path(),
                                                                    item->text(C_DIR),
                                                                    item->text(C_AERO));
	if(QFile::exists(thumb_file)){
		QPixmap aeroImage(thumb_file);
		if(!aeroImage.isNull()){
			outLog("on_tree_selection_changed: Loaded thumb "+thumb_file);
			aeroImageLabel->setText("");
			aeroImageLabel->setPixmap(aeroImage);

		} else{
			outLog("on_tree_selection_changed: NO thumb load "+thumb_file);
			aeroImageLabel->clear();
			aeroImageLabel->setText("No Image");
		}
	}else{
		aeroImageLabel->setText("No Image");
	}
}







//=============================================================
// Save Settings
void AircraftWidget::save_settings(){
	QTreeWidgetItem *item = treeWidget->currentItem();
	if(item && item->text(C_AERO).length() > 0){
		mainObject->settings->setValue("aircraft", item->text(C_AERO) );
	}
	mainObject->settings->setValue("use_default_fuel", checkBoxUseDefaultFuel->isChecked());
	mainObject->settings->setValue("enable_fuel_freeze", checkBoxFuelFreeze->isChecked());

	mainObject->settings->setValue("nav1", txtNav1->text());
	mainObject->settings->setValue("nav2", txtNav2->text());
	mainObject->settings->setValue("adf", txtAdf->text());
	mainObject->settings->setValue("com1", txtComm1->text());
	mainObject->settings->setValue("com2", txtComm2->text());
	
	mainObject->settings->setValue("aircraft_use_default", checkBoxUseDefault->isChecked());
	
	mainObject->settings->setValue("prop:/consumables/fuels/tank[1]/level-gal", txtTank1->text());
	mainObject->settings->setValue("prop:/consumables/fuels/tank[2]/level-gal", txtTank2->text());
	mainObject->settings->setValue("prop:/consumables/fuels/tank[3]/level-gal", txtTank3->text());

	mainObject->settings->sync();
}


//=============================================================
// Load Settings
void AircraftWidget::load_settings(){

	//select_node(mainObject->settings->value("aircraft").toString());
	
	checkBoxUseDefaultFuel->setChecked(mainObject->settings->value("use_default_fuel", false).toBool());
	checkBoxFuelFreeze->setChecked(mainObject->settings->value("enable_fuel_freeze", false).toBool());
	txtTank1->setText(mainObject->settings->value("prop:/consumables/fuels/tank[1]/level-gal").toString());
	txtTank2->setText(mainObject->settings->value("prop:/consumables/fuels/tank[2]/level-gal").toString());
	txtTank3->setText(mainObject->settings->value("prop:/consumables/fuels/tank[3]/level-gal").toString());

	txtNav1->setText(mainObject->settings->value("nav1").toString());
	txtNav2->setText(mainObject->settings->value("nav2").toString());
	txtAdf->setText(mainObject->settings->value("adf").toString());
	txtComm1->setText(mainObject->settings->value("com1").toString());
	txtComm2->setText(mainObject->settings->value("com2").toString());
	checkBoxUseDefault->setChecked(mainObject->settings->value("aircraft_use_default", false).toBool());
	on_use_default_clicked();
}

//==============================
//== Select an aircraft in tree
void AircraftWidget::select_node(QString aero){
	if(aero.length() == 0){
		return;
	}
	QList<QTreeWidgetItem*> items = treeWidget->findItems(aero, Qt::MatchExactly | Qt::MatchRecursive, C_AERO);
	if(items.length() > 0){
		treeWidget->setCurrentItem(items[0]);
		treeWidget->scrollToItem(items[0], QAbstractItemView::PositionAtCenter);

	}
}
//==============================
//== return selected Aircraft
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
		return QString("Aircraft: No aircraft selected or check [x] Use default");
		outLog("FGx reports: AircraftWidget::validate() No aircraft selected (maybe no list), and [x] use default not selected. ***");
	}
	return QString("");
}


//=============================================================
// Rescan airpcarft cache
void AircraftWidget::on_reload_cache(){
	treeWidget->model()->removeRows(0, treeWidget->model()->rowCount());
	statusBarAero->showMessage("Reloading cache");
	QProgressDialog progress(this);
	QSize size(320,100);
	progress.resize(size);
	progress.setWindowIcon(QIcon(":/icon/load"));

	AircraftData::import(progress, mainObject);
	load_tree();
}

//=============================================================
// Load Aircaft To Tree
void AircraftWidget::load_tree(){
	int c =0;

	treeWidget->setUpdatesEnabled(false);
	treeWidget->model()->removeRows(0, treeWidget->model()->rowCount());

	QTreeWidgetItem *parentItem = new QTreeWidgetItem();
	
	int view = 0;
	if (tabsView->isChecked()) {
		view = 1;
	}
	
	treeWidget->setColumnHidden(C_DIR, view == LIST_VIEW);
	treeWidget->setRootIsDecorated(view == FOLDER_VIEW);

	QFile dataFile(mainObject->settings->data_file(("aircraft.txt")));
	if (!dataFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		   return;
	}
	QTextStream in(&dataFile);
	QString line = in.readLine();
	QString last_dir("");
	line = in.readLine();
	c = 0;
	while(!line.isNull()){

		QStringList cols = line.split("\t");
		//qDebug() << cols;
		/* The parent is invisible root if a list,
			otherwise folder nodes are created
		*/
		if(view == LIST_VIEW){
				parentItem = treeWidget->invisibleRootItem();
		}else if(last_dir != cols.at(C_DIR)){
				parentItem = new XTreeWidgetItem(treeWidget->invisibleRootItem());
				parentItem->setText( C_DIR,cols.at(C_DIR));
				parentItem->setIcon(C_DIR, QIcon(":/icon/folder"));
				treeWidget->addTopLevelItem(parentItem);
				treeWidget->setFirstItemColumnSpanned(parentItem, true);
				last_dir = cols.at(C_DIR);
		}

		XTreeWidgetItem *aeroItem = new XTreeWidgetItem(parentItem);
		QString xml_path = QString("%1/%2").arg(cols.at(C_DIR)).arg(C_XML_SET);
		aeroItem->setText(C_XML_SET, xml_path);
		aeroItem->setText(C_AERO, cols.at(C_AERO));
		aeroItem->setIcon(C_AERO, QIcon(":/icon/aircraft"));
		aeroItem->setText(C_DESCRIPTION, cols.at(C_DESCRIPTION));
		aeroItem->setText(C_FDM, cols.at(C_FDM));
		aeroItem->setText(C_AUTHOR, cols.at(C_AUTHOR));
		c++;
		line = in.readLine();
	}

	treeWidget->sortByColumn(view == FOLDER_VIEW ? C_DIR : C_AERO, Qt::AscendingOrder);
	treeWidget->setUpdatesEnabled(true);

	select_node(mainObject->settings->value("aircraft").toString());
	QString str = QString("%1 aircraft").arg(c);
	statusBarAero->showMessage(str);
	outLog("FGx: AircraftWidget::load_tree: with " + str);
}

//=============================================================
// Initialize
void AircraftWidget::initialize(){

	static bool first_load_done = false;
	if(first_load_done){
		return;
	}
	if (!QFile::exists(mainObject->settings->data_file("aircraft.txt"))){
		statusBarAero->showMessage("No cached data. Click Import");
	}else{
		load_tree();
	}
	first_load_done = true;
}




void AircraftWidget::on_use_default_clicked(){
	treeWidget->setEnabled( !checkBoxUseDefault->isChecked() );
}

void AircraftWidget::on_use_default_fuel_clicked(){
	txtTank1->setEnabled( !checkBoxUseDefaultFuel->isChecked() );
	txtTank2->setEnabled( !checkBoxUseDefaultFuel->isChecked() );
	txtTank3->setEnabled( !checkBoxUseDefaultFuel->isChecked() );
}

void AircraftWidget::on_enable_fuel_freeze_clicked(){
	
}
