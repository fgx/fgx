// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // aircraftwidget.cpp
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information


#include "app_config.h"

#include <QProcess>
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QFile>

#include <QXmlQuery>
#include <QDomDocument>

// @TODO - qt5 dont use this
#include <QDesktopServices>
#include <QFileDialog>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QProgressDialog>

#include <QToolBar>
#include <QToolButton>
#include <QAbstractButton>
#include <QButtonGroup>

#include <QAction>
#include <QLabel>
#include <QCheckBox>
#include <QTabWidget>

#include <QHeaderView>
#include <QAbstractItemView>
#include <QPixmap>
#include <QDoubleValidator>


#include "aircraft/aircraftwidget.h"
#include "aircraft/aircraftmodel.h"


#include "utilities/utilities.h"
#include "utilities/messagebox.h"

#include "fgtools/fileviewerwidget.h"


AircraftWidget::AircraftWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{


    mainObject = mOb;

    this->model = new AircraftModel(this->mainObject);

    //========================================================
    //= Model
    proxyModel = new AircraftProxyModel();
    proxyModel->setSourceModel( this->model );
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(AircraftModel::C_FILTER);
    proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);

    //= Custom Dir button stuff
    buttGroupShowDirs = new QButtonGroup(this);
    buttGroupShowDirs->setExclusive(false);
    connect(buttGroupShowDirs, SIGNAL(buttonClicked(QAbstractButton*)),
            this,              SLOT(on_toggle_directory())
    );

    actGroupDeleteCustomDirs = new QActionGroup(this);
    actGroupDeleteCustomDirs->setExclusive(false);
    connect(actGroupDeleteCustomDirs, SIGNAL(triggered(QAction*)),
            this,                     SLOT(on_remove_custom_dir(QAction*))
    );



    //=======================================================
    //* Main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);

    splitter = new QSplitter(this);
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

    //=======================================================
    //= Top Toolbar
    QHBoxLayout *topBar = new QHBoxLayout();
    topBar->setSpacing(5);
    treeLayout->addLayout(topBar);

    //---------------------------------------
    //= Filter Toolbar
    ToolBarGroup *grpFilter = new ToolBarGroup();
    topBar->addWidget(grpFilter);
    grpFilter->setTitle("Filter");

    //= Clear filter button
    QToolButton * buttClearFilter = new QToolButton();
    buttClearFilter->setText("CKR >");
    buttClearFilter->setFixedWidth(20);
    buttClearFilter->setIcon(QIcon(":/icon/clear_filter"));
    buttClearFilter->setAutoRaise(true);
    buttClearFilter->setToolButtonStyle(Qt::ToolButtonIconOnly);
    grpFilter->addWidget(buttClearFilter);
    connect(buttClearFilter, SIGNAL(clicked()), this, SLOT(on_clear_filter()) );

    //= Filter Text
    txtFilter = new QLineEdit();
    txtFilter->setFixedWidth(100);
    grpFilter->addWidget(txtFilter);
    connect(txtFilter, SIGNAL(textChanged(const QString)), this, SLOT(on_filter_text_changed(const QString)));


    //---------------------------------------
    //= Custom Directories
    grpCustomDirs = new ToolBarGroup();
    topBar->addWidget(grpCustomDirs);
    grpCustomDirs->setTitle("Directories");

    //= Show Base button
    buttShowBase = new QToolButton();
    //buttShowBase->setProperty("dir", mainObject->X->aircraft_path() ); ?? WE DO THIS AT LOAD_AIRCRAFT IN CASE CHANGED
    buttShowBase->setProperty("base", "1" );
    buttShowBase->setToolTip( mainObject->X->aircraft_path());
    buttShowBase->setText( "Base Package" );
    buttShowBase->setIcon(QIcon(":/icon/base_folder"));
    buttShowBase->setCheckable(true);
    buttShowBase->setChecked(true);
    buttShowBase->setAutoRaise(true);
    buttShowBase->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    grpCustomDirs->addWidget(buttShowBase);
    buttGroupShowDirs->addButton(buttShowBase);

    //= Show Base button
    QToolButton * buttAddCustomDir = new QToolButton();
    buttAddCustomDir->setText("Add");
    buttAddCustomDir->setIcon(QIcon(":/icon/folder_add"));
    buttAddCustomDir->setAutoRaise(true);
    buttAddCustomDir->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    grpCustomDirs->addWidget(buttAddCustomDir);
    connect(buttAddCustomDir, SIGNAL(clicked()), this, SLOT(on_add_custom_dir()));

    topBar->addStretch(20);

    //---------------------------------------
    //= Refresh Action
    ToolBarGroup *grpActions = new ToolBarGroup();
    topBar->addWidget(grpActions);
    grpActions->setTitle("Actions");

    //== Reload cache
    QToolButton *actionReloadCacheDb = new QToolButton(this);
    actionReloadCacheDb->setText("Reload");
    actionReloadCacheDb->setIcon(QIcon(":/icon/load"));
    actionReloadCacheDb->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    actionReloadCacheDb->setAutoRaise(true);
    grpActions->addWidget(actionReloadCacheDb);
    connect(actionReloadCacheDb, SIGNAL(clicked()), this, SLOT(on_reload_cache()) );

    QMenu *menuReload = new QMenu();
    actionReloadCacheDb->setMenu(menuReload);
    actionReloadCacheDb->setPopupMode(QToolButton::MenuButtonPopup);

    QAction *actView = menuReload->addAction("View `aircraft_cache.txt` cache file" );
    connect(actView, SIGNAL(triggered()),
            this, SLOT(on_view_aircraft_cache())
    );


    //=======================================================
    //= Treeview
    treeView = new QTreeView(this);
    treeLayout->addWidget(treeView);
    //treeView->setModel(proxyModel);
    treeView->setModel(this->model);

    treeView->setRootIsDecorated(false);
    treeView->setUniformRowHeights(true);
    treeView->setSortingEnabled(true);
    treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    treeView->setSelectionBehavior(QAbstractItemView::SelectRows);

    treeView->header()->setStretchLastSection(true);
    treeView->setColumnWidth(AircraftModel::C_DIR, 60);
    treeView->setColumnWidth(AircraftModel::C_FDM, 60);
    treeView->setColumnWidth(AircraftModel::C_DESCRIPTION, 200);
    this->on_debug_mode(); // debug mode shows/hides some columns
    connect( treeView->selectionModel(),
             SIGNAL( selectionChanged(const QItemSelection &, const QItemSelection & ) ),
             SLOT( on_tree_selection_changed() )
    );


    //=================================
    //= Status Bar
    statusBar = new QStatusBar();
    statusBar->setSizeGripEnabled(false);
    treeLayout->addWidget(statusBar);



    //== View nested Checkbox
    /*checkViewNested = new QCheckBox();
    checkViewNested->setText("View folders");
    statusBarTree->addPermanentWidget(checkViewNested);
    connect(checkViewNested, SIGNAL(clicked()), this, SLOT(load_tree()));*/




    //================================================================================================
    //= Right
    //================================================================================================

    QGroupBox *grpAero = new QGroupBox();
    splitter->addWidget(grpAero);
    grpAero->setDisabled(false);
    grpAero->setMinimumWidth(300);
    //grpAero->setTitle(tr("Preview, Radio, Fuel"));


    QVBoxLayout *aeroLayout = new QVBoxLayout();
    grpAero->setLayout(aeroLayout);

    QHBoxLayout *layAeroTopBar = new QHBoxLayout();
    aeroLayout->addLayout(layAeroTopBar);

    lblAero = new QLabel();
    lblAero->setStyleSheet("font-size: 20pt; font-weight: bold;");
    layAeroTopBar->addWidget(lblAero, 10);

    buttOpenAeroDir = new QToolButton();
    buttOpenAeroDir->setToolTip("Browse model files");
    buttOpenAeroDir->setAutoRaise(true);
    buttOpenAeroDir->setIcon(QIcon(":/icon/folder_open"));
    buttOpenAeroDir->setToolButtonStyle(Qt::ToolButtonIconOnly);
    buttOpenAeroDir->setDisabled(true);
    layAeroTopBar->addWidget(buttOpenAeroDir, 0);
    connect(buttOpenAeroDir, SIGNAL(clicked()),
            this, SLOT(on_open_aircraft_path())
    );

    lblAeroDescription = new QLabel();
    aeroLayout->addWidget(lblAeroDescription);

    lblAeroXml = new QLabel();
    aeroLayout->addWidget(lblAeroXml);

    //**********************************************8
    //** Aero Panel
    QGroupBox *grpAeroPanel = new QGroupBox();
    grpAeroPanel->setStyleSheet("background-image: url(:/artwork/background-gray-tabbar); border-top: 0px;");
    aeroLayout->addWidget(grpAeroPanel);


    QVBoxLayout *aeroPanelLayout = new QVBoxLayout();
    grpAeroPanel->setLayout(aeroPanelLayout);

    QHBoxLayout *imgBox = new QHBoxLayout();
    aeroPanelLayout->addLayout(imgBox);

    aeroImageLabel =  new QLabel(this);
    aeroImageLabel->setFixedWidth(171);
    aeroImageLabel->setFixedHeight(128);
    imgBox->addWidget(aeroImageLabel);


    //******************************************************8
   //** Tab Widgets
    QTabWidget *aeroTabs = new QTabWidget();
    aeroLayout->addWidget(aeroTabs, 120);

    QWidget *aeroControlWidget = new QWidget();
    QWidget *fuelWidget = new QWidget();
    aeroTabs->addTab(aeroControlWidget, tr("Radio"));
    aeroTabs->addTab(fuelWidget, tr("Fuel"));

    //** Fuel Widget
    QGridLayout *layoutFuelPane = new QGridLayout();
    fuelWidget->setLayout(layoutFuelPane);
    int rowFuel = 1;

    // Enable fuel freeze
    rowFuel++;
    checkBoxFuelFreeze = new QCheckBox(this);
    checkBoxFuelFreeze->setText("Enable fuel freeze");
    layoutFuelPane->addWidget(checkBoxFuelFreeze, 0, 1, 1, Qt::AlignLeft);
    connect(checkBoxFuelFreeze, SIGNAL(clicked()), this, SLOT(on_enable_fuel_freeze_clicked()));

    //* Tank 1
    rowFuel++;
    layoutFuelPane->addWidget(new QLabel(tr("Tank 1")), rowFuel, 0, 1, 1, Qt::AlignRight);
    txtTank1 = new QLineEdit();
    txtTank1->setValidator(new QDoubleValidator(0, 200, 2, this));
    layoutFuelPane->addWidget(txtTank1,rowFuel, 1, 1, 1);
    connect(txtTank1, SIGNAL(textChanged(QString)), this, SLOT(on_fuel_changed()));

    //* Tank 2
    rowFuel++;
    layoutFuelPane->addWidget(new QLabel(tr("Tank 2")), rowFuel, 0, 1, 1, Qt::AlignRight);
    txtTank2 = new QLineEdit();
    txtTank2->setValidator(new QDoubleValidator(0, 200, 2, this));
    layoutFuelPane->addWidget(txtTank2,rowFuel, 1, 1, 1);
    connect(txtTank2, SIGNAL(textChanged(QString)), this, SLOT(on_fuel_changed()));

    //* Tank 3
    rowFuel++;
    layoutFuelPane->addWidget(new QLabel(tr("Tank 3")), rowFuel, 0, 1, 1, Qt::AlignRight);
    txtTank3 = new QLineEdit();
    txtTank3->setValidator(new QDoubleValidator(0, 200, 2, this));
    layoutFuelPane->addWidget(txtTank3, rowFuel, 1, 1, 1);
    connect(txtTank3, SIGNAL(textChanged(QString)), this, SLOT(on_fuel_changed()));

    layoutFuelPane->setRowStretch(rowFuel + 1, 20); // stretch end


    //** Aero Control Widget
    QGridLayout *layoutAeroPane = new QGridLayout();
    aeroControlWidget->setLayout(layoutAeroPane);
    int row = 1;

    //* Navs
    layoutAeroPane->addWidget(new QLabel(tr("NAV1")), row, 0, 1, 1, Qt::AlignRight);
    txtNav1 = new QLineEdit();
    txtNav1->setValidator(new QDoubleValidator(0, 200, 2, this));
    layoutAeroPane->addWidget(txtNav1,row, 1, 1, 1);
    connect(txtNav1, SIGNAL(textChanged(QString)), this, SLOT(on_navs_changed()));

    row++;
    layoutAeroPane->addWidget(new QLabel(tr("NAV2")), row, 0, 1, 1, Qt::AlignRight);
    txtNav2 = new QLineEdit();
    txtNav2->setValidator(new QDoubleValidator(0, 200, 2, this));
    layoutAeroPane->addWidget(txtNav2,row, 1, 1, 1);
    connect(txtNav2, SIGNAL(textChanged(QString)), this, SLOT(on_navs_changed()));

    //= ADF
    row++;
    layoutAeroPane->addWidget(new QLabel(tr("ADF1")), row, 0, 1, 1, Qt::AlignRight);
    txtAdf1 = new QLineEdit();
    txtAdf1->setValidator(new QDoubleValidator(0, 200,0, this));
    layoutAeroPane->addWidget(txtAdf1, row, 1, 1, 1);
    connect(txtAdf1, SIGNAL(textChanged(QString)), this, SLOT(on_navs_changed()));

    row++;
    layoutAeroPane->addWidget(new QLabel(tr("ADF2")), row, 0, 1, 1, Qt::AlignRight);
    txtAdf2 = new QLineEdit();
    txtAdf2->setValidator(new QDoubleValidator(0, 200,0, this));
    layoutAeroPane->addWidget(txtAdf2, row, 1, 1, 1);
    connect(txtAdf2, SIGNAL(textChanged(QString)), this, SLOT(on_navs_changed()));

    //= Comms
    row++;
    layoutAeroPane->addWidget(new QLabel(tr("COM1")), row, 0, 1, 1, Qt::AlignRight);
    txtCom1 = new QLineEdit();
    txtCom1->setValidator(new QDoubleValidator(0, 200, 2, this));
    layoutAeroPane->addWidget(txtCom1,row, 1, 1, 1);
    connect(txtCom1, SIGNAL(textChanged(QString)), this, SLOT(on_navs_changed()));

    row++;
    layoutAeroPane->addWidget(new QLabel(tr("COM2")), row, 0, 1, 1, Qt::AlignRight);
    txtCom2 = new QLineEdit();
    txtCom2->setValidator(new QDoubleValidator(0, 200, 2, this));
    layoutAeroPane->addWidget(txtCom2,row, 1, 1, 1);
    connect(txtCom2, SIGNAL(textChanged(QString)), this, SLOT(on_navs_changed()));


    layoutAeroPane->setRowStretch(row + 1, 20); // stretch end

    //==================
    //== Setup

    splitter->setCollapsible(0, false);
    splitter->setCollapsible(1, false);
    splitter->setStretchFactor(0, 50);
    splitter->setStretchFactor(1, 10);
    splitter->setProperty("settings_namespace", "aircraft_widget_splitter");
    this->mainObject->settings->restoreSplitter(splitter);
    connect(splitter, SIGNAL(splitterMoved(int,int)), this, SLOT(on_splitter_moved()));

    load_custom_dir_buttons();

    //== Main Settings connection
    connect(this, SIGNAL(setx(QString,bool,QString)), mainObject->X, SLOT(set_option(QString,bool,QString)) );
    connect(mainObject->X, SIGNAL(upx(QString,bool,QString)), this, SLOT(on_upx(QString,bool,QString)));
    connect(mainObject, SIGNAL(on_debug_mode(bool)), this, SLOT(on_debug_mode()));

    this->on_tree_selection_changed();

}
void AircraftWidget::on_splitter_moved(){
    this->mainObject->settings->saveSplitter(splitter);
}

