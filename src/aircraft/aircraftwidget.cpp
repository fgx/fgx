
/**
  This widget displays a list of aircraft,
    does a background call to a process that executes FG
  */


#include <QtXmlPatterns/QXmlQuery>
#include <QtXml/QDomDocument>

#include <QtCore/QDebug>

#include <QtCore/QProcess>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QDir>

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

#include "aircraft/aircraftwidget.h"





AircraftWidget::AircraftWidget(QWidget *parent) :
    QWidget(parent)
{

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


    //** Toolbar
    QToolBar *treeToolbar = new QToolBar();
    treeLayout->addWidget(treeToolbar);
    treeToolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);


    //******************************************************
    //** Filter Buttons
	buttViewGroup = new QButtonGroup(this);
    buttViewGroup->setExclusive(true);
    connect(buttViewGroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(on_view_button_clicked(QAbstractButton*))
    );

	QToolButton *buttList = new QToolButton();
	buttList->setText("List");
	buttList->setCheckable(true);
	buttList->setIcon(QIcon(":/icon/pink"));
	buttList->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	buttList->setChecked(true);
	treeToolbar->addWidget(buttList);
	buttViewGroup->addButton(buttList, V_LIST);


	QToolButton *buttNested = new QToolButton();
	buttNested->setText("Nested");
	buttNested->setCheckable(true);
	buttNested->setIcon(QIcon(":/icon/purple"));
	buttNested->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	treeToolbar->addWidget(buttNested);
	buttViewGroup->addButton(buttNested, V_NESTED);

	treeToolbar->addSeparator();

	QAction *actionRefreshTree = new QAction(this);
	treeToolbar->addAction(actionRefreshTree);
	actionRefreshTree->setText("Reload");
	actionRefreshTree->setIcon(QIcon(":/icon/refresh"));
	connect(actionRefreshTree, SIGNAL(triggered()), this, SLOT(on_refresh_cache()) );

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
    aeroTabs->addTab(aeroControlWidget, tr("Control"));
    QVBoxLayout *aeroControlLayout = new QVBoxLayout();
    aeroControlWidget->setLayout(aeroControlLayout);

    QCheckBox *chkEnableAutoCoordination = new QCheckBox();
    aeroControlLayout->addWidget(chkEnableAutoCoordination);
    chkEnableAutoCoordination->setText(tr("Enable Auto-Cordination"));
	connect(chkEnableAutoCoordination, SIGNAL(clicked(bool)), this, SLOT(on_auto_coordination(bool)));
    //aeroLayout->addStretch(10);

    QToolBar *aeroToolbar = new QToolBar();
    aeroLayout->addWidget(aeroToolbar);
    aeroToolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QAction *actionReloadAero = new QAction(this);
    aeroToolbar->addAction(actionReloadAero);
    actionReloadAero->setText("Reload");
    actionReloadAero->setIcon(QIcon(":/icons/refresh"));
    //connect(actionReloadAero, SIGNAL(triggered()), this, SLOT(load_aircraft()) );

    //***********************************
    //** Setup

	splitter->setCollapsible(0, false);
	splitter->setCollapsible(1, false);
	splitter->setStretchFactor(0, 50);
	splitter->setStretchFactor(1, 1);

	initialize();
	//load_aircraft_xml_set();
}




void AircraftWidget::on_view_button_clicked(QAbstractButton *button){
	Q_UNUSED(button);
	if(buttViewGroup->checkedId() == V_LIST){
		buttViewGroup->button(V_LIST)->setIcon(QIcon(":/icon/pink"));
		buttViewGroup->button(V_NESTED)->setIcon(QIcon(":/icon/purple"));
	}else{
		buttViewGroup->button(V_LIST)->setIcon(QIcon(":/icon/purple"));
		buttViewGroup->button(V_NESTED)->setIcon(QIcon(":/icon/pink"));
	}
	load_tree();
}



