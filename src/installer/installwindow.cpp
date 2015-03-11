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
#include "svnqt/svnqttypes.h"
#include "svnqt/context.h"
#include "svnqt/client.h"
#include "svnqt/client_commit_parameter.h"
#include "svnqt/revision.h"
#include "svnqt/status.h"
#include "svnqt/targets.h"
#include "svnqt/url.h"
#include "svnqt/wc.h"
#include "svnqt/client_parameter.h"
#include "svnqt/client_update_parameter.h"
#include "svnqt/smart_pointer.h"

InstallWindow::InstallWindow(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

    mainObject = mob;




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
    txtSvnCheckoutPath->setText( this->mainObject->settings->value("fgx_extras") );

    QToolButton *buttSvnInit = new QToolButton();
    buttSvnInit->setText("Init SVN");
    layAddOnTop->addWidget(buttSvnInit, 0);
    this->connect(buttSvnInit, SIGNAL(clicked()),
                  this, SLOT(on_init_svn())
                  );
    //==========================================
    //= TreeView
    treeView = new QTreeView(this);
    layAddon->addWidget(treeView, 10);
    treeView->setModel(this->model);
    //treeView->setModel(this->proxyModel);


    //==========================================
    //= StatusBar
    statusBar = new QStatusBar();
    layAddon->addWidget(statusBar);



}


void InstallWindow::moveEvent(QMoveEvent *ev){
    Q_UNUSED(ev);
    this->mainObject->settings->saveWindow(this);
}


void InstallWindow::on_init_svn(){

    QString target_path = this->txtSvnCheckoutPath->text();
    if(target_path.length() == 0){
        // @todo validate path
        return;
    }
    this->mainObject->settings->setValue("fgx_extras", target_path);
    QFileInfo info(target_path);


    QString svn_url("http://svn.code.sf.net/p/flightgear/fgaddon/trunk/");

    // wtf, where does contextP come from
    svn::Context context; // = new svn::Context();

    svn::Client *client = svn::Client::getobject(&context, 0);
    //svn::Path path("/");

    svn::CheckoutParameter checkoutRarams;
    checkoutRarams.moduleName(svn_url)
            .destination("/home/fgxl/svn-test/")
            .depth(svn::DepthImmediates)
            .revision(svn::Revision::HEAD);
    try {
        //client->checkout(checkoutRarams);
        client->checkout(checkoutRarams);
    } catch(svn::ClientException ce) {
        qDebug() << "error=" << ce.msg();

    }

    //svn::InfoEntry infoEntry;
    //infoEntry = client->info(path, false, svn::Revision::UNDEFINED, svn::Revision::UNDEFINED);


}