void AircraftWidget::on_clear_filter(){
    txtFilter->setText("");
    txtFilter->setFocus();
}

void AircraftWidget::on_filter_text_changed(const QString s){
    proxyModel->setFilterFixedString(s);
}

//==========================================================================1
// Aircraft Selected
//==========================================================================
void AircraftWidget::on_tree_selection_changed(){

    if(treeView->selectionModel()->hasSelection() == false){
        outLog("on_tree_selection_changed: no selected item");
        lblAero->setText("-");
        lblAeroDescription->setText("-");
        lblAeroXml->setText("");
        this->buttOpenAeroDir->setDisabled(true);
        this->buttOpenAeroDir->setIcon(QIcon(":/icon/folder_open"));
        emit setx("--aircraft=", false, "");
        return;
    }
    /*
    lblAero->setText( this->mainObject->aircraftModel->itemFromIndex(
                            proxyModel->mapToSource(treeView->selectionModel()->selectedIndexes().at(AircraftModel::C_AERO))
                          )->text());
    lblAeroDescription->setText( this->mainObject->aircraftModel->itemFromIndex(
                            proxyModel->mapToSource(treeView->selectionModel()->selectedIndexes().at(AircraftModel::C_DESCRIPTION))
                          )->text());


    QString xmlFile =  this->mainObject->aircraftModel->itemFromIndex(
                        proxyModel->mapToSource(treeView->selectionModel()->selectedIndexes().at(AircraftModel::C_FILE_PATH))
                    )->text();
    lblAeroXml->setText(xmlFile);

    int is_base =  this->mainObject->aircraftModel->itemFromIndex(
                        proxyModel->mapToSource(treeView->selectionModel()->selectedIndexes().at(AircraftModel::C_BASE))
                    )->text().toInt();
    this->buttOpenAeroDir->setDisabled(false);
    if( is_base == 1 ){
        this->buttOpenAeroDir->setIcon(QIcon(":/icon/base_folder"));
    }else{
        this->buttOpenAeroDir->setIcon(QIcon(":/icon/custom_folder"));
    }

    QModelIndex midx = treeView->selectionModel()->selectedIndexes().at(AircraftModel::C_DIR);
    QStandardItem *item = this->mainObject->aircraftModel->itemFromIndex( proxyModel->mapToSource(midx)  );

    //= Get the thumbnail image
    QString thumb_file = QString("%1/%2/thumbnail.jpg").arg(mainObject->X->aircraft_path(), item->text());
    if(QFile::exists(thumb_file)){
        QPixmap aeroImage(thumb_file);
        if(!aeroImage.isNull()){
            //outLog("on_tree_selection_changed: Loaded thumb "+thumb_file);
            aeroImageLabel->setText("");
            aeroImageLabel->setPixmap(aeroImage);

        } else{
            //outLog("on_tree_selection_changed: NO thumb load "+thumb_file);
            aeroImageLabel->clear();
            aeroImageLabel->setText("No Image");
        }
    }else{
        aeroImageLabel->setText("No Image");
    }

    emit setx("--aircraft=", true, selected_aircraft());
    mainObject->launcherWindow->on_upx("--aircraft=", true, selected_aircraft()); // Show aircraft in header
    */
}


