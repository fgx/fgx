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


    //==========================================
    //= Aircraft Tab
    this->aircraftInstallWidget = new AircraftInstallWidget(mainObject);
    tabWidget->addTab( this->aircraftInstallWidget, "FG Addon");





}


void InstallWindow::moveEvent(QMoveEvent *ev){
    Q_UNUSED(ev);
    this->mainObject->settings->saveWindow(this);
}
