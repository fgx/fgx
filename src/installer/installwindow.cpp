// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // installerdialog.cpp
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2015
// Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information


#include <QLabel>
#include <QVBoxLayout>

#include "installwindow.h"

//#include "svnclient.h"

// apt-get libsvnqt
// https://github.com/anrichter/qsvn/blob/master/src/CMakeLists.txt
// https://github.com/anrichter/qsvn/blob/master/src/svnclient.cpp

/*
https://gcc.gnu.org/wiki/SvnTricks

svn checkout --depth immediates http://svn.code.sf.net/p/flightgear/fgaddon/trunk/ ./fgaddon

## list all aircraft directories
svn list Aircraft/

# get the root files inc -set.xml
svn update --set-depth files ./Aircraft/tu154

## download whole aero
svn update --set-depth infinity ./Aircraft/787


*/

// svnqt
// I think its this said pedro - https://projects.kde.org/projects/playground/devtools/kdesvn/repository
//#include "svnqt/svnqt_defines.h"

InstallWindow::InstallWindow(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

    mainObject = mob;

    //= Setup svn client
    //#svn::Context context; // = new svn::Context();
    //this->svnClient = svn::Client::getobject(&context, 0);


    //==========================================
    //= Setup Models
    this->model = new QStandardItemModel(this);
    this->proxyModel = new QSortFilterProxyModel(this);

    QStringList hLabels;
    hLabels << "Dir" << "Aero" << "Description" << "FDM" << "Authors" << "XML" << "FilePath" << "FilterDir" << "Filter" << "BASE";
    model->setHorizontalHeaderLabels(hLabels);

    //==========================================
    //= Window + Layout
    setProperty("settings_namespace", QVariant("install_window"));
    mainObject->settings->restoreWindow(this);

    setWindowIcon(QIcon(":/icon/install"));
    setWindowTitle("Installer");
    setMinimumWidth(600);

    setWindowTitle(QDir::currentPath());

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //==========================================
    //= Header Label
    QLabel *labelHeader = new QLabel("Installer");
    labelHeader->setStyleSheet("background-color: black; color: white; font-size: 20pt; padding: 5px; font-family: sans-serif;");
    mainLayout->addWidget(labelHeader);

    mainLayout->addSpacing(10);

    //==========================================
    //= Tab Widget
    tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget, 10);

    //====================================================================
    // FgAddons
    //====================================================================

    //= Container widget with layout
    QWidget *widgetAddOns = new QWidget();
    tabWidget->addTab(widgetAddOns, "FG Addons");
    QVBoxLayout *layAddon = new QVBoxLayout();
    layAddon->setContentsMargins(0,0,0,0);
    layAddon->setSpacing(0);
    widgetAddOns->setLayout(layAddon);


    //= Temp path for now
    QHBoxLayout *layAddOnTop = new QHBoxLayout();
    layAddOnTop->setContentsMargins(0,0,0,0);
    layAddOnTop->setSpacing(0);
    layAddon->addLayout(layAddOnTop);

    layAddOnTop->addWidget(new QLabel("Checkout Path:"), 0);

    txtSvnCheckoutPath = new QLineEdit();
    layAddOnTop->addWidget(txtSvnCheckoutPath, 2);
    txtSvnCheckoutPath->setText( this->mainObject->settings->value("install_path").toString() );

    QToolButton *buttSvnInit = new QToolButton();
    buttSvnInit->setText("Init SVN");
    layAddOnTop->addWidget(buttSvnInit, 0);
    this->connect(buttSvnInit, SIGNAL(clicked()),
                  this, SLOT(svn_init())
                  );
    //==========================================
    //= TreeView
    treeView = new QTreeView(this);
    layAddon->addWidget(treeView, 10);
    treeView->setModel(this->model);
    treeView->setRootIsDecorated(false);
    //treeView->setModel(this->proxyModel);


    //==========================================
    //= StatusBar
    statusBar = new QStatusBar();
    layAddon->addWidget(statusBar);

    progressBar = new QProgressBar();
    statusBar->addPermanentWidget(progressBar);
    progressBar->hide();

}


void InstallWindow::moveEvent(QMoveEvent *ev){
    Q_UNUSED(ev);
    this->mainObject->settings->saveWindow(this);
}

QList<QStandardItem*> InstallWindow::create_model_row(){
    QList<QStandardItem*> lst;
    for(int i = 0; i < this->model->columnCount(); i++){
        lst.append( new QStandardItem() );
    }
    model->appendRow(lst);
    return lst;
}