//==============================
//== Select an aircraft in tree
void AircraftWidget::select_node(QString aero){
    if(aero.length() == 0){
        return;
    }
    /*
    QList<QTreeWidgetItem*> items = treeWidget->findItems(aero, Qt::MatchExactly | Qt::MatchRecursive, C_AERO);
    if(items.length() > 0){
        treeWidget->setCurrentItem(items[0]);
        treeWidget->scrollToItem(items[0], QAbstractItemView::PositionAtCenter);

    }*/
}

//==============================
//== return selected Aircraft
QString AircraftWidget::selected_aircraft(){

    QModelIndex midx = treeView->selectionModel()->selectedIndexes().at(AircraftModel::C_AERO);
    //QStandardItem *item = this->mainObject->aircraftModel->itemFromIndex( proxyModel->mapToSource(midx)   );
    //return item->text();
    return QString("TODO");
}

//=============================================================
// Validate
QString AircraftWidget::validate(){
    //if(groupUseAircraft->checkedId() == 1 &&  !treeWidget->currentItem()){
    //	return QString("Aircraft: No aircraft selected or check [x] Use default");
    //	outLog("FGx reports: AircraftWidget::validate() No aircraft selected (maybe no list), and [x] use default not selected. ***");
    //}
    return QString("");
}


