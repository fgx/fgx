
#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <QtCore/QString>
//#include <QtCore/QStringList>


// layouts
#include <QtGui/QVBoxLayout>
#include <QtGui/QSplitter>

#include <QtGui/QSizePolicy>
#include <QtGui/QFont>

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

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>



#include "airports/airportswidget.h"



AirportsWidget::AirportsWidget(QWidget *parent) :
    QWidget(parent)
{

    //* Main Layout
	QHBoxLayout *mainLayout = new QHBoxLayout();
    setLayout(mainLayout);
	mainLayout->setSpacing(10);
	int m = 10;
	mainLayout->setContentsMargins(m,m,m,m);


	groupBoxAirport = new QGroupBox(this);
	groupBoxAirport->setTitle("Start at Airport");
	groupBoxAirport->setCheckable(true);
	mainLayout->addWidget(groupBoxAirport);
	connect(groupBoxAirport, SIGNAL(clicked()), this, SLOT(on_groupbox_airports()));
	QVBoxLayout *layoutAirport = new QVBoxLayout();
	groupBoxAirport->setLayout(layoutAirport);

    QSplitter *splitter = new QSplitter(this);
	layoutAirport->addWidget(splitter);

    //************************************************************************************************
    //** Left
    QWidget *leftWidget = new QWidget();
    splitter->addWidget(leftWidget);
	QVBoxLayout *airportLayout = new QVBoxLayout();
	leftWidget->setLayout(airportLayout);
	airportLayout->setContentsMargins(0,0,0,0);
	airportLayout->setSpacing(0);

    //*************************************************
    //** Toolbar
	QHBoxLayout *treeBar = new QHBoxLayout();
	airportLayout->addLayout(treeBar);
   // treeToolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	//treeToolbar->layout()->setSpacing(10);

    //** Filter Code
	treeBar->addWidget(new QLabel(tr("Filter").append(":")));



    //***********************************s*******************
    //** Filter Buttons
    buttGroupFilter = new QButtonGroup(this);
    buttGroupFilter->setExclusive(true);
    connect(buttGroupFilter, SIGNAL(buttonClicked(QAbstractButton*)),
            this,           SLOT(on_update_filter())
    );
    /*
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


    //** Name Filter
    QRadioButton *buttName = new QRadioButton();
    treeToolbar->addWidget(buttName);
    buttGroupFilter->addButton(buttName);
    buttName->setText("Name");
    buttName->setProperty("column", QVariant(C_NAME));
    buttName->setChecked(true);
	*/

    //** Find Text
    txtAirportsFilter = new QLineEdit();
	treeBar->addWidget(txtAirportsFilter);
    txtAirportsFilter->setFixedWidth(100);
    connect(txtAirportsFilter,    SIGNAL(textChanged(QString)),
            this,           SLOT(on_update_filter())
    );

	treeBar->addStretch(20);


	QPushButton *buttonRefreshTree = new QPushButton(this);
	treeBar->addWidget(buttonRefreshTree);
	buttonRefreshTree->setText("Reload");
	buttonRefreshTree->setToolTip("Scan directories and reload cache");
	buttonRefreshTree->setIcon(QIcon(":/icon/refresh"));
	//buttonRefreshTree->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	buttonRefreshTree->setFlat(true);
	connect(buttonRefreshTree, SIGNAL(clicked()), this, SLOT(on_refresh_clicked()) );




    //******************************************************
    //** Models
    model = new QStandardItemModel(this);
    model->setColumnCount(2);
    QStringList headerLabelsList;
	headerLabelsList << "Location" << "Code"  << "Twr" <<  "Elevation" << "Name" << "Path";
    model->setHorizontalHeaderLabels(headerLabelsList);

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn( C_NAME );

    //******************************************************
    //**  Tree
	treeViewAirports = new QTreeView(this);
	airportLayout->addWidget(treeViewAirports);
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
	airportLayout->addWidget(statusBarAirports);
	statusBarAirports->showMessage("Idle");

	//progressAirportsLoad = new QProgressBar();
	//statusBarTree->addPermanentWidget(progressAirportsLoad);


    //************************************************************************************************
    //** Right
    QWidget *rightWidget = new QWidget();
    splitter->addWidget(rightWidget);
	QVBoxLayout *runwayLayout = new QVBoxLayout();
	rightWidget->setLayout(runwayLayout);
	runwayLayout->setContentsMargins(0,0,0,0);
	runwayLayout->setSpacing(0);

	//* blank to drop runways tree
	QHBoxLayout *hbSpacer = new QHBoxLayout();
	runwayLayout->addLayout(hbSpacer);
	hbSpacer->addWidget(new QLabel(" "));


    //*** Runways Tree
    treeWidgetRunways = new QTreeWidget();
	runwayLayout->addWidget(treeWidgetRunways, 3);
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
	runwayLayout->addWidget(statusBarRunways);
	statusBarAirports->showMessage("");

    //** Map
    //map = new GoogleMapWidget(this);
    //airportLayout->addWidget(map, 10);

	splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 1);



	//====================================================================
	//** Use Coordinates
	//====================================================================
	groupBoxUseCoordinates = new QGroupBox(this);
	groupBoxUseCoordinates->setTitle("Start at Coordinates");
	groupBoxUseCoordinates->setCheckable(true);
	mainLayout->addWidget(groupBoxUseCoordinates);
	connect(groupBoxUseCoordinates, SIGNAL(clicked()), this, SLOT(on_groupbox_use_coordinates()));
	QVBoxLayout *layoutCoordinates = new QVBoxLayout();
	groupBoxUseCoordinates->setLayout(layoutCoordinates);


	layoutCoordinates->addWidget(new QLabel("Latitude"));
	txtLat = new QLineEdit();
	layoutCoordinates->addWidget(txtLat);

	layoutCoordinates->addSpacing(10);
	layoutCoordinates->addWidget(new QLabel("Longtitude"));
	txtLng = new QLineEdit();
	layoutCoordinates->addWidget(txtLng);

	//initialize();

	//***** Qt Has no Show event for a form, so we need to present Widgets first
	//** and then initialise. THis is achieved with a timer that triggers in a moment
	QTimer::singleShot(500, this, SLOT(initialize()));

}

