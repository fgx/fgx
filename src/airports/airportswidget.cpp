
#include <QtCore/QDebug>
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




#include "airports/airportswidget.h"
#include "airports/importairportswidget.h"
#include "airports/airportsimport.h"


AirportsWidget::AirportsWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mOb;

    //* Main Layout
	QGridLayout *mainLayout = new QGridLayout();
    setLayout(mainLayout);
	mainLayout->setSpacing(10);
	int m = 10;
	mainLayout->setContentsMargins(m,m,m,m);

	//=====================================
	//** Top Startup Option Buttons
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


	//=================================================================================
	//* Airport Details
	groupBoxAirport = new QGroupBox(this);
	groupBoxAirport->setTitle("Airport Details");
	mainLayout->addWidget(groupBoxAirport, 1, 0);

	QVBoxLayout *airportsContainerLayout = new QVBoxLayout();
	groupBoxAirport->setLayout(airportsContainerLayout);
	airportsContainerLayout->setContentsMargins(10,10,10,10);
	airportsContainerLayout->setSpacing(0);


	//==================================================================
	//** Airports Filter and Actions Top Bar
	QHBoxLayout *layoutAptTopBar = new QHBoxLayout();
	layoutAptTopBar->setContentsMargins(0,0,0,0);
	layoutAptTopBar->setSpacing(10);
	airportsContainerLayout->addLayout(layoutAptTopBar);

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


	QToolButton *buttonReloadCache = new QToolButton(this);
	layoutAptTopBar->addWidget(buttonReloadCache);
	buttonReloadCache->setText("Import");
	buttonReloadCache->setToolTip("Scan directories and reload cache");
	buttonReloadCache->setIcon(QIcon(":/icon/import"));
	buttonReloadCache->setAutoRaise(true);
	buttonReloadCache->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(buttonReloadCache, SIGNAL(clicked()), this, SLOT(on_reload_cache()) );


	//*==============================================================================
	//** Airports/Runways Splitter
	QSplitter *splitter = new QSplitter(this);
	airportsContainerLayout->addWidget(splitter);


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
			 SIGNAL( currentRowChanged(QModelIndex,QModelIndex) ),
			 this, SLOT( on_airport_tree_selected(QModelIndex,QModelIndex) )
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
	//treeWidgetAirportInfo->setSortingEnabled(true);

	QTreeWidgetItem *headerItem = treeWidgetAirportInfo->headerItem();
	headerItem->setText(CI_NODE, tr(""));
    headerItem->setText(1, tr("Width"));
    headerItem->setText(2, tr("Length"));
    headerItem->setText(3, tr("Lat"));
    headerItem->setText(4, tr("Lng"));
    headerItem->setText(5, tr("Alignment"));
	treeWidgetAirportInfo->setColumnHidden(1,true);
	treeWidgetAirportInfo->setColumnHidden(2,true);
	treeWidgetAirportInfo->setColumnHidden(3,true);
	treeWidgetAirportInfo->setColumnHidden(4,true);
	treeWidgetAirportInfo->setColumnHidden(5,true);


	statusBarAirportInfo = new QStatusBar();
	statusBarAirportInfo->setSizeGripEnabled(false);
	airportInfoLayout->addWidget(statusBarAirportInfo);
	statusBarAirportInfo->showMessage("");


	splitter->setStretchFactor(0, 2);
	splitter->setStretchFactor(1, 2);



	//========================================================================================================
	//** Use Coordinates
	//====================================================================

	groupBoxUseCoordinates = new QGroupBox(this);
	groupBoxUseCoordinates->setTitle("Coordinates");
	groupBoxUseCoordinates->setMaximumWidth(300);
	mainLayout->addWidget(groupBoxUseCoordinates, 1, 2);
	QVBoxLayout *layoutCoordinates = new QVBoxLayout();
	groupBoxUseCoordinates->setLayout(layoutCoordinates);
	int space = 5;

	layoutCoordinates->addWidget(new QLabel(tr("Latitude (negative is west)")));
	txtLat = new QLineEdit();
	layoutCoordinates->addWidget(txtLat);

	layoutCoordinates->addSpacing(space);
	layoutCoordinates->addWidget(new QLabel(tr("Longtitude (negative is south)")));
	txtLng = new QLineEdit();
	layoutCoordinates->addWidget(txtLng);

	layoutCoordinates->addSpacing(space);
	layoutCoordinates->addWidget(new QLabel(tr("Altitude")));
	txtAltitude = new QLineEdit();
	layoutCoordinates->addWidget(txtAltitude);

	layoutCoordinates->addSpacing(space);
	layoutCoordinates->addWidget(new QLabel(tr("Heading")));
	txtHeading = new QLineEdit();
	layoutCoordinates->addWidget(txtHeading);

	layoutCoordinates->addSpacing(space);
	layoutCoordinates->addWidget(new QLabel(tr("Roll")));
	txtRoll = new QLineEdit();
	layoutCoordinates->addWidget(txtRoll);

	layoutCoordinates->addSpacing(space);
	layoutCoordinates->addWidget(new QLabel(tr("Pitch")));
	txtPitch = new QLineEdit();
	layoutCoordinates->addWidget(txtPitch);

	layoutCoordinates->addSpacing(space);
	layoutCoordinates->addWidget(new QLabel(tr("Airspeed")));
	txtAirspeed = new QLineEdit();
	layoutCoordinates->addWidget(txtAirspeed);


	layoutCoordinates->addStretch(20);

	//on_buttonGroupUse();

	connect(buttonGroupFilter, SIGNAL(buttonClicked(QAbstractButton*)),
			this, SLOT(on_update_airports_filter())
	);
}