//=============================================================
// Rescan aircraft cache
void AircraftWidget::on_reload_cache(){
    this->load_aircraft(true);
}



//=============================================================
/* @brief Load/reload the model */
void AircraftWidget::load_aircraft(bool reload_cache){

    buttShowBase->setToolTip( mainObject->X->aircraft_path());
    buttShowBase->setProperty("dir", mainObject->X->aircraft_path());
    statusBar->showMessage(reload_cache ? "Reloading Cache" : "Loading...");


    //treeView->setUpdatesEnabled(false);
    this->model->load(reload_cache);
    //treeView->setUpdatesEnabled(true);
    return;
    treeView->sortByColumn(AircraftModel::C_AERO, Qt::AscendingOrder);

    treeView->resizeColumnToContents(AircraftModel::C_DIR);
    treeView->resizeColumnToContents(AircraftModel::C_AERO);
    treeView->resizeColumnToContents(AircraftModel::C_FDM);
    //treeView->resizeColumnToContents(C_DIR);

    select_node(mainObject->X->getx("--aircraft="));
    QString str = QString("%1 aircraft").arg(this->model->rowCount());
    statusBar->showMessage(str);
    outLog("*** FGx: AircraftWidget::load_tree: with " + str);
}



//=============================================================
// Initialize
void AircraftWidget::initialize(){

    static bool first_load_done = false;
    if(first_load_done){
        return;
    }
    //if (!QFile::exists(mainObject->data_file("aircraft.txt"))){
    //    statusBar->showMessage("*** No cached data. Use set paths and reload");
    //}else{
    this->model->load(false);
    //}
    first_load_done = true;
}




