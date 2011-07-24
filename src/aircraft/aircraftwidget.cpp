
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

#include <QDesktopServices>
#include <QFileDialog>

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
	treeTopBar->setContentsMargins(5,5,5,5);
	treeTopBar->setSpacing(5);
	treeLayout->addLayout(treeTopBar);

	groupUseAircraft = new QButtonGroup(this);
	groupUseAircraft->setExclusive(true);
	connect(groupUseAircraft, SIGNAL(buttonClicked(int)), this, SLOT(on_set_aircraft()) );

	//= Use default
	QRadioButton *radioUseDefault = new QRadioButton();
	radioUseDefault->setText("Use default aircraft");
	treeTopBar->addWidget(radioUseDefault);
	groupUseAircraft->addButton(radioUseDefault, 0);

	//= Use Selected
	QRadioButton *radioUseTree = new QRadioButton();
	radioUseTree->setText("Selected below");
	treeTopBar->addWidget(radioUseTree);
	groupUseAircraft->addButton(radioUseTree, 1);


	//= Use Custom
	QRadioButton *radioUseCustom = new QRadioButton();
	radioUseCustom->setText("Selected Path");
	treeTopBar->addWidget(radioUseCustom);
	groupUseAircraft->addButton(radioUseCustom, 2);
	
	txtAircraftPath = new QLineEdit();
	treeTopBar->addWidget(txtAircraftPath);

	buttSelectPath = new QToolButton();
	buttSelectPath->setText("Select");
	treeTopBar->addWidget(buttSelectPath);
	connect(buttSelectPath, SIGNAL(clicked()), this, SLOT(on_select_path()));



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
	//treeWidget->setColumnHidden(C_XML_SET, true);
	treeWidget->setColumnWidth(C_DIR, 60);
	treeWidget->setColumnWidth(C_FDM, 60);
	treeWidget->setColumnWidth(C_DESCRIPTION, 200);

	connect( treeWidget,
			 SIGNAL( itemSelectionChanged() ),
			 SLOT( on_tree_selection_changed() )
	);


	statusBarTree = new QStatusBar();
	treeLayout->addWidget(statusBarTree);

	//== Path label
	labelAeroPath = new QLabel();
	labelAeroPath->setStyleSheet("font-size: 8pt;");
	//labelAeroPath->setText("0000000000000000000000000000000000000000");
	labelAeroPath->setFixedWidth(250);
	statusBarTree->addPermanentWidget(labelAeroPath);

	buttonAeroPath = new QToolButton();
	buttonAeroPath->setIcon(QIcon(":/icon/folder"));
	statusBarTree->addPermanentWidget(buttonAeroPath);

	//== View nested Checkbox
	checkViewNested = new QCheckBox();
	checkViewNested->setText("View folders");
	statusBarTree->addPermanentWidget(checkViewNested);
	connect(checkViewNested, SIGNAL(clicked()), this, SLOT(load_tree()));


	//== Reload aircrafts
	QToolButton *actionReloadCacheDb = new QToolButton(this);
	actionReloadCacheDb->setText("Reload aircrafts");
	actionReloadCacheDb->setIcon(QIcon(":/icon/load"));
	actionReloadCacheDb->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	actionReloadCacheDb->setAutoRaise(true);
	statusBarTree->addPermanentWidget(actionReloadCacheDb);
	connect(actionReloadCacheDb, SIGNAL(clicked()), this, SLOT(on_reload_cache()) );

	
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
	
	// Enable fuel freeze
	rowFuel++;
	checkBoxFuelFreeze = new QCheckBox(this);
	checkBoxFuelFreeze->setText("Enable fuel freeze");
	layoutFuelPane->addWidget(checkBoxFuelFreeze, 0, 1, 1, Qt::AlignLeft);
	connect(checkBoxFuelFreeze, SIGNAL(clicked()), this, SLOT(on_enable_fuel_freeze_clicked()));
	
	//* Tank 1
	rowFuel++;
	layoutFuelPane->addWidget(new QLabel(tr("Tank 1")), rowFuel, 0, 1, 1, Qt::AlignRight);
	txtTank1 = new QLineEdit();
	txtTank1->setValidator(new QDoubleValidator(0, 200, 2, this));
	layoutFuelPane->addWidget(txtTank1,rowFuel, 1, 1, 1);
	connect(txtTank1, SIGNAL(textChanged(QString)), this, SLOT(on_fuel_changed()));
	
	//* Tank 2
	rowFuel++;
	layoutFuelPane->addWidget(new QLabel(tr("Tank 2")), rowFuel, 0, 1, 1, Qt::AlignRight);
	txtTank2 = new QLineEdit();
	txtTank2->setValidator(new QDoubleValidator(0, 200, 2, this));
	layoutFuelPane->addWidget(txtTank2,rowFuel, 1, 1, 1);
	connect(txtTank2, SIGNAL(textChanged(QString)), this, SLOT(on_fuel_changed()));
	
	//* Tank 3
	rowFuel++;
	layoutFuelPane->addWidget(new QLabel(tr("Tank 3")), rowFuel, 0, 1, 1, Qt::AlignRight);
	txtTank3 = new QLineEdit();
	txtTank3->setValidator(new QDoubleValidator(0, 200, 2, this));
	layoutFuelPane->addWidget(txtTank3, rowFuel, 1, 1, 1);
	connect(txtTank3, SIGNAL(textChanged(QString)), this, SLOT(on_fuel_changed()));
	
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
	connect(txtNav1, SIGNAL(textChanged(QString)), this, SLOT(on_navs_changed()));

	row++;
	layoutAeroPane->addWidget(new QLabel(tr("NAV2")), row, 0, 1, 1, Qt::AlignRight);
	txtNav2 = new QLineEdit();
	txtNav2->setValidator(new QDoubleValidator(0, 200, 2, this));
	layoutAeroPane->addWidget(txtNav2,row, 1, 1, 1);
	connect(txtNav2, SIGNAL(textChanged(QString)), this, SLOT(on_navs_changed()));

	//= ADF
	row++;
	layoutAeroPane->addWidget(new QLabel(tr("ADF1")), row, 0, 1, 1, Qt::AlignRight);
	txtAdf1 = new QLineEdit();
	txtAdf1->setValidator(new QDoubleValidator(0, 200,0, this));
	layoutAeroPane->addWidget(txtAdf1, row, 1, 1, 1);
	connect(txtAdf1, SIGNAL(textChanged(QString)), this, SLOT(on_navs_changed()));

	row++;
	layoutAeroPane->addWidget(new QLabel(tr("ADF2")), row, 0, 1, 1, Qt::AlignRight);
	txtAdf2 = new QLineEdit();
	txtAdf2->setValidator(new QDoubleValidator(0, 200,0, this));
	layoutAeroPane->addWidget(txtAdf2, row, 1, 1, 1);
	connect(txtAdf2, SIGNAL(textChanged(QString)), this, SLOT(on_navs_changed()));

	//= Comms
	row++;
	layoutAeroPane->addWidget(new QLabel(tr("COM1")), row, 0, 1, 1, Qt::AlignRight);
	txtCom1 = new QLineEdit();
	txtCom1->setValidator(new QDoubleValidator(0, 200, 2, this));
	layoutAeroPane->addWidget(txtCom1,row, 1, 1, 1);
	connect(txtCom1, SIGNAL(textChanged(QString)), this, SLOT(on_navs_changed()));

	row++;
	layoutAeroPane->addWidget(new QLabel(tr("COM2")), row, 0, 1, 1, Qt::AlignRight);
	txtCom2 = new QLineEdit();
	txtCom2->setValidator(new QDoubleValidator(0, 200, 2, this));
	layoutAeroPane->addWidget(txtCom2,row, 1, 1, 1);
	connect(txtCom2, SIGNAL(textChanged(QString)), this, SLOT(on_navs_changed()));


	layoutAeroPane->setRowStretch(row + 1, 20); // stretch end

	//==================
	//== Setup

	splitter->setCollapsible(0, false);
	splitter->setCollapsible(1, false);
	splitter->setStretchFactor(0, 50);
	splitter->setStretchFactor(1, 1);

	//== Main Settings connection
	connect(this, SIGNAL(setx(QString,bool,QString)), mainObject->X, SLOT(set_option(QString,bool,QString)) );
	connect(mainObject->X, SIGNAL(upx(QString,bool,QString)), this, SLOT(on_upx(QString,bool,QString)));

}



