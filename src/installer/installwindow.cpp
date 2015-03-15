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

