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

#include "svncpp/client.hpp"
#include "svncpp/info.hpp"
#include "svncpp/dirent.hpp"


InstallWindow::InstallWindow(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

    mainObject = mob;

    svn::Context context("/home/fgxx/svn-test");

    svn::Client client(&context);

    svn::Path path("/");

    svn::InfoVector infoVector;
    infoVector = client.info(path, false, svn::Revision::UNSPECIFIED, svn::Revision::UNSPECIFIED);

    setWindowIcon(QIcon(":/icon/metar"));
    setWindowTitle("Installer");
    setMinimumWidth(600);




    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    QLabel *labelHeader = new QLabel("Installer");
    labelHeader->setStyleSheet("background-color: black; color: white; font-size: 22pt; padding: 5px; font-family: courier");
    mainLayout->addWidget(labelHeader);

}