//==========================================================================1
// Aircraft Selected
//==========================================================================
void AircraftWidget::on_tree_selection_changed(){



	QTreeWidgetItem *item = treeWidget->currentItem();
	if(!item){
            outLog("on_tree_selection_changed: no selected item");
		labelAeroPath->setText("");
		buttonAeroPath->setDisabled(true);
		return;
	}

	//= Check there is item and its a model
	if(item->text(C_AERO).length() == 0){
                outLog("on_tree_selection_changed: no C_AERO item");
                aeroImageLabel->clear();
		emit setx("--aircraft=", false, "");
		labelAeroPath->setText("");
		buttonAeroPath->setDisabled(true);
		return;
	}

	//qDebug() << item->text(C_DIR);
	labelAeroPath->setText(mainObject->X->aircraft_path() + "/" + item->text(C_AERO));
	buttonAeroPath->setDisabled(false);

	//= Get the thumbnail image
	QString thumb_file = QString("%1/%2/%3/thumbnail.jpg").arg( mainObject->X->aircraft_path(),
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
	emit setx("--aircraft=", true, selected_aircraft());
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
	if(groupUseAircraft->checkedId() == 1 &&  !treeWidget->currentItem()){
		return QString("Aircraft: No aircraft selected or check [x] Use default");
		outLog("FGx reports: AircraftWidget::validate() No aircraft selected (maybe no list), and [x] use default not selected. ***");
	}
	return QString("");
}


//=============================================================
// Rescan airpcarft cache
void AircraftWidget::on_reload_cache(){
	treeWidget->model()->removeRows(0, treeWidget->model()->rowCount());
	statusBarTree->showMessage("Reloading cache");
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
	if (checkViewNested->isChecked()) {
		view = 1;
	}
	
	treeWidget->setColumnHidden(C_DIR, view == LIST_VIEW);
	treeWidget->setRootIsDecorated(view == FOLDER_VIEW);

	QFile dataFile(mainObject->data_file(("aircraft.txt")));
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

	select_node(mainObject->X->getx("--aircraft="));
	QString str = QString("%1 aircraft(s)").arg(c);
	statusBarTree->showMessage(str);
	outLog("*** FGx: AircraftWidget::load_tree: with " + str);
}

//=============================================================
// Initialize
void AircraftWidget::initialize(){

	static bool first_load_done = false;
	if(first_load_done){
		return;
	}
	if (!QFile::exists(mainObject->data_file("aircraft.txt"))){
		statusBarTree->showMessage("*** FGx: No cached data. Use set paths and reload");
	}else{
		load_tree();
	}
	first_load_done = true;
}




void AircraftWidget::on_navs_changed()
{
	emit setx("--nav1=", true, txtNav1->text());
	emit setx("--nav2=", true, txtNav2->text());
	emit setx("--adf1=", true, txtAdf1->text());
	emit setx("--adf2=", true, txtAdf2->text());
	emit setx("--com1=", true, txtCom1->text());
	emit setx("--com2=", true, txtCom2->text());

}

void AircraftWidget::on_enable_fuel_freeze_clicked(){
	emit setx("--enable-fuel-freeze", checkBoxFuelFreeze->isChecked(), "");
}

void AircraftWidget::on_fuel_changed()
{
	emit setx("--prop:/consumables/fuels/tank[1]/level-gal=", true, txtTank1->text());
	emit setx("--prop:/consumables/fuels/tank[2]/level-gal=", true, txtTank2->text());
	emit setx("--prop:/consumables/fuels/tank[3]/level-gal=", true, txtTank3->text());
	
}

void AircraftWidget::on_set_aircraft()
{

	emit setx("use_aircraft", true, QString::number(groupUseAircraft->checkedId()));

	emit setx("--fg-aircraft=",
			  groupUseAircraft->checkedId() == 2 && txtAircraftPath->text().length() > 0,
			  txtAircraftPath->text()
			  );

}



//=====================================================
void AircraftWidget::on_upx( QString option, bool enabled, QString value)
{
	Q_UNUSED(enabled);
	//= NOTE: The --aircraft=. --runway, etc is detected as the tree loads from cache

	if(option == "use_aircraft"){

		int bid = value.toInt();
		groupUseAircraft->button(bid)->setChecked(true);
		treeWidget->setEnabled(bid == 1);
		txtAircraftPath->setEnabled(bid == 2);
		buttSelectPath->setEnabled(bid == 2);

	}else if(option == "--aircraft"){
		//= see tree load
		select_node(value);

	}else if(option == "--fg-aircraft="){

		txtAircraftPath->setText(value);

	//== tab radio
	}else if(option == "--nav1="){
		txtNav1->setText(value);

	}else if(option == "--nav2="){
		txtNav2->setText(value);


	}else if(option == "--adf1="){
		txtAdf1->setText(value);

	}else if(option == "--adf2="){
		txtAdf2->setText(value);


	}else if(option == "--com1="){
		txtCom1->setText(value);

	}else if(option == "--com2="){
		txtCom2->setText(value);


	//=== tab fuel
		
	}else if(option == "use_default_fuel"){
		checkBoxUseDefaultFuel->setChecked(enabled);
		
	}else if(option == "--enable-fuel-freeze"){
		checkBoxFuelFreeze->setChecked(enabled);
	
	}else if(option == "--prop:/consumables/fuels/tank[1]/level-gal="){
		txtTank1->setText(value);
		
	}else if(option == "--prop:/consumables/fuels/tank[2]/level-gal="){
		txtTank2->setText(value);
		
	}else if(option == "--prop:/consumables/fuels/tank[3]/level-gal="){
		txtTank3->setText(value);
	}
}


void AircraftWidget::on_select_path()
{
	QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select Aircraft directory"),
														 txtAircraftPath->text(), QFileDialog::ShowDirsOnly);
	if(dirPath.length() > 0){
		txtAircraftPath->setText(dirPath);
		on_set_aircraft();
	}

}


void AircraftWidget::on_open_aircraft_path()
{
	if(labelAeroPath->text().length() == 0){
		return;
	}
	QUrl url(QString("file://%1").arg(labelAeroPath->text()));
	QDesktopServices::openUrl(url);
}
