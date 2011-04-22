
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
//#include <QtCore/QDir>
#include <QtCore/QFile>

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
#include "aircraft/aerotools.h"




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

	QString thumb_file = QString("%1/%2/%3/thumbnail.jpg").arg( mainObject->settings->aircraft_path(),
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
















//========================================================
//** Load Aircraft Shell
void AircraftWidget::load_aircraft_shell(){

	QString command = mainObject->settings->fgfs_path();
	command.append(" --fg-root=").append(mainObject->settings->fg_root());
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


//=============================================================
// Save Settings
void AircraftWidget::save_settings(){
	QTreeWidgetItem *item = treeWidget->currentItem();
	if(item && item->text(C_AERO).length() == 0){
		mainObject->settings->setValue("aircraft", item->text(C_AERO) );
	}
	mainObject->settings->setValue("nav1", txtNav1->text());
	mainObject->settings->setValue("nav2", txtNav2->text());
	mainObject->settings->setValue("adf", txtAdf->text());
	mainObject->settings->setValue("comm1", txtComm1->text());
	mainObject->settings->setValue("comm2", txtComm2->text());
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
}

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
QString AircraftWidget::aircraft(){
	QTreeWidgetItem *item = treeWidget->currentItem();
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
	qDebug() << "on refresh_cache";
	//* scan Airraft dirs and save in settings (mad encoding for now)
	AeroTools *aeroTool = new AeroTools(this);
	aeroList = aeroTool->scan_xml_sets();
	mainObject->settings->setValue("AIRCRAFT_CACHED", aeroList );
	mainObject->settings->sync();
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
	aeroList = mainObject->settings->value("AIRCRAFT_CACHED").toStringList();
	if(aeroList.count() == 0){
		//assume its never been initialised
		//scan_xml_sets();
	}
	aeroList = mainObject->settings->value("AIRCRAFT_CACHED").toStringList();
	load_tree();
}

//=============================================================
// Get Args
QStringList AircraftWidget::get_args(){

	QStringList args;

	if(aircraft().length() > 0){
		args << QString("--aircraft=%1").arg(aircraft());
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
