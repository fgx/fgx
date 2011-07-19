
//#include <QtDebug>
#include <QtCore/QString>


#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QtXml/QXmlStreamReader>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>


#include <QtGui/QMessageBox>
#include <QtGui/QSizePolicy>
#include <QtGui/QFont>

#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QSplitter>

#include <QtGui/QToolBar>
#include <QtGui/QToolButton>
#include <QtGui/QPushButton>
#include <QtGui/QAction>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

#include <QtGui/QStandardItemModel>
#include <QtGui/QItemSelection>
#include <QtGui/QItemSelectionModel>
#include <QtGui/QAbstractItemView>
#include <QtGui/QHeaderView>

#include <QtGui/QProgressDialog>



#include "airports/airportswidget.h"
#include "airports/airportsdata.h"
#include "airports/importairportsdialog.h"


AirportsWidget::AirportsWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mOb;

    //* Main Layout
	QHBoxLayout *mainLayout = new QHBoxLayout();
    setLayout(mainLayout);
	mainLayout->setSpacing(10);
	int m = 10;
	mainLayout->setContentsMargins(m,m,m,m);


	//= Tab Widget
	tabWidget = new QTabWidget();
	mainLayout->addWidget(tabWidget);


	//=====================================
	//** Top Startup Option Buttons
	/*
	QHBoxLayout *layoutTop = new QHBoxLayout();
	mainLayout->addLayout(layoutTop,0,0,1,2);

	buttonGroupUse = new QButtonGroup(this);
	buttonGroupUse->setExclusive(true);
	connect(buttonGroupUse, SIGNAL(buttonClicked(int)), this, SLOT(on_buttonGroupUse()));

	QRadioButton *radioButtonUseDefault = new QRadioButton(tr("Default (KSFO)"));
	radioButtonUseDefault->setChecked(true);
	layoutTop->addWidget(radioButtonUseDefault);
	buttonGroupUse->addButton(radioButtonUseDefault, USE_DEFAULT);

	QRadioButton *radioButtonUseAirport = new QRadioButton(tr("Start at Airport"));
	layoutTop->addWidget(radioButtonUseAirport);
	buttonGroupUse->addButton(radioButtonUseAirport, USE_AIRPORT);

	QRadioButton *radioButtonUseCoordinates = new QRadioButton(tr("Start at Coordinates"));
	layoutTop->addWidget(radioButtonUseCoordinates);
	buttonGroupUse->addButton(radioButtonUseCoordinates, USE_COORDINATES);

	layoutTop->addStretch(10);
	*/

	//=================================================================================
	//* Airport Details
	groupBoxAirport = new XGroupVBox("Airport Details");
	tabWidget->addTab(groupBoxAirport, QIcon(":/icon/XXX"), tr("Start At Aiport"));

	groupBoxAirport->xLayout->setContentsMargins(10,10,10,10);
	groupBoxAirport->xLayout->setSpacing(0);


	//==================================================================
	//** Airports Filter and Actions Top Bar
	QHBoxLayout *layoutAptTopBar = new QHBoxLayout();
	layoutAptTopBar->setContentsMargins(0,0,0,0);
	layoutAptTopBar->setSpacing(10);
	groupBoxAirport->addLayout(layoutAptTopBar);

    //** Filter Code
	layoutAptTopBar->addWidget(new QLabel(tr("Filter").append(":")));

	//** Filter Buttons - TODO
	buttonGroupFilter = new QButtonGroup(this);
	buttonGroupFilter->setExclusive(true);


	//** Aiport Code Filter
    QRadioButton *buttCode = new QRadioButton();
    buttCode->setText("Code");
	buttCode->setProperty("filter_column_idx", QVariant(CA_CODE));
	buttCode->setChecked(true);
	layoutAptTopBar->addWidget(buttCode);
	buttonGroupFilter->addButton(buttCode, 0);

	//** Airport Name Filter
    QRadioButton *buttName = new QRadioButton();
    buttName->setText("Name");
	buttName->setProperty("filter_column_idx", QVariant(CA_NAME));
	layoutAptTopBar->addWidget(buttName);
	buttonGroupFilter->addButton(buttName, 1);

	//** Find Airport Text
    txtAirportsFilter = new QLineEdit();
	layoutAptTopBar->addWidget(txtAirportsFilter);
    txtAirportsFilter->setFixedWidth(100);
	connect(	txtAirportsFilter, SIGNAL(textChanged(QString)),
				this, SLOT(on_update_airports_filter())
	);

	layoutAptTopBar->addStretch(20);

	/*
	buttonViewMap = new QToolButton(this);
	layoutAptTopBar->addWidget(buttonViewMap);
	buttonViewMap->setText("View Map");
	buttonViewMap->setToolTip("View airport in map");
	buttonViewMap->setIcon(QIcon(":/icon/mpmap"));
	buttonViewMap->setAutoRaise(true);
	buttonViewMap->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(buttonViewMap, SIGNAL(clicked()), this, SLOT(on_view_map()) );
	//buttonViewMap->setVisible(false); // NOt to confuse anyone
	*/

	QToolButton *buttonReloadCache = new QToolButton(this);
	layoutAptTopBar->addWidget(buttonReloadCache);
	buttonReloadCache->setText("Reload airports");
	buttonReloadCache->setToolTip("Scan directories and reload cache");
	buttonReloadCache->setIcon(QIcon(":/icon/load"));
	buttonReloadCache->setAutoRaise(true);
	buttonReloadCache->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(buttonReloadCache, SIGNAL(clicked()), this, SLOT(on_reload_cache()) );


	//*==============================================================================
	//** Airports/Runways Splitter
	QSplitter *splitter = new QSplitter(this);
	groupBoxAirport->addWidget(splitter);


	//=== Airport Widget + Layout
	QWidget *airportWidget = new QWidget();
	splitter->addWidget(airportWidget);
	QVBoxLayout *airportsLayout = new QVBoxLayout();
	airportsLayout->setContentsMargins(0,0,0,0);
	airportsLayout->setSpacing(0);
	airportWidget->setLayout(airportsLayout);

	/*  ======== NOTE ============
		# The Airports tree is a treeView
			## with a model/proxt to enable filtersing
			## and the columns constants a CA_
		# The Airport Info is a treeWidget
			## uses CI_ as the column constants
	*/

	//==============================================
	//** Airport Models
    model = new QStandardItemModel(this);
	model->setColumnCount(2);
    QStringList headerLabelsList;
	headerLabelsList << tr("Code") << tr("Name") << "Dir";
    model->setHorizontalHeaderLabels(headerLabelsList);

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
	proxyModel->setFilterKeyColumn( CA_NAME );

	//==============================================
	//**  TreeView aiports
	treeViewAirports = new QTreeView(this);
	airportsLayout->addWidget(treeViewAirports, 1, 0);
	treeViewAirports->setModel(proxyModel);

	treeViewAirports->setUniformRowHeights(true);
	treeViewAirports->setAlternatingRowColors(true);
	treeViewAirports->setRootIsDecorated(false);
	treeViewAirports->setSortingEnabled(true);
	treeViewAirports->sortByColumn(CA_NAME, Qt::AscendingOrder);
	treeViewAirports->setSelectionMode(QAbstractItemView::SingleSelection);
	treeViewAirports->setSelectionBehavior(QAbstractItemView::SelectRows);
	treeViewAirports->setEditTriggers(QAbstractItemView::NoEditTriggers);

	//* Headers and columns
	treeViewAirports->header()->setStretchLastSection(true);
	treeViewAirports->setColumnWidth(CA_CODE, 80);
	treeViewAirports->setColumnWidth(CA_NAME, 50);
	treeViewAirports->setColumnHidden(CA_DIR, true);

	connect( treeViewAirports->selectionModel(),
			 SIGNAL( currentChanged(QModelIndex,QModelIndex)),
			 this, SLOT( on_airport_tree_selected(QModelIndex, QModelIndex) )
	);

	//* StatusBar for Airports
	statusBarAirports = new QStatusBar();
	statusBarAirports->setSizeGripEnabled(false);
	airportsLayout->addWidget(statusBarAirports);
	statusBarAirports->showMessage("Idle");


	//========================================================
	//** Airport Info Widget
	QWidget *airportInfoWidget = new QWidget();
	splitter->addWidget(airportInfoWidget);
	QVBoxLayout *airportInfoLayout = new QVBoxLayout();
	airportInfoLayout->setContentsMargins(0,0,0,0);
	airportInfoLayout->setSpacing(0);
	airportInfoWidget->setLayout(airportInfoLayout);

	//=========================================================
	//* Airport Info TreeWidget
	treeWidgetAirportInfo = new QTreeWidget();
	airportInfoLayout->addWidget(treeWidgetAirportInfo, 3);
	treeWidgetAirportInfo->setAlternatingRowColors(true);
	treeWidgetAirportInfo->setRootIsDecorated(true);
	treeWidgetAirportInfo->setUniformRowHeights(true);

	QTreeWidgetItem *headerItem = treeWidgetAirportInfo->headerItem();
	headerItem->setText(CI_NODE, tr(""));
	headerItem->setText(CI_WIDTH, tr("Width"));
	headerItem->setText(CI_LENGTH, tr("Length"));
	headerItem->setText(CI_LAT, tr("Lat"));
	headerItem->setText(CI_LON, tr("Lng"));
	headerItem->setText(CI_ALIGNMNET, tr("Alignment"));
	headerItem->setText(CI_RUNWAYS, tr("R"));
	treeWidgetAirportInfo->setColumnHidden(CI_TYPE,true);
	treeWidgetAirportInfo->setColumnHidden(CI_LABEL,true);
	treeWidgetAirportInfo->setColumnHidden(CI_SETTING_KEY,true);
	treeWidgetAirportInfo->setColumnHidden(CI_WIDTH,true);
	treeWidgetAirportInfo->setColumnHidden(CI_LENGTH,true);
	treeWidgetAirportInfo->setColumnHidden(CI_ALIGNMNET,true);
	treeWidgetAirportInfo->setColumnHidden(CI_RUNWAYS, true);


	treeWidgetAirportInfo->setColumnWidth(CI_NODE, 120);
	treeWidgetAirportInfo->header()->setStretchLastSection(true);

	connect(treeWidgetAirportInfo, SIGNAL(itemSelectionChanged()), this, SLOT(on_airport_info_selection_changed()));


	statusBarAirportInfo = new QStatusBar();
	statusBarAirportInfo->setSizeGripEnabled(false);
	airportInfoLayout->addWidget(statusBarAirportInfo);
	statusBarAirportInfo->showMessage("");


	splitter->setStretchFactor(0, 2);
	splitter->setStretchFactor(1, 2);



	//========================================================================================================
	//** Use Coordinates
	//====================================================================

	groupBoxUseCoordinates = new XGroupVBox("Coordinates", this);
	groupBoxUseCoordinates->setMaximumWidth(300);
	tabWidget->addTab(groupBoxUseCoordinates, QIcon(":/icon/XXX"), "Start At Position");

	int space = 5;

	groupBoxUseCoordinates->addWidget(new QLabel(tr("Latitude (negative is west)")));
	txtLat = new QLineEdit();
	groupBoxUseCoordinates->addWidget(txtLat);
	connect(txtLat, SIGNAL(textChanged(QString)), this, SLOT(on_coordinates_changed()));

	groupBoxUseCoordinates->xLayout->addSpacing(space);
	groupBoxUseCoordinates->addWidget(new QLabel(tr("Longtitude (negative is south)")));
	txtLon = new QLineEdit();
	groupBoxUseCoordinates->addWidget(txtLon);
	connect(txtLon, SIGNAL(textChanged(QString)), this, SLOT(on_coordinates_changed()));
	
	groupBoxUseCoordinates->xLayout->addSpacing(space);
	groupBoxUseCoordinates->addWidget(new QLabel(tr("Start at VOR")));
	txtVOR = new QLineEdit();
	groupBoxUseCoordinates->addWidget(txtVOR);
	connect(txtVOR, SIGNAL(textChanged(QString)), this, SLOT(on_coordinates_changed()));
	
	groupBoxUseCoordinates->xLayout->addSpacing(space);
	groupBoxUseCoordinates->addWidget(new QLabel(tr("Start at NDB")));
	txtNDB = new QLineEdit();
	groupBoxUseCoordinates->addWidget(txtNDB);
	connect(txtNDB, SIGNAL(textChanged(QString)), this, SLOT(on_coordinates_changed()));
	
	groupBoxUseCoordinates->xLayout->addSpacing(space);
	groupBoxUseCoordinates->addWidget(new QLabel(tr("Start at FIX")));
	txtFIX = new QLineEdit();
	groupBoxUseCoordinates->addWidget(txtFIX);
	connect(txtFIX, SIGNAL(textChanged(QString)), this, SLOT(on_coordinates_changed()));
	
	groupBoxUseCoordinates->xLayout->addSpacing(space);
	groupBoxUseCoordinates->addWidget(new QLabel(tr("Offset distance")));
	txtOffset = new QLineEdit();
	groupBoxUseCoordinates->addWidget(txtOffset);
	connect(txtOffset, SIGNAL(textChanged(QString)), this, SLOT(on_coordinates_changed()));

	groupBoxUseCoordinates->xLayout->addSpacing(space);
	groupBoxUseCoordinates->addWidget(new QLabel(tr("Altitude")));
	txtAltitude = new QLineEdit();
	groupBoxUseCoordinates->addWidget(txtAltitude);
	connect(txtAltitude, SIGNAL(textChanged(QString)), this, SLOT(on_coordinates_changed()));

	groupBoxUseCoordinates->xLayout->addSpacing(space);
	groupBoxUseCoordinates->addWidget(new QLabel(tr("Heading")));
	txtHeading = new QLineEdit();
	groupBoxUseCoordinates->addWidget(txtHeading);
	connect(txtHeading, SIGNAL(textChanged(QString)), this, SLOT(on_coordinates_changed()));

	/*groupBoxUseCoordinates->xLayout->addSpacing(space);
	groupBoxUseCoordinates->addWidget(new QLabel(tr("Roll")));
	txtRoll = new QLineEdit();
	groupBoxUseCoordinates->addWidget(txtRoll);

	groupBoxUseCoordinates->xLayout->addSpacing(space);
	groupBoxUseCoordinates->addWidget(new QLabel(tr("Pitch")));
	txtPitch = new QLineEdit();
	groupBoxUseCoordinates->addWidget(txtPitch);

	groupBoxUseCoordinates->xLayout->addSpacing(space);
	groupBoxUseCoordinates->addWidget(new QLabel(tr("Airspeed")));
	txtAirspeed = new QLineEdit();
	groupBoxUseCoordinates->addWidget(txtAirspeed);*/


	groupBoxUseCoordinates->xLayout->addStretch(20);


	connect(buttonGroupFilter, SIGNAL(buttonClicked(QAbstractButton*)),
			this, SLOT(on_update_airports_filter())
	);




	//============================================================================
	// Map Widget
	//============================================================================
	mapWidget = new OpenLayerWidget(mainObject);
	mapWidget->setMinimumWidth(400);
	mainLayout->addWidget(mapWidget);


	//============================================================================
	//== Main Settings connection
	connect(this, SIGNAL(setx(QString,bool,QString)), mainObject->X, SLOT(set_option(QString,bool,QString)) );
	connect(mainObject->X, SIGNAL(upx(QString,bool,QString)), this, SLOT(on_upx(QString,bool,QString)));



}

