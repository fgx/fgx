
#include <QtCore/QDebug>
#include <QtCore/QString>


#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>


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

	QPushButton *buttonImportAirports = new QPushButton(this);
	layoutAptTopBar->addWidget(buttonImportAirports);
	buttonImportAirports->setText("Reload");
	buttonImportAirports->setToolTip("Scan directories and reload cache");
	buttonImportAirports->setIcon(QIcon(":/icon/import"));
	buttonImportAirports->setFlat(true);
	connect(buttonImportAirports, SIGNAL(clicked()), this, SLOT(on_import_aptdat_clicked()) );

	QPushButton *buttonRefreshAirports = new QPushButton(this);
	layoutAptTopBar->addWidget(buttonRefreshAirports);
	buttonRefreshAirports->setText("Reload");
	buttonRefreshAirports->setToolTip("Scan directories and reload cache");
	buttonRefreshAirports->setIcon(QIcon(":/icon/refresh"));
	buttonRefreshAirports->setFlat(true);
	connect(buttonRefreshAirports, SIGNAL(clicked()), this, SLOT(on_rescan_xml_clicked()) );


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
	headerLabelsList << tr("Code") << tr("Name");
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

	//** Headers and columns
	treeViewAirports->header()->setStretchLastSection(true);
	treeViewAirports->setColumnWidth(CA_CODE, 80);
	treeViewAirports->setColumnWidth(CA_NAME, 50);

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
	QTreeWidgetItem *headerItem = treeWidgetAirportInfo->headerItem();
	headerItem->setText(CI_NODE, tr("Startup Position"));
    headerItem->setText(1, tr("Width"));
    headerItem->setText(2, tr("Length"));
    headerItem->setText(3, tr("Lat"));
    headerItem->setText(4, tr("Lng"));
    headerItem->setText(5, tr("Alignment"));
	//treeWidgetAirportInfo->setColumnHidden(1,true);
	//treeWidgetAirportInfo->setColumnHidden(2,true);
	//treeWidgetAirportInfo->setColumnHidden(3,true);
	//treeWidgetAirportInfo->setColumnHidden(4,true);
	//treeWidgetAirportInfo->setColumnHidden(5,true);


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
	QSqlQuery query("SELECT code, name FROM airports order by code ASC", mainObject->db);

	//* Loop results  and appendRows
	while(query.next()){

		QStandardItem *itemAirportCode = new QStandardItem();
		itemAirportCode->setText(query.value(0).toString());

		QStandardItem *itemAirportName = new QStandardItem();
		itemAirportName->setText(query.value(1).toString());

		QList<QStandardItem *> items;
		items << itemAirportCode << itemAirportName;

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
		load_info_tree( model->item(srcIdx.row(), CA_CODE)->text() );
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
	load_info_tree(airport_code);

}

//==============================================================
// Load the Info Tree
//==============================================================
void AirportsWidget::load_info_tree(QString airport_code){
	//* Load Nodes from DB - with a count from each
	QString count_label;

	int runways_count = load_runways_node(airport_code);
	if(runways_count == 0){
		count_label.append(tr("No runway"));
	}else if(runways_count == 1){
		count_label.append(tr("One runway"));
	}else{
		count_label.append("%s runways").arg(runways_count);
	}

	count_label.append(" / ");

	int stands_count =  load_parking_node(airport_code);
	if(stands_count == 0){
		count_label.append(tr("No stands"));
	}else if(stands_count == 1){
		count_label.append(tr("One stand"));
	}else{
		count_label.append("%s stands").arg(runways_count);
	}

	statusBarAirportInfo->showMessage(count_label);

}

//==============================================================
// Load Runways
//==============================================================
int AirportsWidget::load_runways_node(QString airport_code){

	//* Create the Runways Node
	QTreeWidgetItem *runwaysParent = new QTreeWidgetItem();
	runwaysParent->setText(0, "Runways" );
	runwaysParent->setIcon(0, QIcon(":/icon/folder"));
	treeWidgetAirportInfo->addTopLevelItem(runwaysParent);
	treeWidgetAirportInfo->setItemExpanded(runwaysParent, true);

	//* DB query
	QSqlQuery query(mainObject->db);
	query.prepare("SELECT runway, heading, lat, lon FROM runways WHERE airport_code = ? ORDER BY runway ASC;");
	query.bindValue(0, airport_code);
	query.exec();

	//* No results so create a "none node"
	if(query.size() == 0){

		QTreeWidgetItem *item = new QTreeWidgetItem(runwaysParent);
		item->setText(0, "-- None --");
		return 0;
	}

	//* Loop and add the runways
	while(query.next()){
		QTreeWidgetItem *iRunway = new QTreeWidgetItem(runwaysParent);
		iRunway->setText(CI_NODE, query.value(0).toString());
		iRunway->setText(CI_SETTING_KEY, QString("runway_").append(query.value(0).toString()));

		QTreeWidgetItem *iHeading = new QTreeWidgetItem(iRunway);
		iHeading->setText(CI_NODE, query.value(1).toString());

	}

	return runwaysParent->childCount();
}

//==============================================================
// Load Stands + Parking
//==============================================================
int AirportsWidget::load_parking_node(QString airport_code){

	//* Create the Parkings Node
	QTreeWidgetItem *parkingParent = new QTreeWidgetItem();
	parkingParent->setText(0, "Parking" );
	parkingParent->setIcon(0, QIcon(":/icon/folder"));
	treeWidgetAirportInfo->addTopLevelItem(parkingParent);
	treeWidgetAirportInfo->setItemExpanded(parkingParent, true);

	//* DB query
	QSqlQuery query(mainObject->db);
	query.prepare("SELECT stand, heading, lat, lng FROM stands WHERE airport_code = ? ORDER BY stand ASC;");
	query.bindValue(0, airport_code);
	query.exec();

	//* No results so create a "none node"
	if(query.size() == 0){
		QTreeWidgetItem *item = new QTreeWidgetItem(parkingParent);
		item->setText(0, tr("-- None --"));
		return 0;
	}

	//* Loop and add the Stands
	while(query.next()){
		QTreeWidgetItem *item = new QTreeWidgetItem(parkingParent);
		item->setText(CI_NODE, query.value(0).toString());
		item->setText(CI_SETTING_KEY, QString("stand_").append(query.value(0).toString()));
	}

	//* return the count
	return parkingParent->childCount();
}



//===========================================================================
void AirportsWidget::on_import_aptdat_clicked(){
	ImportAirportsWidget *widget = new ImportAirportsWidget();
	widget->setWindowModality(Qt::WindowModal);
	widget->show();
}

void AirportsWidget::on_rescan_xml_clicked(){
	qDebug() << "Refresh Clicked <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
	//scan_airports_xml();
	AirportsImport *airportsImport = new AirportsImport(this, mainObject);
	airportsImport->import_airports();
	//load_airports_tree();
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