void AirportsWidget::show_progress(bool state){
	/*
    if(state){
        progressAirportsLoad->setRange(0,0);
        progressAirportsLoad->setValue(0);
        progressAirportsLoad->show();
        progressAirportsLoad->repaint();
    }else{
        progressAirportsLoad->setRange(0,100);
        progressAirportsLoad->hide();
    }
	*/
}

void AirportsWidget::initialize(){
	QStringList airports = settings.value("cache/airports").toStringList();
	if(airports.count() == 0){
		scan_airports_xml();
	}
	load_tree();
}

//============================================================================
//** Scan XML's for airports
//============================================================================
void AirportsWidget::scan_airports_xml(){
	qDebug() << "Scanning";
	QProgressDialog progress;
	progress.show();

	QString directory = settings.airports_path();

	QStringList airportsList;
	QString entry;

	QDirIterator it(directory, QDirIterator::Subdirectories);
	while (it.hasNext()) {
		//files << it.next();
		entry = it.next();
		if(entry.endsWith(".threshold.xml") ){
			QFileInfo info(entry);
			airportsList << QString("%1~|~%2").arg(	entry,
													info.fileName().split(".").at(0)
												);
		}
	}

	settings.setValue("cache/aiports", airportsList);
	progress.hide();
	qDebug() << "Scanning DONE";
}
//============================================================================
//** Load Airports Tree
//============================================================================
void AirportsWidget::load_tree(){

	model->removeRows(0, model->rowCount());

	QStringList airports = settings.value("cache/aiports").toStringList();
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
	//qDebug() << "set row count";
	//show_progress(false);
	statusBarAirports->showMessage( QString("%1 airports").arg(model->rowCount()) );

	QList<QStandardItem *> items = model->findItems("KSFO", Qt::MatchExactly, C_ICAO);
	if(items.count() > 0){
		QModelIndex srcIdx = model->indexFromItem(items[0]);
		QModelIndex proxIdx = proxyModel->mapFromSource(srcIdx);
		treeViewAirports->selectionModel()->select(proxIdx, QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);

	}
}


