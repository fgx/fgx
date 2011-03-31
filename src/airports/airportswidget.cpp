

#include <QtCore/QDebug>
#include <QtCore/QString>
//#include <QtCore/QStringList>


// layouts
#include <QtGui/QVBoxLayout>
#include <QtGui/QSplitter>

#include <QtGui/QSizePolicy>
#include <QtGui/QFont>

#include <QtGui/QToolBar>
#include <QtGui/QToolButton>

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
	QVBoxLayout *layoutAirport = new QVBoxLayout();
	groupBoxAirport->setLayout(layoutAirport);

    QSplitter *splitter = new QSplitter(this);
	layoutAirport->addWidget(splitter);

    //************************************************************************************************
    //** Left
    QWidget *leftWidget = new QWidget();
    splitter->addWidget(leftWidget);
    QVBoxLayout *treeLayout = new QVBoxLayout();
    leftWidget->setLayout(treeLayout);
    treeLayout->setContentsMargins(0,0,0,0);
    treeLayout->setSpacing(0);

    //*************************************************
    //** Toolbar
    QToolBar *treeToolbar = new QToolBar();
    treeLayout->addWidget(treeToolbar);
    treeToolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    treeToolbar->layout()->setSpacing(10);

    //** Filter Code
    treeToolbar->addWidget(new QLabel(tr("Filter").append(":")));



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
    buttCode->setProperty("column", QVariant(C_CODE));


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
    treeToolbar->addWidget(txtAirportsFilter);
    txtAirportsFilter->setFixedWidth(100);
    connect(txtAirportsFilter,    SIGNAL(textChanged(QString)),
            this,           SLOT(on_update_filter())
    );

    treeToolbar->addSeparator();

    //** Add spacer widget to force right
    QWidget *spacerWidget = new QWidget();

    spacerWidget->setSizePolicy( QSizePolicy::Expanding , QSizePolicy::Minimum );
    treeToolbar->addWidget(spacerWidget);

    QAction *actionRefreshTree = new QAction(this);
    treeToolbar->addAction(actionRefreshTree);
	actionRefreshTree->setText("Reload");
	actionRefreshTree->setToolTip("Scan directories and reload cache");
	actionRefreshTree->setIcon(QIcon(":/icon/refresh"));
	connect(actionRefreshTree, SIGNAL(triggered()), this, SLOT(on_refresh_clicked()) );




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
    treeView = new QTreeView(this);
    treeLayout->addWidget(treeView);
    treeView->setModel(proxyModel);

    treeView->setUniformRowHeights(true);
    treeView->setAlternatingRowColors(true);
    treeView->setRootIsDecorated(false);
    treeView->setSortingEnabled(true);
    treeView->sortByColumn(C_NAME, Qt::AscendingOrder);
    treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //** Deaders and columns
    treeView->header()->setStretchLastSection(true);
	treeView->setColumnHidden(C_ELEVATION, true);
	treeView->setColumnHidden(C_TOWER, true);
	treeView->setColumnHidden(C_FAV, true);
	treeView->setColumnHidden(C_XML, true);
    treeView->setColumnWidth(C_FAV, 50);
    treeView->setColumnWidth(C_CODE, 80);
    treeView->setColumnWidth(C_TOWER, 50);
    treeView->setColumnWidth(C_ELEVATION, 80);

    connect( treeView->selectionModel(),
             SIGNAL( selectionChanged (const QItemSelection&, const QItemSelection&) ),
			 SLOT( on_aiport_selection_changed(const QItemSelection&, const QItemSelection&) )
    );

    //connect(treeView,
    //        SIGNAL(clicked(QModelIndex)),
    //        this, SLOT(on_tree_clicked(QModelIndex))
    //);

	statusBarTree = new QStatusBar();
    treeLayout->addWidget(statusBarTree);
    statusBarTree->showMessage("Idle");

	//progressAirportsLoad = new QProgressBar();
	//statusBarTree->addPermanentWidget(progressAirportsLoad);


    //************************************************************************************************
    //** Right
    QWidget *rightWidget = new QWidget();
    splitter->addWidget(rightWidget);
    QVBoxLayout *airportLayout = new QVBoxLayout();
    rightWidget->setLayout(airportLayout);
    airportLayout->setContentsMargins(0,0,0,0);
    airportLayout->setSpacing(0);


    //*** Runways Tree
    treeWidgetRunways = new QTreeWidget();
    airportLayout->addWidget(treeWidgetRunways, 3);
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
    connect( treeView->selectionModel(),
             SIGNAL( selectionChanged (const QItemSelection&, const QItemSelection&) ),
			 SLOT( on_aiport_selection_changed(const QItemSelection&, const QItemSelection&) )
    );
	*/

	connect( treeView->selectionModel(),
			 SIGNAL( currentRowChanged(QModelIndex,QModelIndex) ),
			 SLOT( on_aiport_row_changed(QModelIndex, QModelIndex) )
	);


    //** Map
    //map = new GoogleMapWidget(this);
    //airportLayout->addWidget(map, 10);

    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);




	groupBoxUseCoordinates = new QGroupBox(this);
	groupBoxUseCoordinates->setTitle("Start at Coordinates");
	groupBoxUseCoordinates->setCheckable(true);
	mainLayout->addWidget(groupBoxUseCoordinates);
	QVBoxLayout *layoutCoordinates = new QVBoxLayout();
	groupBoxUseCoordinates->setLayout(layoutCoordinates);


	initialize();

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

    show_progress(true);

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
		model->setItem(i, C_CODE, itemIcao);


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
	statusBarTree->showMessage( QString("%1 airports").arg(model->rowCount()) );
}


//**********************************************
//*** Update Filter
void AirportsWidget::on_update_filter(){

    int column = buttGroupFilter->checkedButton()->property("column").toInt();
    proxyModel->setFilterKeyColumn( column );
    proxyModel->setFilterFixedString( txtAirportsFilter->text() );
    treeView->sortByColumn(column);
}

//**********************************************
//*** Import Airports Dialog
void AirportsWidget::import_airports_dialog(){
	 //ImportAirportsWidget *imp = new ImportAirportsWidget(mainObject);
	 //if(imp->exec()){
	 //   load_airports();
	// }
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

	//QModelIndex proxyIndex =  treeView->selectionModel()->selectedRows(C_CODE).first();
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
