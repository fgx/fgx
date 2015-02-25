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
#include "aircraft/aircraftdata.h"
#include "xwidgets/toolbargroup.h"
#include "utilities/utilities.h"
#include "utilities/messagebox.h"


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
    treeTopBar->setContentsMargins(5,5,5,5);
    treeTopBar->setSpacing(5);
    treeLayout->addLayout(treeTopBar);

    //= Use Custom Hangar (Aircraft Directory)
    checkBoxUseCustomHangar = new QCheckBox("Use Custom Hangar (Custom aircraft directory):");
    treeTopBar->addWidget(checkBoxUseCustomHangar);
    connect(checkBoxUseCustomHangar, SIGNAL(clicked()), this, SLOT(on_custom_hangar_path()) );

    txtAircraftPath = new QLineEdit();
    treeTopBar->addWidget(txtAircraftPath);

    buttSelectPath = new QToolButton();
    buttSelectPath->setText("Select");
    treeTopBar->addWidget(buttSelectPath);
    connect(buttSelectPath, SIGNAL(clicked()), this, SLOT(on_select_path()));

    //=======================================================
    // Filter toolbar
    QToolBar *topBar = new QToolBar();
    treeLayout->addWidget(topBar);

    ToolBarGroup *grpFilter = new ToolBarGroup();
    topBar->addWidget(grpFilter);
    grpFilter->setTitle("Filter");

    QToolButton * buttClearFilter = new QToolButton();
    buttClearFilter->setText("CKR >");
    connect(buttClearFilter, SIGNAL(clicked()), this, SLOT(on_clear_filter()) );
    grpFilter->addWidget(buttClearFilter);

    txtFilter = new QLineEdit();
    grpFilter->addWidget(txtFilter);
    connect(txtFilter, SIGNAL(textChanged(const QString)), this, SLOT(on_filter_text_changed(const QString)));

    //== TODO NEW treeview _ + model
    model = new QStandardItemModel(this);
    QStringList hLabels;
    //directory << aero << description << fdm << author << xml_file << file_path;
    hLabels << "Dir" << "Aero" << "Description" << "FDM" << "Authors" << "XML" << "FilePath" << "Filter";
    //model->setColumnCount(hLabels.length());
    model->setHorizontalHeaderLabels(hLabels);

    proxyModel = new QSortFilterProxyModel();
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(C_FILTER);


    treeView = new QTreeView(this);
    treeLayout->addWidget(treeView);
    treeView->setModel(proxyModel);

    treeView->setRootIsDecorated(false);
    treeView->setUniformRowHeights(true);
    treeView->setSortingEnabled(true);
    treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    treeView->setSelectionBehavior(QAbstractItemView::SelectRows);

    treeView->header()->setStretchLastSection(true);
    treeView->setColumnWidth(C_DIR, 60);
    treeView->setColumnWidth(C_FDM, 60);
    treeView->setColumnWidth(C_DESCRIPTION, 200);
    treeView->setColumnHidden(C_XML_FILE, true);
    treeView->setColumnHidden(C_FILE_PATH, true);
    treeView->setColumnHidden(C_FILTER, true);
    connect( treeView->selectionModel(),
             SIGNAL( selectionChanged(const QItemSelection &, const QItemSelection & ) ),
             SLOT( on_tree_selection_changed() )
    );



    statusBarTree = new QStatusBar();
    statusBarTree->setSizeGripEnabled(false);
    treeLayout->addWidget(statusBarTree);

    //== Path label
    labelAeroPath = new QLabel();
    statusBarTree->addPermanentWidget(labelAeroPath);

    //== View nested Checkbox
    /*checkViewNested = new QCheckBox();
    checkViewNested->setText("View folders");
    statusBarTree->addPermanentWidget(checkViewNested);
    connect(checkViewNested, SIGNAL(clicked()), this, SLOT(load_tree()));*/

    //== Reload aircrafts
    QToolButton *actionReloadCacheDb = new QToolButton(this);
    actionReloadCacheDb->setText("Reload");
    actionReloadCacheDb->setIcon(QIcon(":/icon/load"));
    actionReloadCacheDb->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    actionReloadCacheDb->setAutoRaise(true);
    statusBarTree->addPermanentWidget(actionReloadCacheDb);
    connect(actionReloadCacheDb, SIGNAL(clicked()), this, SLOT(on_reload_cache()) );


    //================================================================================================
    //= Right
    //================================================================================================


    QGroupBox *grpAero = new QGroupBox();
    splitter->addWidget(grpAero);
    grpAero->setDisabled(false);
    grpAero->setTitle(tr("Preview, Radio, Fuel"));


    QVBoxLayout *aeroLayout = new QVBoxLayout();
    grpAero->setLayout(aeroLayout);

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

    //== Main Settings connection
    connect(this, SIGNAL(setx(QString,bool,QString)), mainObject->X, SLOT(set_option(QString,bool,QString)) );
    connect(mainObject->X, SIGNAL(upx(QString,bool,QString)), this, SLOT(on_upx(QString,bool,QString)));

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

    qDebug() << "on_tree_selection_changed" ;
    //QTreeWidgetItem *item = treeWidget->currentItem();
    if(treeView->selectionModel()->hasSelection() == false){
        outLog("on_tree_selection_changed: no selected item");
        labelAeroPath->setText("nooooo");
        emit setx("--aircraft=", false, "");
        //buttonAeroPath->setDisabled(true);
        return;
    }

    QModelIndex midx = treeView->selectionModel()->selectedIndexes().at(C_DIR);

    QStandardItem *item = model->itemFromIndex( proxyModel->mapToSource(midx)  );

    //= Get the thumbnail image
    QString thumb_file = QString("%1/%2/thumbnail.jpg").arg(mainObject->X->aircraft_path(), item->text());

    //qDebug() << "on_tree_selection_changed" << thumb_file << item->text();
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
}