//==========================================================================
// Aircraft Selected
//==========================================================================
void AircraftWidget::on_tree_selection_changed(){

	QTreeWidgetItem *item = treeWidget->currentItem();
	if(!item){
		return;
	}

	//** Check there is item and its a model
	if(item->text(C_AERO).length() == 0){
		aeroImageLabel->clear();
		emit set_arg("remove", "--aircraft=", "");
		return;
	}

	emit set_arg("set", "--aircraft=", item->text(C_AERO));

	QString thumb_file = QString("%1/%2/%3/thumbnail.jpg").arg( settings.aircraft_path(),
																item->text(C_DIR),
																item->text(C_AERO));
	if(QFile::exists(thumb_file)){
		QPixmap aeroImage(thumb_file);
		if(!aeroImage.isNull()){
			aeroImageLabel->setPixmap(aeroImage);
		}
	}else{
		aeroImageLabel->clear();
	}
}




//=====================================
// Auto Coordination
void AircraftWidget::on_auto_coordination(bool state){
	emit set_arg(state ? "set" : "remove", "--enable-auto-coordination", ""); //** --disable-auto-coordination is default
}













//========================================================
//** Load Aircraft Shell
void AircraftWidget::load_aircraft_shell(){

	QString command = settings.fgfs_path();
	command.append(" --fg-root=").append(settings.fg_root());
	command.append(" --show-aircraft");

	QProcess *process = new QProcess(this);
	process->start(command);

	QStringList::Iterator it;
	QString line;
	int row_count=0;

	if(process->waitForStarted()){
			process->waitForFinished();
			QByteArray result =  process->readAllStandardOutput();
			QByteArray errorResult = process->readAllStandardError();

			//** The fgfs --show_a_mistake returns the "--help" and not an error output ;-(
			//* so only way to detect is to get "-available aircraft" as text
			QStringList lines = QString(result).split("\n");

			for ( it = lines.begin() ; it != lines.end() ; it++ ){

				line = it->simplified();

				//* Unless first item == Available aircraft: then its an error (messy)
				if(it == lines.begin()){
					if(line  != "Available aircraft:"){
						  //TODO emit("error")
						return;
					}else{
						//  first_line

					}
				}else{
					/*
					QStandardItem *modelItem = new QStandardItem();
					modelItem->setText( line.section( ' ', 0, 0 )); // first chars to space
					model->setItem(row_count, 0, modelItem);
					QStandardItem *descriptionItem = new QStandardItem();
					descriptionItem->setText( line.section( ' ', 1 )); // after first space
					model->setItem(row_count, 1, descriptionItem);
					*/
					row_count++;
				}
			}

	}
}

//========================================================
//*** Walk XML - sets
QStringList AircraftWidget::scan_xml_sets(){

	int c = 0;
	int found = 0;
	QProgressDialog progress("Loading Aircraft to Cache", "Cancel", 0, 0, this);
	progress.setWindowModality(Qt::WindowModal);

	QStringList aeroList;

	QDir aircraftDir( settings.aircraft_path() );
	aircraftDir.setFilter( QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	QStringList entries = aircraftDir.entryList();

	for( QStringList::ConstIterator entry=entries.begin(); entry!=entries.end(); ++entry ){

		// Filter out default dir names, should be a QDir name filter?
		if (*entry != "Instruments" &&  *entry != "Instruments-3d" && *entry != "Generic") {

			//** get the List of *-set.xml files in dir
			QDir dir( settings.aircraft_path(*entry) );
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

					//** parse the Xml file - fucking long winded
					QString file_path =  settings.aircraft_path(*entry);
					file_path.append("/");
					file_path.append(list_xml.at(i));
					QFile xmlFile( file_path);
					if (xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)){

						QXmlQuery query;
						query.setFocus(&xmlFile);
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
							author = n.firstChildElement("author").text();
							fdm = n.firstChildElement("flight-model").text();
						} /* !query.isValid() */
					} /*  xmlFile.open() */

					QString record = QString("%1~|~%2~|~%3~|~%4~|~%5~|~%6").arg(directory, xml_file, aero, fdm, description, author );
					aeroList << record;
					found++;

					if(c % 100 == 0){
						QString str = QString("%1 aircraft found").arg(found);
						statusBarAero->showMessage(str);
					}
					if(progress.wasCanceled()){
						break;
					}
					c++;
				} // <Add models

			}
		}
	}
	return aeroList;

}