//**********************************************
//*** Update Filter
void AirportsWidget::on_update_filter(){

    int column = buttGroupFilter->checkedButton()->property("column").toInt();
    proxyModel->setFilterKeyColumn( column );
    proxyModel->setFilterFixedString( txtAirportsFilter->text() );
	treeViewAirports->sortByColumn(column);
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
	QString airportXmlFile = model->item(srcIndex.row(), C_XML)->text();
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

	QTreeWidgetItem *runwaysItem = new QTreeWidgetItem();
	runwaysItem->setText(0, "Runways" );
	treeWidgetRunways->addTopLevelItem(runwaysItem);
	treeWidgetRunways->setItemExpanded(runwaysItem, true);
	if(runwayList.count() == 0){
		QTreeWidgetItem *item = new QTreeWidgetItem(runwaysItem);
		item->setText(0, "None");
	}else{
		for(i =0; i < runwayList.count(); i++){
			QTreeWidgetItem *item = new QTreeWidgetItem(runwaysItem);
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

	QTreeWidgetItem *parkingsItem = new QTreeWidgetItem();
	parkingsItem->setText(0, "Parking Stands" );
	treeWidgetRunways->addTopLevelItem(parkingsItem);
	treeWidgetRunways->setItemExpanded(parkingsItem, true);
	if(parkingPositions.count() == 0){
		QTreeWidgetItem *item = new QTreeWidgetItem(parkingsItem);
		item->setText(0, "None");
	}else{
		for(i =0; i < parkingPositions.count(); i++){
			QTreeWidgetItem *item = new QTreeWidgetItem(parkingsItem);
			item->setText(0, parkingPositions.at(i));
			item->setText(1, "stand");
		}
	}
}


void AirportsWidget::on_refresh_clicked(){
	scan_airports_xml();
	load_tree();
}

void AirportsWidget::on_groupbox_airports(){
	qDebug() << "on_groupbox_airports " << groupBoxAirport->isChecked();
	if(groupBoxAirport->isChecked()){
		groupBoxUseCoordinates->setChecked(false);
	}
}
void AirportsWidget::on_groupbox_use_coordinates(){
	qDebug() << "on_groupbox_use_coordinates " << groupBoxUseCoordinates->isChecked();
	if(groupBoxUseCoordinates->isChecked()){
		groupBoxAirport->setChecked(false);
	}
}



QStringList AirportsWidget::get_args(){
	QStringList args;
	//if (parkPosition->isEnabled() == true) {
	//	args << QString("--parkpos=").append(parkPosition->currentText());
	//}

	//* Coordinates
	if(groupBoxUseCoordinates->isChecked()){
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

	settings.setValue("use_coordinates", groupBoxUseCoordinates->isChecked());
	settings.setValue("lon", txtLat->text());
	settings.setValue("lat", txtLng->text());


	//settings.setValue("runway", runWay->currentText());
	//settings.setValue("parkPosition", parkPosition->currentText());

	settings.sync();
	//qDebug() <<  "SAVE" << item->text(C_AERO);
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
	groupBoxUseCoordinates->setChecked(settings.value("use_coordinates").toBool());
	txtLat->setText(settings.value("lat").toString());
	txtLng->setText(settings.value("lng").toString());
	//settings.setValue("airport", locationIcao->currentText());
}


bool AirportsWidget::validate(){

	if(groupBoxUseCoordinates->isChecked()){
		if(txtLat->text().trimmed().length() == 0){
			txtLat->setFocus();
			return false;
		}
		if(txtLng->text().trimmed().length() == 0){
			txtLng->setFocus();
			return false;
		}
	}

	return true;

}