void AircraftWidget::on_navs_changed()
{
    emit setx("--nav1=", true, txtNav1->text());
    emit setx("--nav2=", true, txtNav2->text());
    emit setx("--adf1=", true, txtAdf1->text());
    emit setx("--adf2=", true, txtAdf2->text());
    emit setx("--com1=", true, txtCom1->text());
    emit setx("--com2=", true, txtCom2->text());

}

void AircraftWidget::on_enable_fuel_freeze_clicked(){
    emit setx("--enable-fuel-freeze", checkBoxFuelFreeze->isChecked(), "");
}

void AircraftWidget::on_fuel_changed()
{
    emit setx("--prop:/consumables/fuels/tank[1]/level-gal=", true, txtTank1->text());
    emit setx("--prop:/consumables/fuels/tank[2]/level-gal=", true, txtTank2->text());
    emit setx("--prop:/consumables/fuels/tank[3]/level-gal=", true, txtTank3->text());

}



//=====================================================
void AircraftWidget::on_upx( QString option, bool enabled, QString value)
{
    Q_UNUSED(enabled);
    //= NOTE: The --aircraft=. --runway, etc is detected as the tree loads from cache

    if(option == "--aircraft"){
        //= see tree load
        select_node(value);

    }else if(option == "--fg-aircraft="){
        //txtAircraftPath->setText(value);

    //== tab radio
    }else if(option == "--nav1="){
        txtNav1->setText(value);

    }else if(option == "--nav2="){
        txtNav2->setText(value);


    }else if(option == "--adf1="){
        txtAdf1->setText(value);

    }else if(option == "--adf2="){
        txtAdf2->setText(value);


    }else if(option == "--com1="){
        txtCom1->setText(value);

    }else if(option == "--com2="){
        txtCom2->setText(value);


    //=== tab fuel

    }else if(option == "use_default_fuel"){
        checkBoxUseDefaultFuel->setChecked(enabled);

    }else if(option == "--enable-fuel-freeze"){
        checkBoxFuelFreeze->setChecked(enabled);

    }else if(option == "--prop:/consumables/fuels/tank[1]/level-gal="){
        txtTank1->setText(value);

    }else if(option == "--prop:/consumables/fuels/tank[2]/level-gal="){
        txtTank2->setText(value);

    }else if(option == "--prop:/consumables/fuels/tank[3]/level-gal="){
        txtTank3->setText(value);
    }
}