//============================================
//== Select an hangar/custom aircraft directory path

void AircraftWidget::on_custom_hangar_path() {
    emit setx("custom_hangar_enabled", checkBoxUseCustomHangar->isChecked(), "");
    emit setx("--fg-aircraft=", checkBoxUseCustomHangar->isChecked(), txtAircraftPath->text());

    if (txtAircraftPath->text() != "") {
        on_reload_cache();
    }else {
        QMessageBox::warning(this, tr("Custom Aircraft Directory"),
            tr("Click on \"select\" and set a valid path to an aircraft directory containing one or more aircrafts."),
            QMessageBox::Ok);
    }
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

    QModelIndex midx = treeView->selectionModel()->selectedIndexes().at(C_AERO);
    QStandardItem *item = model->itemFromIndex( proxyModel->mapToSource(midx)   );
    return item->text();
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

    model->removeRows(0, model->rowCount());
    statusBarTree->showMessage("Reloading cache");

    QProgressDialog progress(this);
    QSize size(320,100);
    progress.resize(size);
    progress.setWindowIcon(QIcon(":/icon/load"));

    AircraftData::import(progress, mainObject);
    load_tree();
}

//=============================================================
// Load Aircaft To Tree
void AircraftWidget::load_tree(){
    int c =0;

    model->removeRows(0, model->rowCount());


    QFile dataFile(mainObject->data_file(("aircraft.txt")));
    if (!dataFile.open(QIODevice::ReadOnly | QIODevice::Text)){
           //TODO  - catch error
           return;
    }
    QTextStream in(&dataFile);
    QString line = in.readLine();
    QString last_dir("");
    int countaircrafts = 0;
    QList<QStandardItem*> row;
    c = 0;
    while(!line.isNull()){

        QStringList cols = line.split("\t");


        // Add model row
        row = this->create_model_row();
        row.at(C_DIR)->setText(cols.at(C_DIR));
        row.at(C_DIR)->setIcon(QIcon(":/icon/folder"));

        row.at(C_AERO)->setText(cols.at(C_AERO));
        row.at(C_AERO)->setIcon(QIcon(":/icon/aircraft"));
        QFont f = row.at(C_AERO)->font();
        f.setBold(true);
        row.at(C_AERO)->setFont(f);

        row.at(C_DESCRIPTION)->setText(cols.at(C_DESCRIPTION));
        row.at(C_FDM)->setText(cols.at(C_FDM));
        row.at(C_AUTHOR)->setText(cols.at(C_AUTHOR));
        row.at(C_XML_FILE)->setText(cols.at(C_XML_FILE));
        row.at(C_FILE_PATH)->setText(cols.at(C_FILE_PATH));

        QString filter_str = cols.at(C_AERO);
        filter_str.append(cols.at(C_DESCRIPTION) );
        row.at(C_FILTER)->setText( filter_str );
        c++;
        line = in.readLine();
    }

    /*treeWidget->sortByColumn(view == FOLDER_VIEW ? C_DIR : C_AERO, Qt::AscendingOrder);*/
    treeView->sortByColumn(C_DIR, Qt::AscendingOrder);
    treeView->sortByColumn(C_AERO, Qt::AscendingOrder);


    select_node(mainObject->X->getx("--aircraft="));
    QString str = QString("%1 aircrafts").arg(countaircrafts)+QString(", %1 models").arg(c);
    statusBarTree->showMessage(str);
    outLog("*** FGx: AircraftWidget::load_tree: with " + str);
}

QList<QStandardItem*> AircraftWidget::create_model_row(){
    QList<QStandardItem*> lst;
    lst << new QStandardItem() << new QStandardItem() << new QStandardItem() << new QStandardItem() << new QStandardItem() << new QStandardItem() << new QStandardItem() << new QStandardItem();
    model->appendRow(lst);
    return lst;
}

//=============================================================
// Initialize
void AircraftWidget::initialize(){

    static bool first_load_done = false;
    if(first_load_done){
        return;
    }
    if (!QFile::exists(mainObject->data_file("aircraft.txt"))){
        statusBarTree->showMessage("*** FGx: No cached data. Use set paths and reload");
    }else{
        load_tree();
    }
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

    }else if(option == "custom_hangar_enabled"){
        checkBoxUseCustomHangar->setChecked(enabled);

    }else if(option == "--fg-aircraft="){
        txtAircraftPath->setText(value);

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


void AircraftWidget::on_select_path()
{
#ifdef USE_ALTERNATE_GETFILE
    QString dirPath = util_getDirName(this, tr("Select Hangar (Aircraft directory)"),
                                            txtAircraftPath->text());
#else // !#ifdef USE_ALTERNATE_GETFILE
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select Hangar (Aircraft directory)"),
                                                         txtAircraftPath->text(), QFileDialog::ShowDirsOnly);
#endif // #ifdef USE_ALTERNATE_GETFILE y/n
    if(dirPath.length() > 0){
        txtAircraftPath->setText(dirPath);
    }

    on_custom_hangar_path();

}


void AircraftWidget::on_open_aircraft_path()
{
    if(labelAeroPath->text().length() == 0){
        return;
    }
    QUrl url(QString("file://%1").arg(labelAeroPath->text()));
    QDesktopServices::openUrl(url);
}
