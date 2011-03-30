
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

#include <QtGui/QToolBar>
#include <QtGui/QToolButton>
#include <QtGui/QAbstractButton>
#include <QtGui/QButtonGroup>

#include <QtGui/QAction>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QTabWidget>

#include <QtGui/QTreeWidgetItem>

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
    
    QAction *actionRefreshTree = new QAction(this);
    treeToolbar->addAction(actionRefreshTree);
	actionRefreshTree->setText("Reload");
	actionRefreshTree->setIcon(QIcon(":/icon/refresh"));
    connect(actionRefreshTree, SIGNAL(triggered()), this, SLOT(load_aircraft()) );

    treeToolbar->addSeparator();

    //******************************************************
    //** Filter Buttons
    QButtonGroup *buttViewGroup = new QButtonGroup(this);
    buttViewGroup->setExclusive(true);
    connect(buttViewGroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(on_view_button_clicked(QAbstractButton*))
    );

    QToolButton *buttAll = new QToolButton();
    treeToolbar->addWidget(buttAll);
    buttViewGroup->addButton(buttAll);
    buttAll->setText("All");
    buttAll->setCheckable(true);
	buttAll->setIcon(QIcon(":/icon/pink"));
    buttAll->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);


    QToolButton *buttAvailable = new QToolButton();
    treeToolbar->addWidget(buttAvailable);
    buttViewGroup->addButton(buttAvailable);
	buttAvailable->setText("Favourites");
    buttAvailable->setCheckable(true);
	buttAvailable->setIcon(QIcon(":/icon/purple"));
    buttAvailable->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    buttAll->setChecked(true);


	//===============================================================
    //** Models
	/*
    model = new QStandardItemModel(this);
    model->setColumnCount(2);
    QStringList headerLabelsList;
	headerLabelsList << "Model" << "Description" << "Path";
    model->setHorizontalHeaderLabels(headerLabelsList);

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
	*/
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

	connect( treeWidget,
			 SIGNAL( itemSelectionChanged() ),
			 SLOT( on_tree_selection_changed() )
	);


    statusBarTree = new QStatusBar();
    treeLayout->addWidget(statusBarTree);
    statusBarTree->showMessage("Idle");



    //*************************************************************************************************
    //** Right
    //*************************************************************************************************


    QGroupBox *grpAero = new QGroupBox();
    splitter->addWidget(grpAero);
   // QString style = QString("QGroupBox{border:2px solid gray;border-radius:5px;  margin-top: 1ex;} QGroupBox::title{subcontrol-origin: margin;subcontrol-position:top center;padding:0 3px;}");
	//grpAero->setStyleSheet(style);
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
	grpAeroPanel->setStyleSheet("background-color: white;");
	aeroLayout->addWidget(grpAeroPanel);


	QVBoxLayout *aeroPanelLayout = new QVBoxLayout();
	grpAeroPanel->setLayout(aeroPanelLayout);

	lblAircraftModel = new QLabel("");
	lblAircraftModel->setAlignment(Qt::AlignHCenter);
	aeroPanelLayout->addWidget(lblAircraftModel);


	lblAuthor = new QLabel("");
	aeroPanelLayout->addWidget(lblAuthor);
	lblAuthor->setAlignment(Qt::AlignHCenter);
	lblAuthor->setFixedWidth(200);

	lblFdm = new QLabel("");
	aeroPanelLayout->addWidget(lblFdm);
	lblFdm->setAlignment(Qt::AlignHCenter);

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
    qDebug("on_view_button_clicked()");
    //qDebug() << button->text();
    // TODO
    statusBarTree->showMessage(button->text());
    //QString
}



//==========================================================================
// Aircraft Selected
//==========================================================================
void AircraftWidget::on_tree_selection_changed(){

	QTreeWidgetItem *item = treeWidget->currentItem();

	//** Check there is item and its a model
	if(!item | item->text(C_AERO).length() == 0){
		aeroImageLabel->clear();
		lblAircraftModel->setText("");
		lblAuthor->setText("");
		lblFdm->setText("");
		emit set_arg("remove", "--aircraft=", "");
		return;
	}

	emit set_arg("set", "--aircraft=", item->text(C_AERO));

	lblAircraftModel->setText( item->text(C_AERO));
	lblAuthor->setText( item->text(C_AUTHOR));
	lblFdm->setText( item->text(C_FDM));

	QString thumb_file( settings.aircraft_path( item->parent()->text(C_DIR) ) );
	thumb_file.append("/thumbnail.jpg");
	QPixmap aeroImage(thumb_file);
	if(aeroImage.isNull()){
		qDebug("NULL");
	}
	aeroImageLabel->setPixmap(aeroImage);

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


//*** Borrowed from fgX gitourious - ta Gral ;-)
QStringList AircraftWidget::aircraft_xml_set(){


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

				} // <Add models

			}
		}
	}
	return aeroList;

}

//=============================================================
// Save Settings
void AircraftWidget::save_settings(){




}


//=============================================================
// Load Settings
void AircraftWidget::load_settings(){



}



//=============================================================
// Validate
bool AircraftWidget::validate(){
	return true;
}


//=============================================================
// Initialize
void AircraftWidget::initialize(){
	QStringList aeroList = settings.value("aircraft_list").toStringList();
	QString record;
	QString last_dir("");
	QTreeWidgetItem *dirItem;

	 for (int i = 0; i < aeroList.size(); ++i) {
		record = aeroList.at(i);
		QStringList fields = record.split("~|~");

		if(last_dir != fields.at(C_DIR)){
			dirItem = new QTreeWidgetItem();
			dirItem->setText( C_DIR, fields.at(0));
			dirItem->setIcon(C_DIR, QIcon(":/icon/folder"));
			treeWidget->addTopLevelItem(dirItem);
			treeWidget->setFirstItemColumnSpanned(dirItem, true);
			last_dir = fields.at(C_DIR);
		}
		//directory, xml_file, aero, fdm, description, author
		QTreeWidgetItem *aeroItem = new QTreeWidgetItem(dirItem);
		aeroItem->setText(C_XML, fields.at(1));
		aeroItem->setText(C_AERO, fields.at(2));
		aeroItem->setIcon(C_AERO, QIcon(":/icon/aircraft"));
		aeroItem->setText(C_FDM, fields.at(3));
		aeroItem->setText(C_DESCRIPTION, fields.at(4));
		aeroItem->setText(C_AUTHOR, fields.at(5));
	 }
	 treeWidget->sortByColumn(C_DIR, Qt::AscendingOrder);
}

//=============================================================
// Initialize
void AircraftWidget::load_aircraft(){
	settings.setValue("aircraft_list", aircraft_xml_set());
}
