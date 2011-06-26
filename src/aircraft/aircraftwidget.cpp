
/**
	Started apr 2011 by: pete at freeflightsim.org
	then ..

	TODO: Convert QTreeWidget to QTreeView with A filterSortProxy Model

*/



#include <QtCore/QDebug>

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
	checkBoxUseDefault->setText("Use Default");
	treeTopBar->addWidget(checkBoxUseDefault);
	connect(checkBoxUseDefault, SIGNAL(clicked()), this, SLOT(on_use_default_clicked()));

	treeTopBar->addSpacing(10);

	//==========================
	//= Filter tabs
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

	//= Get the thumbnail image
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
		outLog("*** FGx reports: No aircraft selected (maybe no list), and [x] use default not selected. ***");
	}
	return QString();
}


//=============================================================
// Load Airaft To Tree

void AircraftWidget::on_reload_db_cache(){
	treeWidget->model()->removeRows(0, treeWidget->model()->rowCount());

	//* scan Airaft dirs and save in file
	scan_xml_sets();
	load_tree();
}

void AircraftWidget::load_tree(){
	int c =0;

		
	QString currAero = selected_aircraft();
	treeWidget->setUpdatesEnabled(false);
	treeWidget->model()->removeRows(0, treeWidget->model()->rowCount());

	QTreeWidgetItem *parentItem = new QTreeWidgetItem();

	int view = tabsView->currentIndex();
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

	select_node(currAero);
	QString str = QString("%1 aircraft").arg(c);
	statusBarAero->showMessage(str);
	outLog("load_tree: with "+str);
}

//=============================================================
// Initialize
void AircraftWidget::initialize(){

	static bool first_load_done = false;
	if(first_load_done){
		return;
	}

	load_tree();
	select_node(mainObject->settings->value("aircraft").toString());
	first_load_done = true;
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
	} else {
		args << QString("--aircraft=%1").arg(selected_aircraft());
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





//========================================================
//*** Walk XML - sets
//========================================================

/* This function walks the /Aircraft/ directory
 its find files in a directory maching "-set.xml"
   and then quires the xml for vars
   TODO find keys, include ETC
*/

void AircraftWidget::scan_xml_sets(){

	int c = 0;
	int found = 0;



	//= Cache File
	QFile cacheFile( mainObject->settings->data_file("aircraft.txt") );
	if(!cacheFile.open(QIODevice::WriteOnly | QIODevice::Text)){
		//qDebug() << "TODO Open error cachce file=";
		return;
	}

	QProgressDialog progress(tr("Scanning aircraft directory ..."), tr("Cancel"), 0, 0);
	progress.setWindowModality(Qt::WindowModal);
	progress.show();


	QTextStream out(&cacheFile);

	//= Get files Entries from Aircaft/ directory
	QDir aircraftDir( mainObject->settings->aircraft_path() );
	aircraftDir.setFilter( QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	qDebug() << "aircraft directory path: " << mainObject->settings->aircraft_path();

	QStringList entries = aircraftDir.entryList();
	progress.setMaximum(entries.size());

	for( QStringList::ConstIterator entry=entries.begin(); entry!=entries.end(); ++entry ){

		// Filter out default dir names, should be a QDir name filter?
		if (*entry != "Instruments" &&  *entry != "Instruments-3d" && *entry != "Generic") {

			progress.setValue(progress.value() + 1);

			//** get the List of *-set.xml files in dir
			QDir dir( mainObject->settings->aircraft_path(*entry) );
			QStringList filters;
			filters << "*-set.xml";
			QStringList list_xml = dir.entryList(filters);

			if(list_xml.count() > 0){ // << Scan MOdels
				QString directory;
				QString description;
				QString author;
				QString fdm;
				QString xml_file;
				QString aero;

				//** Add Path Node
				directory = QString(*entry);
				//** Add Models
				for (int i = 0; i < list_xml.size(); ++i){

					xml_file = QString(list_xml.at(i));
					aero = QString(xml_file);
					aero.chop(8);

					//*=parse the Xml file - f&*& long winded
					QString file_path =  mainObject->settings->aircraft_path(*entry);
					file_path.append("/");
					file_path.append(list_xml.at(i));
					QFile xmlFile( file_path);
					if (xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)){

						/* The file content is converted to UTF-8.
							 Some files are Windows, encoding and throw error with QxmlQuery etc
							 Its a hack and don't quite understand whats happening.. said pedro
						*/
						QString xmlString = QString(xmlFile.readAll()).toUtf8();

						QXmlQuery query;
						query.setFocus(xmlString);
						//query.setFocus(&xmlFile); << Because file is not QTF8 using sting instead
						query.setQuery("PropertyList/sim");
						if (query.isValid()){

							QString res;
							query.evaluateTo(&res);
							xmlFile.close();

							QDomDocument dom;
							dom.setContent("" + res + "");
							QDomNodeList nodes = dom.elementsByTagName("sim");

							QDomNode n = nodes.at(0);
							description = n.firstChildElement("description").text();
							author = n.firstChildElement("author").text().trimmed().replace(("\n"),"");
							fdm = n.firstChildElement("flight-model").text();
						} /* !query.isValid() */
					} /*  xmlFile.open() */

					QStringList lines;
					lines  << directory << aero << xml_file << description << fdm << author << file_path;
					out << lines.join("\t") << "\n";

					found++;

					if(progress.wasCanceled()){
						qDebug() << "Progress cancelled!";
						progress.hide();
						break; // TODO ?? why..
					}
					c++;
				}

			} /* list_xml.count() > 0 */
		} /* entry != INstruments etc */
	} /* loop entries.() */

	cacheFile.close();
	progress.hide();
}