//==============================================
//== Initialize
void AirportsWidget::initialize(){
	static bool first_load_done = false;
	if(first_load_done){
		return;
	}

	if (!QFile::exists(mainObject->data_file("airports.txt"))){
		statusBarAirports->showMessage("No cached data. Click Import");
	}else{
		load_airports_tree();
	}
	first_load_done = true;
}




//============================================================================
//== Load Airports Tree
//============================================================================
void AirportsWidget::load_airports_tree(){

	//* Clear existing tree and inhibit updates till end
	model->removeRows(0, model->rowCount());
	treeViewAirports->setUpdatesEnabled(false);

	//* Airports Cache File
	QFile dataFile(mainObject->data_file(("airports.txt")));
	if (!dataFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		   return;
	}
	QTextStream in(&dataFile);
	QString line = in.readLine();
	line = in.readLine();
	while(!line.isNull()){

		QStringList cols = line.split("\t");
		QStandardItem *itemAirportCode = new QStandardItem();
		itemAirportCode->setText(cols.at(0));

		QStandardItem *itemAirportName = new QStandardItem();
		itemAirportName->setText(cols.at(1));

		QStandardItem *itemAirportDir = new QStandardItem();
		itemAirportDir->setText(cols.at(2));

		QList<QStandardItem *> items;
		items << itemAirportCode << itemAirportName << itemAirportDir;

		model->appendRow(items);
		line = in.readLine();
	}

	//= Update the status bar with the count
	statusBarAirports->showMessage( QString("%1 airports").arg(model->rowCount()) );

	//= Restore previous airport from settings.. if found
	QList<QStandardItem *> items = model->findItems( mainObject->X->getx("--airport=", "KSFO"), Qt::MatchExactly, CA_CODE);
	if(items.count() > 0){
		QModelIndex srcIdx = model->indexFromItem(items[0]);
		QModelIndex proxIdx = proxyModel->mapFromSource(srcIdx);
		treeViewAirports->selectionModel()->select(proxIdx, QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
		treeViewAirports->scrollTo(proxIdx, QAbstractItemView::PositionAtCenter);
		load_info_tree( model->item(srcIdx.row(), CA_DIR)->text(), model->item(srcIdx.row(), CA_CODE)->text() );
	}
	treeViewAirports->setUpdatesEnabled(true);
}


//==============================================================
//*** Update Filters
void AirportsWidget::on_update_airports_filter(){

	mainObject->settings->setValue("airports_button_filter", buttonGroupFilter->checkedId());
	int filter_column_id = buttonGroupFilter->checkedButton()->property("filter_column_idx").toInt();
	proxyModel->setFilterKeyColumn( filter_column_id );
	proxyModel->setFilterFixedString( txtAirportsFilter->text() );
}


//==============================================================
//*** Airport Clicked 
void AirportsWidget::on_airport_tree_selected(QModelIndex currentIdx, QModelIndex previousIdx){
	Q_UNUSED(previousIdx);

	//= Clear the Runways tree
	treeWidgetAirportInfo->model()->removeRows(0, treeWidgetAirportInfo->model()->rowCount());

	//= No selection -eg a filter removing a selected node
	if(!currentIdx.isValid()){
		emit setx("--airport=", false, "");
		emit setx("--runway=", false, "");
		emit setx("--parking-id=", false,"");
		return;
	}
	//= Get the airport code forn source model
	QModelIndex srcIndex = proxyModel->mapToSource(currentIdx);
	QString airport_code = model->item(srcIndex.row(), CA_CODE)->text();
	QString airport_dir = model->item(srcIndex.row(), CA_DIR)->text();
	emit setx("--airport=", true, airport_code);
	emit setx("--runway=", false, "");
	emit setx("--parking-id=", false,"");
	load_info_tree(airport_dir, airport_code);

}

//==============================================================
// Load the Info Tree
//==============================================================
void AirportsWidget::load_info_tree(QString airport_dir, QString airport_code){

	QString count_label;
	int runways_count = load_runways_node(airport_dir, airport_code);
	if(runways_count == 0){
		count_label.append(tr("No runway"));
	}else if(runways_count == 1){
		count_label.append(tr("One runway"));
	}else{
		count_label.append(QString::number(runways_count)).append(" runways");
	}

	count_label.append(" / ");

	int stands_count =  load_parking_node(airport_dir, airport_code);
	if(stands_count == 0){
		count_label.append(tr("No stands"));
	}else if(stands_count == 1){
		count_label.append(tr("One stand"));
	}else{
		count_label.append(QString::number(stands_count)).append(" stands");
	}

	statusBarAirportInfo->showMessage(count_label);

	//= Restore Runway node from settings
	XOpt optRunway = mainObject->X->get_opt("--runway=");
	if(optRunway.enabled){
		QList<QTreeWidgetItem *> items = treeWidgetAirportInfo->findItems(
														optRunway.value,
														Qt::MatchExactly | Qt::MatchRecursive,
														CI_NODE);
		if(items.size() > 0){
			treeWidgetAirportInfo->setCurrentItem(items[0]);
			treeWidgetAirportInfo->scrollToItem(items[0], QAbstractItemView::EnsureVisible);
		}
		return; //= dontbother with stand below as we found runway
	}
	//= Restore Stand node from settings
	XOpt optStand = mainObject->X->get_opt("--parking-id=");
	if(optStand.enabled){
		QList<QTreeWidgetItem *> items = treeWidgetAirportInfo->findItems(
														optStand.value,
														Qt::MatchExactly | Qt::MatchRecursive,
														CI_NODE);
		if(items.size() > 0){
			treeWidgetAirportInfo->setCurrentItem(items[0]);
			treeWidgetAirportInfo->scrollToItem(items[0], QAbstractItemView::EnsureVisible);
		}
	}
}

//==============================================================
// Load Runways
//==============================================================
int AirportsWidget::load_runways_node(QString airport_dir, QString airport_code){

	//= Create the Runways Node
	QTreeWidgetItem *runwaysParent = new QTreeWidgetItem();
	runwaysParent->setText(0, "Runways" );
	runwaysParent->setIcon(0, QIcon(":/icon/folder"));
	treeWidgetAirportInfo->addTopLevelItem(runwaysParent);
	treeWidgetAirportInfo->setItemExpanded(runwaysParent, true);
	treeWidgetAirportInfo->setFirstItemColumnSpanned(runwaysParent, true);


	//==============================================================
	// Parse the <CODE>.threshold.xml file to get runways
	//==============================================================
	/*
	<?xml version="1.0"?>
	<PropertyList>
	  <runway>
		<threshold>
		  <lon>0.044298885776989</lon>
		  <lat>51.505569223906</lat>
		  <rwy>10</rwy>
		  <hdg-deg>92.88</hdg-deg>
		  <displ-m>95</displ-m>
		  <stopw-m>55</stopw-m>
		</threshold>
		<threshold>
		  <lon>0.065996952433288</lon>
		  <lat>51.5048897753222</lat>
		  <rwy>28</rwy>
		  <hdg-deg>272.88</hdg-deg>
		  <displ-m>71</displ-m>
		  <stopw-m>90</stopw-m>
		</threshold>
	  </runway>
	</PropertyList>
	*/

	//* Get the contents of the file
	QString threshold_file( airport_dir.append("/").append(airport_code).append(".threshold.xml") );
	QFile fileXmlThrehsold(threshold_file);
	fileXmlThrehsold.open(QIODevice::ReadOnly);

	//= Make file contents into a string from bytearray
	QString xmlThresholdString = fileXmlThrehsold.readAll();

	//= Create domDocument - important dont pass string in  QDomConstrucor(string) as ERRORS.. took hours DONT DO IT
	QDomDocument dom;
	dom.setContent(xmlThresholdString); //* AFTER dom has been created, then set the content from a string from the file

	//= Get <runway> nodes
	QDomNodeList nodeRunways = dom.elementsByTagName("runway");

	if (nodeRunways.count() > 0){
		for(int idxd =0; idxd < nodeRunways.count(); idxd++){

			// loops the <runway> nodes
			QDomNode nodeRunway = nodeRunways.at(idxd);

			//== Add runway parent ie X - Y both ends
			QTreeWidgetItem *rItem = new QTreeWidgetItem(runwaysParent);
			rItem->setIcon(0, QIcon(":/icon/runways"));
			rItem->setText(CI_RUNWAYS, "1");
			rItem->setText(CI_NODE, nodeRunway.childNodes().at(0).firstChildElement("rwy").text().append(
									" - ").append(
											nodeRunway.childNodes().at(1).firstChildElement("rwy").text()
									));
			treeWidgetAirportInfo->setItemExpanded(rItem, true);
			treeWidgetAirportInfo->setFirstItemColumnSpanned(rItem, true);

			//= Runway threshold 0
			QTreeWidgetItem *tItem0 = new QTreeWidgetItem(rItem);
			tItem0->setIcon(0, QIcon(":/icon/runway"));
			tItem0->setText(CI_NODE,  nodeRunway.childNodes().at(0).firstChildElement("rwy").text());
			tItem0->setText(CI_LAT,  nodeRunway.childNodes().at(0).firstChildElement("lat").text());
			tItem0->setText(CI_LON,  nodeRunway.childNodes().at(0).firstChildElement("lon").text());
			tItem0->setText(CI_TYPE, "runway");
			tItem0->setText(CI_SETTING_KEY, QString(airport_code).append("runway").append(
											nodeRunway.childNodes().at(0).firstChildElement("rwy").text()));

			//= Runway  threshold 1
			QTreeWidgetItem *tItem1 = new QTreeWidgetItem(rItem);
			tItem1->setIcon(0, QIcon(":/icon/runway"));
			tItem1->setText(CI_NODE,  nodeRunway.childNodes().at(1).firstChildElement("rwy").text());
			tItem1->setText(CI_LAT,  nodeRunway.childNodes().at(1).firstChildElement("lat").text());
			tItem1->setText(CI_LON,  nodeRunway.childNodes().at(1).firstChildElement("lon").text());
			tItem1->setText(CI_TYPE, "runway");
			tItem1->setText(CI_SETTING_KEY, QString(airport_code).append("runway").append(
											nodeRunway.childNodes().at(1).firstChildElement("rwy").text()));



			mapWidget->add_runway(	airport_code,
									tItem0->text(CI_LAT), tItem0->text(CI_LON),
									tItem1->text(CI_LAT), tItem1->text(CI_LON)
									);
		}
	}
	mapWidget->zoom_to_airport(airport_code);

	return runwaysParent->childCount();
}

//==============================================================
// Load Stands + Parking
//==============================================================
int AirportsWidget::load_parking_node(QString airport_dir, QString airport_code){

	//* Create the Parkings Node
	QTreeWidgetItem *parkingParent = new QTreeWidgetItem();
	parkingParent->setText(0, "Parking" );
	parkingParent->setIcon(0, QIcon(":/icon/folder"));
	treeWidgetAirportInfo->addTopLevelItem(parkingParent);
	treeWidgetAirportInfo->setItemExpanded(parkingParent, true);

	//=======================================================================
	// Parse the <groundnet/parking>.threshold.xml file for Parking Postiton
	//=======================================================================
	/*
	<?xml version="1.0"?>
	<groundnet>
	  <frequencies>
		<AWOS>12807</AWOS>
		<CLEARANCE>12197</CLEARANCE>
		<GROUND>12190</GROUND>
		<TOWER>12447</TOWER>
		<APPROACH>13497</APPROACH>
	  </frequencies>
	  <parkingList>
		<Parking index="0"
				 type="gate"
				 name="EGLL London Heathrow Ramp #11111"
				 number="1"
				 lat="N51 28.192"
				 lon="W00 27.892"
				 heading="56"
				 radius="54"
				 airlineCodes="" />
	  .... snipped ....
	*/

	//* Files in terrasync are named "groundnet.xml"; in scenery their "parking.xml" -- Why asks pete??
	QString file_path(airport_dir.append("/").append(airport_code));
	file_path.append( mainObject->X->terrasync_enabled() ? ".groundnet.xml" : ".parking.xml");

	//* Check parking file exists
	if(QFile::exists(file_path)){

		//* Open file and read contents to string
		QFile ppfile(file_path);
		ppfile.open(QIODevice::ReadOnly);
		QString xmlString = ppfile.readAll();

		//* Create domDocument - important - don't pass string in  QDomConstrucor(string) as ERRORS.. took hours DONT DO IT
		QDomDocument dom;
		dom.setContent(xmlString); //* AFTER dom has been created, then set the content from a string from the file

		QStringList listParkingPositions;

		//* Get <Parking/> nodes and loop thru them and add to list (removing dupes)
		QDomNodeList parkingNodes = dom.elementsByTagName("Parking");
		if (parkingNodes.count() > 0){
			for(int idxd =0; idxd < parkingNodes.count(); idxd++){

				 QDomNode parkingNode = parkingNodes.at(idxd);
				 QDomNamedNodeMap attribs = parkingNode.attributes();
				 QString gate(attribs.namedItem("name").nodeValue());
				 gate.append(attribs.namedItem("number").nodeValue());

				//* Check it doesnt already exist - pete is confused as to multiple entries
				 if(!listParkingPositions.contains(gate)){
					 if(attribs.namedItem("type").nodeValue() == "gate"){

						//* Append position to eliminate dupes
						if(!gate.contains(" ")){
							listParkingPositions.append(gate);
						}else{
							//qDebug() << gate;
						}
					}
				}
			}

		}

		//* Create the tree nodes
		if(listParkingPositions.size() == 0){
			QTreeWidgetItem *pItem = new QTreeWidgetItem(parkingParent);
			pItem->setText(CI_NODE, "None");
			return 0;
		}else{
			listParkingPositions.sort();
			for(int i =0; i < listParkingPositions.size(); i++){
				QTreeWidgetItem *pItem = new QTreeWidgetItem(parkingParent);
				pItem->setIcon(0, QIcon(":/icon/stand"));
				pItem->setText(CI_NODE, listParkingPositions.at(i));
				pItem->setText(CI_TYPE, "stand");
				pItem->setText(CI_SETTING_KEY, QString(airport_code).append("stand").append(listParkingPositions.at(i)));
			}
		}
	} /* File Exists */

	//* return the count
	return parkingParent->childCount();
}




void AirportsWidget::on_reload_cache(){

	ImportAirportsDialog *widget = new ImportAirportsDialog();
	int resp = widget->exec();
	if(resp == QDialog::Rejected){
		return;
	}

	bool icao_only = widget->buttIcaoOnly->isChecked();

	QProgressDialog progress(this);
	AirportsData::import(progress, mainObject, icao_only);
	load_airports_tree();
}

void AirportsWidget::on_buttonGroupUse(){
	groupBoxAirport->setEnabled(buttonGroupUse->checkedId() == USE_AIRPORT);
	groupBoxUseCoordinates->setEnabled(buttonGroupUse->checkedId() == USE_COORDINATES);
}


//====================================================================
// Current Selected Aiport
//====================================================================

QString AirportsWidget::current_airport(){
	qDebug() << "curr_apt"; 
	if(treeViewAirports->selectionModel()->selectedIndexes().size() == 0){
		return "";
	}
	QModelIndex index = proxyModel->index(treeViewAirports->selectionModel()->selectedIndexes().at(0).row(), CA_CODE);
	QStandardItem *item = model->itemFromIndex(proxyModel->mapToSource(index));
	return item->text();
}



QString AirportsWidget::validate(){

	//* Validate Coordinates TODO - other stuff
	if(buttonGroupUse->checkedId() == USE_COORDINATES){
		if(txtLat->text().trimmed().length() == 0){
			txtLat->setFocus();
			return QString("Need Latitude");
		}
		if(txtLon->text().trimmed().length() == 0){
			txtLon->setFocus();
			return QString("Need Longtitude");
		}

	//* Validate Airport
	}else if(buttonGroupUse->checkedId() == USE_AIRPORT){
		if (!treeViewAirports->selectionModel()->hasSelection()){
			return QString(tr("Airport: No Airport selected or check [x] Use Default;"));
		}
	}

	return QString("");

}


//void AirportsWidget::on_view_map(){
//	qDebug() << "Map: OpenLayer";
	//QString cApt = current_airport();
	//QList<QTreeWidgetItem *> runways = treeWidgetAirportInfo->findItems("1", Qt::MatchExactly | Qt::MatchRecursive, CI_RUNWAYS);
	//qDebug() << "runways" << runways.length();
	//mainObject->mpMapXWidget->add_airport(cApt);
	/*for(int idx =0; idx < runways.length(); idx++){
		//qDebug() << item->child(idx)->text(CI_LAT);
		mainObject->mpMapXWidget->add_runway(cApt,
										runways.at(idx)->child(0)->text(CI_NODE),
										runways.at(idx)->child(0)->text(CI_LAT),
										runways.at(idx)->child(0)->text(CI_LNG),
										runways.at(idx)->child(1)->text(CI_NODE),
										runways.at(idx)->child(1)->text(CI_LAT),
										runways.at(idx)->child(1)->text(CI_LNG)
										);

	}*/
	//mainObject->mpMapXWidget->show_airport(cApt);
	//mainObject->xOpenLayerWidget->show();

//}

void AirportsWidget::on_coordinates_changed()
{
	emit setx("--vor=", true, txtVOR->text());
	emit setx("--ndb=", true, txtNDB->text());
	emit setx("--fix=", true, txtFIX->text());
	emit setx("--lat=", true, txtLat->text());
	emit setx("--lon=", true, txtLon->text());
	emit setx("--offset-distance=", true, txtOffset->text());
	emit setx("--altitude=", true, txtAltitude->text());
	emit setx("--heading=", true, txtHeading->text());
}


void AirportsWidget::on_upx(QString option, bool enabled, QString value)
{
	Q_UNUSED(option);
	Q_UNUSED(enabled);
	Q_UNUSED(value);
	
	if(option == "--vor="){
		txtVOR->setText(value);
		
	}else if(option == "--ndb="){
		txtNDB->setText(value);
		
	}else if(option == "--fix="){
		txtFIX->setText(value);
		
	}else if(option == "--lat="){
		txtLat->setText(value);
		
	}else if(option == "--lon="){
		txtLon->setText(value);
		
	}else if(option == "--offset-distance="){
		txtOffset->setText(value);
		
	}else if(option == "--altitude="){
		txtAltitude->setText(value);
		
	}else if(option == "--heading="){
		txtHeading->setText(value);
	}
	
	
}


void AirportsWidget::on_airport_info_selection_changed()
{
	//= No Selection
	if(treeWidgetAirportInfo->selectionModel()->hasSelection() == false){
		emit setx("--runway=", false, "");
		emit setx("--parking-id=", false,"");
		return;
	}

	QTreeWidgetItem *item = treeWidgetAirportInfo->currentItem();

	//= Its a runway
	if(item->text(CI_TYPE) == "runway"){
		emit setx("--runway=", true, item->text(CI_NODE));
		emit setx("--parking-id=", false,"");
		return;
	}

	//= Its a runway
	if(item->text(CI_TYPE) == "stand"){
		emit setx("--runway=", false, "");
		emit setx("--parking-id=", true, item->text(CI_NODE));
		return;
	}

	//.if(treeWidgetAirportInfo->indexOfTopLevelItem(item) != -1){
	//	emit setx("--runway=", false, "");
	//	emit setx("--parking-id=", false,"");
	//}

}