void AirportsWidget::initialize(){
	load_airports_tree();
}




//============================================================================
//** Load Airports Tree
//============================================================================
void AirportsWidget::load_airports_tree(){

	//* Clear existing tree and inhibit updates till end
	model->removeRows(0, model->rowCount());
	treeViewAirports->setUpdatesEnabled(false);

	//* Get Airports from database
	QSqlQuery query("SELECT code, name, dir FROM airports order by code ASC", mainObject->db);

	//* Loop results  and appendRows
	while(query.next()){

		QStandardItem *itemAirportCode = new QStandardItem();
		itemAirportCode->setText(query.value(0).toString());

		QStandardItem *itemAirportName = new QStandardItem();
		itemAirportName->setText(query.value(1).toString());

		QStandardItem *itemAirportDir = new QStandardItem();
		itemAirportDir->setText(query.value(2).toString());

		QList<QStandardItem *> items;
		items << itemAirportCode << itemAirportName << itemAirportDir;

		model->appendRow(items);
	}

	//* Update the status bar with the count
	statusBarAirports->showMessage( QString("%1 airports").arg(model->rowCount()) );

	//* Restore previous airport from settings.. if found
	QList<QStandardItem *> items = model->findItems( mainObject->settings->value("airport", "KSFO").toString(), Qt::MatchExactly, CA_CODE);
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

	//* Clear the Runways tree
	treeWidgetAirportInfo->model()->removeRows(0, treeWidgetAirportInfo->model()->rowCount());

	//* No selection -eg a filter removing a selected node
	if(!currentIdx.isValid()){
		return;
	}

	//* Get the airport code forn source model
	QModelIndex srcIndex = proxyModel->mapToSource(currentIdx);
	QString airport_code = model->item(srcIndex.row(), CA_CODE)->text();
	QString airport_dir = model->item(srcIndex.row(), CA_DIR)->text();
	load_info_tree(airport_dir, airport_code);

}

//==============================================================
// Load the Info Tree
//==============================================================
void AirportsWidget::load_info_tree(QString airport_dir, QString airport_code){
	//* Load Nodes from DB - with a count from each
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

}

//==============================================================
// Load Runways
//==============================================================
int AirportsWidget::load_runways_node(QString airport_dir, QString airport_code){

	//* Create the Runways Node
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

	//* Make file contents into a string from bytearray
	QString xmlThresholdString = fileXmlThrehsold.readAll();

	//* Create domDocument - important dont pass string in  QDomConstrucor(string) as ERRORS.. took hours DONT DO IT
	QDomDocument dom;
	dom.setContent(xmlThresholdString); //* AFTER dom has been created, then set the content from a string from the file

	//* Get threhold nodes
	QDomNodeList nodesThreshold = dom.elementsByTagName("threshold");

	if (nodesThreshold.count() > 0){
		for(int idxd =0; idxd < nodesThreshold.count(); idxd++){
			//* Nodes "rwy" << "hdg-deg" << "lat" << "lon";
			QDomNode thresholdNode = nodesThreshold.at(idxd);
			QTreeWidgetItem *rItem = new QTreeWidgetItem(runwaysParent);
			rItem->setText(CI_NODE, thresholdNode.firstChildElement("rwy").text());
			rItem->setText(CI_TYPE, "runway");
		}
	}


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
	file_path.append(mainObject->settings->value("use_terrasync").toBool() == true ? ".groundnet.xml" : ".parking.xml");

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
		//* Get <Parking/> nodes and loop thru them
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
						listParkingPositions.append(gate);
					}
				}
			}

		}

		if(listParkingPositions.length() == 0){
			QTreeWidgetItem *pItem = new QTreeWidgetItem(parkingParent);
			pItem->setText(CI_NODE, "None");
			return 0;
		}else{
			listParkingPositions.sort();
			for(int i =0; i < listParkingPositions.size(); i++){
				QTreeWidgetItem *pItem = new QTreeWidgetItem(parkingParent);
				pItem->setText(CI_NODE, listParkingPositions.at(i));
				pItem->setText(CI_TYPE, "stand");
			}
		}
	} /* File Exists */

	//* return the count
	return parkingParent->childCount();
}




