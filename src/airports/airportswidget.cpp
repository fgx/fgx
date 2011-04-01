
#include <QtCore/QDebug>
#include <QtCore/QString>

#include <QtGui/QSizePolicy>
#include <QtGui/QFont>

#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QSplitter>

#include <QtGui/QToolBar>
//#include <QtGui/QToolButton>
#include <QtGui/QPushButton>


#include <QtGui/QAction>
#include <QtGui/QLabel>
#include <QtGui/QProgressDialog>
#include <QtGui/QLineEdit>

#include <QtGui/QStandardItemModel>
#include <QtGui/QItemSelection>
#include <QtGui/QItemSelectionModel>
#include <QtGui/QAbstractItemView>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeWidgetItem>


#include "airports/airportswidget.h"



AirportsWidget::AirportsWidget(QWidget *parent) :
    QWidget(parent)
{

    //* Main Layout
	QGridLayout *mainLayout = new QGridLayout();
    setLayout(mainLayout);
	mainLayout->setSpacing(10);
	int m = 10;
	mainLayout->setContentsMargins(m,m,m,m);

	//=====================================
	//** Top Option Buttons
	QHBoxLayout *layoutTop = new QHBoxLayout();
	mainLayout->addLayout(layoutTop,0,0,1,2);

	QButtonGroup *buttonGroupUse = new QButtonGroup(this);
	buttonGroupUse->setExclusive(true);
	connect(buttonGroupUse, SIGNAL(buttonClicked(int)), this, SLOT(on_buttonGroupUse()));

	radioButtonUseDefault = new QRadioButton("Default (KSFO)");
	radioButtonUseDefault->setChecked(true);
	layoutTop->addWidget(radioButtonUseDefault);
	buttonGroupUse->addButton(radioButtonUseDefault);

	radioButtonUseAirport = new QRadioButton("Start at Airport");
	layoutTop->addWidget(radioButtonUseAirport);
	buttonGroupUse->addButton(radioButtonUseAirport);

	radioButtonUseCoordinates = new QRadioButton("Start at Coordinates");
	layoutTop->addWidget(radioButtonUseCoordinates);
	buttonGroupUse->addButton(radioButtonUseCoordinates);

	layoutTop->addStretch(10);


	groupBoxAirport = new QGroupBox(this);
	groupBoxAirport->setTitle("Airport Details");
	mainLayout->addWidget(groupBoxAirport, 1, 0);

	QVBoxLayout *airportsLayout = new QVBoxLayout();
	groupBoxAirport->setLayout(airportsLayout);
	//layoutAirport->setLayout(airportsLayout);
	airportsLayout->setContentsMargins(10,10,10,10);
	airportsLayout->setSpacing(10);


	//==================================================================
	//** Airports Top Bar
	QHBoxLayout *layoutAptTopBar = new QHBoxLayout();
	layoutAptTopBar->setContentsMargins(0,0,0,0);
	airportsLayout->addLayout(layoutAptTopBar);

    //** Filter Code
	layoutAptTopBar->addWidget(new QLabel(tr("Filter").append(":  ")));

	//** Filter Buttons - TODO
	 /*
    buttGroupFilter = new QButtonGroup(this);
    buttGroupFilter->setExclusive(true);
    connect(buttGroupFilter, SIGNAL(buttonClicked(QAbstractButton*)),
            this,           SLOT(on_update_filter())
    );

    QToolButton *buttAll = new QToolButton();
    treeToolbar->addWidget(buttAll);
    buttViewGroup->addButton(buttAll);
    buttAll->setText("All");
    buttAll->setCheckable(true);
    buttAll->setIcon(QIcon(":/icons/pink"));
    buttAll->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    */
    //** Code Filter
	/*
    QRadioButton *buttCode = new QRadioButton();
    treeToolbar->addWidget(buttCode);
    buttGroupFilter->addButton(buttCode);
    buttCode->setText("Code");
	buttCode->setProperty("column", QVariant(C_ICAO));

	// Name Filter
    QRadioButton *buttName = new QRadioButton();
    treeToolbar->addWidget(buttName);
    buttGroupFilter->addButton(buttName);
    buttName->setText("Name");
    buttName->setProperty("column", QVariant(C_NAME));
    buttName->setChecked(true);
	*/

    //** Find Text
    txtAirportsFilter = new QLineEdit();
	layoutAptTopBar->addWidget(txtAirportsFilter);
    txtAirportsFilter->setFixedWidth(100);
	connect(txtAirportsFilter, SIGNAL(textChanged(QString)), this, SLOT(on_update_filter()));

	layoutAptTopBar->addStretch(20);


	QPushButton *buttonRefreshTree = new QPushButton(this);
	layoutAptTopBar->addWidget(buttonRefreshTree);
	buttonRefreshTree->setText("Reload");
	buttonRefreshTree->setToolTip("Scan directories and reload cache");
	buttonRefreshTree->setIcon(QIcon(":/icon/refresh"));
	//buttonRefreshTree->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	buttonRefreshTree->setFlat(true);
	connect(buttonRefreshTree, SIGNAL(clicked()), this, SLOT(on_refresh_clicked()) );



	//*==============================================================================
	//** Airports Splitter
	QSplitter *splitter = new QSplitter(this);
	airportsLayout->addWidget(splitter);


	//************************************************************************************************
	//** Left
	QWidget *leftWidget = new QWidget();
	splitter->addWidget(leftWidget);
	QVBoxLayout *leftLayout = new QVBoxLayout();
	leftLayout->setContentsMargins(0,0,0,0);
	leftLayout->setSpacing(0);
	leftWidget->setLayout(leftLayout);

    //******************************************************
    //** Models
    model = new QStandardItemModel(this);
    model->setColumnCount(2);
    QStringList headerLabelsList;
	headerLabelsList << "Location" << "ICAO"  << "Twr" <<  "Elevation" << "Name" << "Path";
    model->setHorizontalHeaderLabels(headerLabelsList);

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn( C_NAME );

    //******************************************************
    //**  Tree
	treeViewAirports = new QTreeView(this);
	leftLayout->addWidget(treeViewAirports, 1, 0);
	treeViewAirports->setModel(proxyModel);

	treeViewAirports->setUniformRowHeights(true);
	treeViewAirports->setAlternatingRowColors(true);
	treeViewAirports->setRootIsDecorated(false);
	treeViewAirports->setSortingEnabled(true);
	treeViewAirports->sortByColumn(C_NAME, Qt::AscendingOrder);
	treeViewAirports->setSelectionMode(QAbstractItemView::SingleSelection);
	treeViewAirports->setSelectionBehavior(QAbstractItemView::SelectRows);
	treeViewAirports->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //** Deaders and columns
	treeViewAirports->header()->setStretchLastSection(true);
	treeViewAirports->setColumnHidden(C_ELEVATION, true);
	treeViewAirports->setColumnHidden(C_TOWER, true);
	treeViewAirports->setColumnHidden(C_FAV, true);
	treeViewAirports->setColumnHidden(C_XML, true);
	treeViewAirports->setColumnHidden(C_NAME, true);
	treeViewAirports->setColumnWidth(C_FAV, 50);
	treeViewAirports->setColumnWidth(C_ICAO, 80);
	treeViewAirports->setColumnWidth(C_TOWER, 50);
	treeViewAirports->setColumnWidth(C_ELEVATION, 80);

	connect( treeViewAirports->selectionModel(),
             SIGNAL( selectionChanged (const QItemSelection&, const QItemSelection&) ),
			 SLOT( on_aiport_selection_changed(const QItemSelection&, const QItemSelection&) )
    );

	//connect(treeViewAirports,
    //        SIGNAL(clicked(QModelIndex)),
    //        this, SLOT(on_tree_clicked(QModelIndex))
    //);

	statusBarAirports = new QStatusBar();
	statusBarAirports->setSizeGripEnabled(false);
	leftLayout->addWidget(statusBarAirports);
	statusBarAirports->showMessage("Idle");

	//progressAirportsLoad = new QProgressBar();
	//statusBarTree->addPermanentWidget(progressAirportsLoad);


    //************************************************************************************************
    //** Right
    QWidget *rightWidget = new QWidget();
    splitter->addWidget(rightWidget);
	QVBoxLayout *rightLayout = new QVBoxLayout();
	rightWidget->setLayout(rightLayout);
	rightLayout->setContentsMargins(0,0,0,0);
	rightLayout->setSpacing(0);


    //*** Runways Tree
    treeWidgetRunways = new QTreeWidget();
	rightLayout->addWidget(treeWidgetRunways, 3);
    treeWidgetRunways->setAlternatingRowColors(true);
	treeWidgetRunways->setRootIsDecorated(true);
    QTreeWidgetItem *headerItem = treeWidgetRunways->headerItem();
    headerItem->setText(0, tr("Runway"));
    headerItem->setText(1, tr("Width"));
    headerItem->setText(2, tr("Length"));
    headerItem->setText(3, tr("Lat"));
    headerItem->setText(4, tr("Lng"));
    headerItem->setText(5, tr("Alignment"));
	treeWidgetRunways->setColumnHidden(1,true);
	treeWidgetRunways->setColumnHidden(2,true);
	treeWidgetRunways->setColumnHidden(3,true);
	treeWidgetRunways->setColumnHidden(4,true);
	treeWidgetRunways->setColumnHidden(5,true);
	/*
	connect( treeViewAirports->selectionModel(),
             SIGNAL( selectionChanged (const QItemSelection&, const QItemSelection&) ),
			 SLOT( on_aiport_selection_changed(const QItemSelection&, const QItemSelection&) )
    );
	*/

	connect( treeViewAirports->selectionModel(),
			 SIGNAL( currentRowChanged(QModelIndex,QModelIndex) ),
			 SLOT( on_aiport_row_changed(QModelIndex, QModelIndex) )
	);

	statusBarRunways = new QStatusBar();
	statusBarRunways->setSizeGripEnabled(false);
	rightLayout->addWidget(statusBarRunways);
	statusBarAirports->showMessage("");


	splitter->setStretchFactor(0, 2);
	splitter->setStretchFactor(1, 2);



	//====================================================================
	//** Use Coordinates
	//====================================================================


	groupBoxUseCoordinates = new QGroupBox(this);
	groupBoxUseCoordinates->setTitle("Coordinates");
	mainLayout->addWidget(groupBoxUseCoordinates, 1, 2);
	QVBoxLayout *layoutCoordinates = new QVBoxLayout();
	groupBoxUseCoordinates->setLayout(layoutCoordinates);


	layoutCoordinates->addWidget(new QLabel("Latitude"));
	txtLat = new QLineEdit();
	layoutCoordinates->addWidget(txtLat);

	layoutCoordinates->addSpacing(10);
	layoutCoordinates->addWidget(new QLabel("Longtitude"));
	txtLng = new QLineEdit();
	layoutCoordinates->addWidget(txtLng);

	layoutCoordinates->addSpacing(10);
	layoutCoordinates->addWidget(new QLabel("Altitude"));
	txtAltitude = new QLineEdit();
	layoutCoordinates->addWidget(txtAltitude);

	layoutCoordinates->addSpacing(10);
	layoutCoordinates->addWidget(new QLabel("Heading"));
	txtHeading = new QLineEdit();
	layoutCoordinates->addWidget(txtHeading);

	layoutCoordinates->addSpacing(10);
	layoutCoordinates->addWidget(new QLabel("Roll"));
	txtRoll = new QLineEdit();
	layoutCoordinates->addWidget(txtRoll);

	layoutCoordinates->addSpacing(10);
	layoutCoordinates->addWidget(new QLabel("Pitch"));
	txtPitch = new QLineEdit();
	layoutCoordinates->addWidget(txtPitch);

	layoutCoordinates->addSpacing(10);
	layoutCoordinates->addWidget(new QLabel("Airspeed"));
	txtAirspeed = new QLineEdit();
	layoutCoordinates->addWidget(txtAirspeed);


	layoutCoordinates->addStretch(20);

	on_buttonGroupUse();
}