void InstallWindow::svn_init(){

    QString install_path = this->txtSvnCheckoutPath->text().trimmed();
    if(install_path.length() == 0){
        // @todo validate path
        return;
    }
    this->mainObject->settings->setValue("install_path", install_path);
    this->mainObject->settings->sync();

    QFileInfo info(install_path);

    QString target_path = QDir::fromNativeSeparators(info.absoluteFilePath());
    target_path.append("/fgaddon");

    QString svn_url("http://svn.code.sf.net/p/flightgear/fgaddon/trunk/");

    svn::Context context; // = new svn::Context();
    svn::Client *svnClient = svn::Client::getobject(&context, 0);

    //= first we clean up as svn gets corrupt !!
    svnClient->cleanup(target_path);

    //= Initial Checkout - this runs regardless atmo
    // @todo determine is a repos exists and is valid
    svn::CheckoutParameter params;
    params.moduleName( svn_url )
                .destination( QDir::toNativeSeparators(target_path) )
                .depth( svn::DepthImmediates )
                .revision( svn::Revision::HEAD );

    qDebug() << "Checking out=" << QString("Checking Out: ").append(svn_url);
    //= Do checkout
    statusBar->showMessage( QString("Checking Out: ").append(svn_url) );
    progressBar->setRange(0, 0);
    progressBar->show();
    try {
        svnClient->checkout(params);
    } catch(svn::ClientException ce) {
        qDebug() << "error=" << ce.msg();
        statusBar->showMessage( QString("ERROR: ").append(ce.msg()) );
        return;
    }
    qDebug() << "Inital Checkout OK";


    svn::UpdateParameter uparams;
    uparams.targets(QDir::toNativeSeparators(target_path + "/Aircraft"))
                .depth( svn::DepthImmediates )
                .revision( svn::Revision::HEAD )
                .sticky_depth(true);
    svn::Revisions toRevisions;

    try {
        toRevisions = svnClient->update(uparams);
        qDebug() << " udpate=" << toRevisions.length();

    } catch(svn::ClientException ce) {
        qDebug() << "error=" << ce.msg();
        statusBar->showMessage( QString("ERROR: ").append(ce.msg()) );
        return;
    }

    //this->svn_list_aircraft();


}

void InstallWindow::svn_list_aircraft(){

    statusBar->showMessage( QString("Getting List") );

    svn::Context context; // = new svn::Context();
    svn::Client *svnClient = svn::Client::getobject(&context, 0);

    QString aircraft_path = this->mainObject->settings->value("install_path").toString().append("/fgaddon").append("/Aircraft");
    qDebug() << "target_path=" << aircraft_path;

    //= List Directories
    svn::DirEntries entries;
    try {
        entries = svnClient->list(aircraft_path, svn::Revision::HEAD, svn::Revision::HEAD,svn::DepthImmediates, false);

    } catch(svn::ClientException ce) {
        qDebug() << "error=" << ce.msg();
        statusBar->showMessage( QString("ERROR: ").append(ce.msg()) );
        return;
    }
    qDebug() << "GOT LIST=" << entries.count();

    //= Add entries to model
    progressBar->setRange(0, entries.count());
    progressBar->show();
    for(int i = 0; i < entries.count(); i++){
        progressBar->setValue(i);
        svn::DirEntry *dir = entries.at(i);
        qDebug() << " =" << dir->isDir() << "=" << dir->name();

        if( dir->name().length() > 0){ // first entry is blank!

            QList<QStandardItem *> items = this->create_model_row();
            QStandardItem *dItem = items.at(0);
            dItem->setText( dir->name() );
            dItem->setIcon( QIcon(":/icon/custom_folder") );
        }
    }



    //= Loop our model rows, and get the base files
    progressBar->setRange(0, this->model->rowCount());
    svn::Revisions toRevisions;
    for(int ridx = 0; ridx < this->model->rowCount(); ridx++){
        progressBar->setValue(ridx);
        QString adir = model->item(ridx, 0)->text();
        QString aero_path = aircraft_path;
        aero_path.append("/").append(adir);
        qDebug() << " =" << adir << aero_path;

        //svn update --set-depth files ./Aircraft/tu154
        svn::UpdateParameter params;
        params.targets(aero_path)
                    .depth( svn::DepthFiles )
                    .revision( svn::Revision::HEAD );

        try {
            toRevisions = svnClient->update(params);
            qDebug() << " udpate=" << toRevisions.length();

        } catch(svn::ClientException ce) {
            qDebug() << "error=" << ce.msg();
            statusBar->showMessage( QString("ERROR: ").append(ce.msg()) );
            return;
        }
    }

}