//=====================================================================
// Custom aircraft Dir Stuff
//=====================================================================
// switches a directories visiibility
void AircraftWidget::on_toggle_directory(){


    for(int i = 0; i < buttGroupShowDirs->buttons().length(); i++){
        QString dir = buttGroupShowDirs->buttons().at(i)->property("dir").toString();
       // qDebug() << "d=" << dir;
        this->proxyModel->show_dir(dir, buttGroupShowDirs->buttons().at(i)->isChecked());
    }
}



// HELP. .this function crashed for some reason..
void AircraftWidget::on_remove_custom_dir(QAction *act)
{
    qDebug() << "--------on_remove_custom_dir()";

    QString dir = act->property("dir").toString();

    //return;
    QStringList custom_dirs = this->mainObject->settings->value("custom_aircraft_dirs").toStringList();
    int idx = custom_dirs.indexOf(dir);
    qDebug() << dir << "==" << idx;
    if( idx == -1) {
        qDebug() << "arrgggggghhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh";
        return;
    }
    custom_dirs.removeAt(idx);
    qDebug() << "removed dir " << idx << custom_dirs;
    this->mainObject->settings->setValue("custom_aircraft_dirs", custom_dirs);
    this->mainObject->settings->sync();
    qDebug() << "saved " << custom_dirs;
    this->load_custom_dir_buttons();
    qDebug() << " ALL DONE ";
}