void AirportsWidget::on_reload_cache(){

	int resp = QMessageBox::information(this, tr("Import Airports Cache"),
										tr("Importing airports can take some time. Click Ok to continue."),
										QMessageBox::Cancel | QMessageBox::Ok);
	if(resp == QMessageBox::Cancel){
		return;
	}

	AirportsImport *airportsImport = new AirportsImport(this, mainObject);
	airportsImport->import_airports(this);
	load_airports_tree();
}

void AirportsWidget::on_buttonGroupUse(){
	groupBoxAirport->setEnabled(buttonGroupUse->checkedId() == USE_AIRPORT);
	groupBoxUseCoordinates->setEnabled(buttonGroupUse->checkedId() == USE_COORDINATES);
}


//================================================================
// Get Args
QStringList AirportsWidget::get_args(){
	QStringList args;
	//if (parkPosition->isEnabled() == true) {
	//	args << QString("--parkpos=").append(parkPosition->currentText());
	//}
	buttonGroupUse->button(mainObject->settings->value("use_position", USE_DEFAULT).toInt());

	return args;
}

//=============================================================
// Save Settings
void AirportsWidget::save_settings(){

	mainObject->settings->setValue("use_position", buttonGroupUse->checkedId());

	mainObject->settings->setValue("lat", txtLat->text());
	mainObject->settings->setValue("lng", txtLng->text());
	mainObject->settings->setValue("altitude", txtAltitude->text());
	mainObject->settings->setValue("heading", txtHeading->text());
	mainObject->settings->setValue("roll", txtRoll->text());
	mainObject->settings->setValue("pitch", txtPitch->text());
	mainObject->settings->setValue("airspeed", txtAirspeed->text());

	//** Save Airport
	if(treeViewAirports->currentIndex().row() != -1){
		QModelIndex index = proxyModel->index(treeViewAirports->currentIndex().row(), CA_CODE);
		QStandardItem *item = model->itemFromIndex(proxyModel->mapToSource(index));
		mainObject->settings->setValue("airport", item->text());

		//** save runway or parking
		if(treeWidgetAirportInfo->currentItem()){
			//* check its not one of the parents ie savable
			if(treeWidgetAirportInfo->indexOfTopLevelItem( treeWidgetAirportInfo->currentItem()  ) != -1){
				//QString key = treeWidgetAirportInfo->currentItem()->text(CI_KEY);
				//QString val = treeWidgetAirportInfo->currentItem()->text(CI_NODE);
				//if(key == "runway"){
				//	mainObject->
				//}else if(key == "stand"):
				//	stand = treeWidgetAirportInfo->currentItem()->text(CI_NODE);
				qDebug() << "saved" << treeWidgetAirportInfo->currentItem()->text(CI_SETTING_KEY);
				mainObject->settings->setValue("runway_stand", treeWidgetAirportInfo->currentItem()->text(CI_SETTING_KEY));
			}else{
				mainObject->settings->setValue("runway_stand", "");
			}

		}else{
			mainObject->settings->setValue("runway_stand", "");
		}
	}

	mainObject->settings->sync();
}


//====================================================================
// Load Settings
// Restoring the airport/runway is done in the load_tree functions
//====================================================================
void AirportsWidget::load_settings(){

	buttonGroupUse->button( mainObject->settings->value("use_position", USE_DEFAULT).toInt() )->setChecked(true);

	txtLat->setText(mainObject->settings->value("lat").toString());
	txtLng->setText(mainObject->settings->value("lng").toString());
	txtAltitude->setText(mainObject->settings->value("altitude").toString());
	txtHeading->setText(mainObject->settings->value("heading").toString());
	txtRoll->setText(mainObject->settings->value("roll").toString());
	txtPitch->setText(mainObject->settings->value("pitch").toString());
	txtAirspeed->setText(mainObject->settings->value("airspeed").toString());

	on_buttonGroupUse();

}


QString AirportsWidget::validate(){

	if(buttonGroupUse->checkedId() == USE_COORDINATES){
		if(txtLat->text().trimmed().length() == 0){
			txtLat->setFocus();
			return QString("Need Latitude");
		}
		if(txtLng->text().trimmed().length() == 0){
			txtLng->setFocus();
			return QString("Need Longtitude");
		}
	}else if(buttonGroupUse->checkedId() == USE_AIRPORT){
		if (!treeViewAirports->selectionModel()->hasSelection()){
			return QString(tr("No Airport selected"));
		}
	}

	return QString();

}
