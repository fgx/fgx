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

//SvnCpp
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

    QString fgsvn("http://svn.code.sf.net/p/flightgear/fgaddon/trunk/");

    // wtf, where does contextP come from
    svn::Context context; // = new svn::Context();

    svn::Client *client = svn::Client::getobject(&context, 0);
    svn::Path path("/");

    svn::CheckoutParameter checkoutRarams;
    checkoutRarams.moduleName(fgsvn).destination("/home/fgxl/svn-test/").depth(svn::DepthEmpty);
    client->checkout(checkoutRarams);
    //svn::InfoEntry infoEntry;
    //infoEntry = client->info(path, false, svn::Revision::UNDEFINED, svn::Revision::UNDEFINED);


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