void AirportsWidget::initialize(){
	QStringList airports = settings.value("cache/airports").toStringList();
	if(airports.count() == 0){
		scan_airports_xml();
	}
	load_airports_tree();
}

//============================================================================
//** Scan XML's for airports
//============================================================================
void AirportsWidget::scan_airports_xml(){
	int c = 0;
	int found = 0;
	QProgressDialog progress("Loading Airports to Cache", "Cancel", 0, 0, this);
	progress.setWindowModality(Qt::WindowModal);

	QString directory = settings.airports_path();

	QStringList airportsList;
	QString entry;

	QDirIterator it(directory, QDirIterator::Subdirectories);
	while (it.hasNext()) {
		progress.setValue(c);
		entry = it.next();
		if(entry.endsWith(".threshold.xml") ){
			QFileInfo info(entry);
			airportsList << QString("%1~|~%2").arg(	entry,
													info.fileName().split(".").at(0)
												);
			found++;
		}
		c++;
		if(c % 100 == 0){
			QString str = QString("%1 airports found").arg(found);
			statusBarAirports->showMessage(str);
		}
		if(progress.wasCanceled()){
			return;
		}
	}

	settings.setValue("cache/airports", airportsList);
	settings.sync();
	progress.hide();
}
//============================================================================
//** Load Airports Tree
//============================================================================
void AirportsWidget::load_airports_tree(){

	model->removeRows(0, model->rowCount());
	treeViewAirports->setUpdatesEnabled(false);
	QStringList airports = settings.value("cache/airports").toStringList();
	for (int i = 0; i < airports.size(); ++i){
		QStringList airport = airports.at(i).split("~|~");
		int new_row_index = model->rowCount();
		model->insertRow(new_row_index);


		QStandardItem *itemIcao = new QStandardItem();
		QFont font = itemIcao->font();
		font.setFamily("monospace");
		itemIcao->setFont(font);
		itemIcao->setText(airport.at(1));
		model->setItem(i, C_ICAO, itemIcao);


		QStandardItem *itemXml = new QStandardItem();
		itemXml->setText(airport.at(0));
		model->setItem(new_row_index, C_XML, itemXml);

		/*
		QStandardItem *itemAirportName = new QStandardItem();
		itemAirportName->setText(query.value(1).toString());
		model->setItem(new_row_index, C_NAME, itemAirportName);

		QStandardItem *itemAirportTower = new QStandardItem();
		itemAirportTower->setText(query.value(2).toString());
		model->setItem(new_row_index, C_TOWER, itemAirportTower);

		QStandardItem *itemAirportElevation = new QStandardItem();
		itemAirportElevation->setText(query.value(3).toString());
		model->setItem(new_row_index, C_ELEVATION, itemAirportElevation);
		*/
	}

	statusBarAirports->showMessage( QString("%1 airports").arg(model->rowCount()) );

	QList<QStandardItem *> items = model->findItems("KSFO", Qt::MatchExactly, C_ICAO);
	if(items.count() > 0){
		QModelIndex srcIdx = model->indexFromItem(items[0]);
		QModelIndex proxIdx = proxyModel->mapFromSource(srcIdx);
		treeViewAirports->selectionModel()->select(proxIdx, QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
		treeViewAirports->scrollTo(proxIdx, QAbstractItemView::PositionAtCenter);
		load_runways( model->item(srcIdx.row(), C_XML)->text() );
	}
	treeViewAirports->setUpdatesEnabled(true);
}


//**********************************************
//*** Update Filter
void AirportsWidget::on_update_filter(){

   /* LATER TODO int column = buttGroupFilter->checkedButton()->property("column").toInt();
		proxyModel->setFilterKeyColumn( column ); */
    proxyModel->setFilterFixedString( txtAirportsFilter->text() );
}



//**********************************************
//*** Airport Row Clicked = Show Runways
void AirportsWidget::on_aiport_selection_changed(const QItemSelection&, const QItemSelection&){
}
void AirportsWidget::on_aiport_row_changed(QModelIndex current, QModelIndex previous){
	Q_UNUSED(previous);
    treeWidgetRunways->model()->removeRows(0,
                                            treeWidgetRunways->model()->rowCount()
                                            );

	//QModelIndex proxyIndex =  treeViewAirports->selectionModel()->selectedRows(C_ICAO).first();
	QModelIndex proxyIndex =  proxyModel->index(current.row(), C_XML);
    if(!proxyIndex.isValid()){
		//emit set_arg("remove", "--airport=", "");
		return;
    }
    QModelIndex srcIndex = proxyModel->mapToSource(proxyIndex);
	//QString airportXmlFile = model->item(srcIndex.row(), C_XML)->text();
	load_runways(model->item(srcIndex.row(), C_XML)->text());
}
void AirportsWidget::load_runways(QString airportXmlFile){

	int i;

	//*** Load Runways
	QStringList runwayList;
	QFile rwyfile(airportXmlFile);
	if (rwyfile.open(QIODevice::ReadOnly)) {

		QXmlStreamReader rwyreader(&rwyfile);
		QXmlStreamReader::TokenType tokenType;

		while ((tokenType = rwyreader.readNext()) != QXmlStreamReader::EndDocument) {
			if (rwyreader.name() == "rwy") {
				runwayList << rwyreader.readElementText();
			}
		}
		rwyfile.close();
	}
	runwayList.removeDuplicates();
	runwayList.sort();

	QTreeWidgetItem *runwaysParent = new QTreeWidgetItem();
	runwaysParent->setText(0, "Runways" );
	runwaysParent->setIcon(0, QIcon(":/icon/folder"));
	treeWidgetRunways->addTopLevelItem(runwaysParent);
	treeWidgetRunways->setItemExpanded(runwaysParent, true);
	if(runwayList.count() == 0){
		QTreeWidgetItem *item = new QTreeWidgetItem(runwaysParent);
		item->setText(0, "None");
	}else{
		for(i =0; i < runwayList.count(); i++){
			QTreeWidgetItem *item = new QTreeWidgetItem(runwaysParent);
			item->setText(0, runwayList.at(i));
			item->setText(1, "runway");
		}
	}

	//*** Load Parking Positions
	QStringList parkingPositions;
	QString parkPosFile = QString(airportXmlFile);
	parkPosFile.chop(13); // strip the threshold.xml part
	parkPosFile.append("parking.xml");  //"groundnet.xml"

	QFile ppfile(parkPosFile);
	if(ppfile.open(QIODevice::ReadOnly)) {

		QXmlStreamReader ppreader(&ppfile);
		QXmlStreamReader::TokenType tokenType;

		while ((tokenType = ppreader.readNext()) != QXmlStreamReader::EndDocument) {
			if (ppreader.name() == "Parking") {
				QXmlStreamAttributes attributes = ppreader.attributes();

				if (attributes.value("type").toString() == "gate" && attributes.value("name").toString() != "Startup Location") {
					QString ppname = attributes.value("name").toString();
					QString ppnumber = attributes.value("number").toString();
					QString ppall ;
					ppall.append(ppname); ppall.append(ppnumber);
					parkingPositions << ppall;
				}

			}
		}

		ppfile.close();
	}
	parkingPositions.removeDuplicates();
	parkingPositions.sort();

	QTreeWidgetItem *parkingParent = new QTreeWidgetItem();
	parkingParent->setText(0, "Parking Stands" );
	parkingParent->setIcon(0, QIcon(":/icon/folder"));
	treeWidgetRunways->addTopLevelItem(parkingParent);
	treeWidgetRunways->setItemExpanded(parkingParent, true);
	if(parkingPositions.count() == 0){
		QTreeWidgetItem *item = new QTreeWidgetItem(parkingParent);
		item->setText(0, "None");
	}else{
		for(i =0; i < parkingPositions.count(); i++){
			QTreeWidgetItem *item = new QTreeWidgetItem(parkingParent);
			item->setText(0, parkingPositions.at(i));
			item->setText(1, "stand");
		}
	}
}


void AirportsWidget::on_refresh_clicked(){
	scan_airports_xml();
	load_airports_tree();
}

void AirportsWidget::on_buttonGroupUse(){
	groupBoxAirport->setEnabled(radioButtonUseAirport->isChecked());;
	groupBoxUseCoordinates->setEnabled(radioButtonUseCoordinates->isChecked());;
}



QStringList AirportsWidget::get_args(){
	QStringList args;
	//if (parkPosition->isEnabled() == true) {
	//	args << QString("--parkpos=").append(parkPosition->currentText());
	//}

	//* Coordinates
	if(radioButtonUseCoordinates->isChecked()){
		if( txtLat->text().length() > 0 and txtLng->text().length() > 0){
			args << QString("--lat=").append(txtLat->text());
			args << QString("--lon=").append(txtLng->text());
		}

	//* Airports
	}else{
		if(treeViewAirports->selectionModel()->hasSelection()){

		}

	}
	return args;
}

// Save Settings
void AirportsWidget::save_settings(){

	settings.setValue("use_coordinates", radioButtonUseCoordinates->isChecked());
	settings.setValue("lon", txtLat->text());
	settings.setValue("lat", txtLng->text());


	//settings.setValue("runway", runWay->currentText());
	//settings.setValue("parkPosition", parkPosition->currentText());

	settings.sync();
}


//=============================================================
// Load Settings
void AirportsWidget::load_settings(){
	//select_node(settings.value("aircraft").toString());
	/*if (settings.value("locationIcao").toString() != "") {
		QString locationICAOSet = settings.value("locationIcao").toString();
		locationIcao->insertItem(0, locationICAOSet);
		locationIcao->insertItem(1, "----");
	} else {
		locationIcao->insertItem(0, "KSFO");
		locationIcao->insertItem(1, "----");
		//timeoftheDay->setCurrentIndex(0);
	}*/
	radioButtonUseCoordinates->setChecked(settings.value("use_coordinates").toBool());
	txtLat->setText(settings.value("lat").toString());
	txtLng->setText(settings.value("lng").toString());
	//settings.setValue("airport", locationIcao->currentText());
}


QString AirportsWidget::validate(){

	if(radioButtonUseCoordinates->isChecked()){
		if(txtLat->text().trimmed().length() == 0){
			txtLat->setFocus();
			return QString("Need Latitude");
		}
		if(txtLng->text().trimmed().length() == 0){
			txtLng->setFocus();
			return QString("Need Longtitude");
		}
	}

	return QString();

}