//=============================================================
// Save Settings
void AircraftWidget::save_settings(){
	QTreeWidgetItem *item = treeWidget->currentItem();
	if(!item or item->text(C_AERO).length() == 0){
		return;
	}
	settings.setValue("aircraft", item->text(C_AERO) );
	settings.sync();
	qDebug() <<  "SAVE" << item->text(C_AERO);
}


//=============================================================
// Load Settings
void AircraftWidget::load_settings(){
	select_node(settings.value("aircraft").toString());
}

void AircraftWidget::select_node(QString aero){
	if(aero.length() == 0){
		return;
	}
	QList<QTreeWidgetItem*> items = treeWidget->findItems(aero, Qt::MatchExactly | Qt::MatchRecursive, C_AERO);
	qDebug() << aero;
	if(items.length() > 0){
		treeWidget->setCurrentItem(items[0]);
		treeWidget->scrollToItem(items[0]);
	}
}
QString AircraftWidget::aircraft(){
	QTreeWidgetItem *item = treeWidget->currentItem();
	qDebug() << item;
	if(!item or item->text(C_AERO).length() == 0){
		return "";
	}
	return item->text(C_AERO);
}

//=============================================================
// Validate
QString AircraftWidget::validate(){
	return QString();
}


//=============================================================
// Load Airraft To Tree

void AircraftWidget::on_refresh_cache(){
	treeWidget->model()->removeRows(0, treeWidget->model()->rowCount());
	scan_xml_sets();
	load_tree();
}

void AircraftWidget::load_tree(){
	int c =0;
	QString currAero = aircraft();
	treeWidget->setUpdatesEnabled(false);
	treeWidget->model()->removeRows(0, treeWidget->model()->rowCount());
	QString record;
	QString last_dir("");
	QTreeWidgetItem *parentItem;

	int view = buttViewGroup->checkedId();
	treeWidget->setColumnHidden(C_DIR, view == V_LIST);
	treeWidget->setRootIsDecorated(view == V_NESTED);

	 for (int i = 0; i < aeroList.size(); ++i) {
		record = aeroList.at(i);
		QStringList fields = record.split("~|~");

		if(view == V_LIST){
			parentItem = treeWidget->invisibleRootItem();
		}else if(last_dir != fields.at(C_DIR)){
			parentItem = new QTreeWidgetItem(treeWidget->invisibleRootItem());
			parentItem->setText( C_DIR, fields.at(0));
			parentItem->setIcon(C_DIR, QIcon(":/icon/folder"));
			treeWidget->addTopLevelItem(parentItem);
			treeWidget->setFirstItemColumnSpanned(parentItem, true);
			last_dir = fields.at(C_DIR);
		}

		//directory, xml_file, aero, fdm, description, author
		QTreeWidgetItem *aeroItem = new QTreeWidgetItem(parentItem);
		QString xml_path = QString("%1/%2").arg(fields.at(0), fields.at(1));
		aeroItem->setText(C_XML, xml_path);
		aeroItem->setText(C_AERO, fields.at(2));
		aeroItem->setIcon(C_AERO, QIcon(":/icon/aircraft"));
		aeroItem->setText(C_FDM, fields.at(3));
		aeroItem->setText(C_DESCRIPTION, fields.at(4));
		aeroItem->setText(C_AUTHOR, fields.at(5));
		c++;
	 }
	 treeWidget->sortByColumn(view == V_NESTED ? C_DIR : C_AERO, Qt::AscendingOrder);
	 treeWidget->setUpdatesEnabled(true);

	 select_node(currAero);
	 QString str = QString("%1 aircraft").arg(c);
	 statusBarAero->showMessage(str);
}

//=============================================================
// Initialize
void AircraftWidget::initialize(){
	aeroList = settings.value("aircraft_list").toStringList();
	if(aeroList.count() == 0){
		//assume its never been initialised
		scan_xml_sets();
	}
	aeroList = settings.value("aircraft_list").toStringList();
	load_tree();
}

//=============================================================
// Get Args
QStringList AircraftWidget::get_args(){

	QStringList args;

	if(aircraft().length() > 0){
		args << QString("--aircraft=%1").arg(aircraft());
	}


	return args;

}