void AircraftWidget::on_add_custom_dir()
{

    QString sel_dir = QFileDialog::getExistingDirectory(this, tr("Select an Aircraft directory"),
                                                         "", QFileDialog::ShowDirsOnly);
    if(sel_dir.length() == 0){
       return;
    }
    QString sto_dir = QDir::fromNativeSeparators(sel_dir);
    QStringList custom_dirs = this->mainObject->settings->value("custom_aircraft_dirs").toStringList();
    if(custom_dirs.indexOf(sto_dir) == -1){
        custom_dirs.append(sto_dir);
    }
    this->mainObject->settings->setValue("custom_aircraft_dirs", custom_dirs);
    this->mainObject->settings->sync();
    this->load_custom_dir_buttons();
}

void AircraftWidget::load_custom_dir_buttons()
{
    //qDebug() << "---load_custom_dir_buttons" << actGroupDeleteCustomDirs->actions().length() << "=" << buttGroupShowDirs->buttons().length() << "=" << lstCustomDirButtons.length();

    // Delete all the deletes from actinGroup
    QList<QAction*> actions = actGroupDeleteCustomDirs->actions();
    for(int i = 0; i < actions.length(); i++){
        actGroupDeleteCustomDirs->removeAction(actions.at(i));
    }

    // Loop buttons and remove from containers then delete
    while(!lstCustomDirButtons.isEmpty() ){
        qDebug() << "nuked button" ;
        grpCustomDirs->removeWidget(lstCustomDirButtons.at(0));
        buttGroupShowDirs->removeButton(lstCustomDirButtons.at(0));
        delete lstCustomDirButtons.takeFirst();
    }
    //qDebug() << " after nuke " << actGroupDeleteCustomDirs->actions().length() << "=" << buttGroupShowDirs->buttons().length() << "=" << lstCustomDirButtons.length();

   QStringList custom_dirs = this->mainObject->settings->value("custom_aircraft_dirs").toStringList();
   for(int i=0; i < custom_dirs.size(); i++){

       QFileInfo dinfo(custom_dirs.at(i));

       QToolButton * buttDir = new QToolButton();
       grpCustomDirs->bottomLayout->insertWidget(grpCustomDirs->bottomLayout->count() - 1, buttDir);
       buttGroupShowDirs->addButton(buttDir);
       lstCustomDirButtons.append(buttDir);
       //qDebug() << "Add button" << dinfo.absoluteFilePath();
       buttDir->setProperty("dir", QDir::toNativeSeparators(dinfo.absoluteFilePath()));
       buttDir->setToolTip( QDir::toNativeSeparators( custom_dirs.at(i) ) );
       buttDir->setText(dinfo.baseName());
       buttDir->setIcon(QIcon(":/icon/custom_folder"));
       buttDir->setCheckable(true);
       buttDir->setChecked(true);
       buttDir->setAutoRaise(true);
       buttDir->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

       // qDebug() << "pre menu =" << dinfo.absoluteFilePath();
       QMenu *meniw = new QMenu();
       buttDir->setMenu(meniw);
       buttDir->setPopupMode(QToolButton::MenuButtonPopup); //grhhhh.. vs Qt::ToolButtonTextBesideIcon

       /// qDebug() << "pre action =" << dinfo.absoluteFilePath();
       // remove action
       QAction *act = meniw->addAction("Remove");
       act->setIcon(QIcon(":/icon/bin"));
       act->setProperty("dir", dinfo.absoluteFilePath());
       meniw->addAction(act);
      //  qDebug() << "post action =" << dinfo.absoluteFilePath();
       actGroupDeleteCustomDirs->addAction(act);
       qDebug() << "  psot group =" << dinfo.absoluteFilePath();


   }
   qDebug() << " Done loading";
   this->on_toggle_directory();
}

void AircraftWidget::on_open_aircraft_path()
{
    if(lblAeroXml->text().length() == 0){
        return;
    }
    QFileInfo fi(lblAeroXml->text());
    QUrl url(QString("file://%1").arg(fi.dir().absolutePath()));
    QDesktopServices::openUrl(url);
}
void AircraftWidget::on_view_aircraft_cache()
{
    FileViewerWidget *fileViewer = new FileViewerWidget();

    fileViewer->setWindowState(Qt::WindowMaximized);
    fileViewer->show();
    fileViewer->setFile(this->model->cacheFileName());

}

void AircraftWidget::on_debug_mode(){
    bool hidden = !this->mainObject->debug_mode;
    treeView->setColumnHidden(AircraftModel::C_XML_FILE, hidden);
    treeView->setColumnHidden(AircraftModel::C_FILE_PATH, hidden);
    treeView->setColumnHidden(AircraftModel::C_FILTER_DIR, hidden);
    treeView->setColumnHidden(AircraftModel::C_FILTER, hidden);
}
