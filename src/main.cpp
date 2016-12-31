//             ____
//       ,,-'''    `'''--.                  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//     ,'                 `.                FGx FlightGear Launcher
//   ,'                     \               -=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//  ,'                     / '              (c) 2010-2013
// /                  ,---[  ||             Yves Sablonier, Pete Morgan
// |             _,.--|     /  \            Geoff McLane
// |         .-''     '   /'  ,',-._
// |          '---...--'''_.']-'    `..     General Public Licence,
// |                 _,.-:.-'          '-.  Version 2 (GNU GPLv2)
//  `.____,,....--=::.--'             ,,'
//           -\'''                 _,'      This program is free software
//             `._               ,'        You can redistribute it
//                `._         _,'       and/or modify it under the terms
//                   ``------'      of the GNU General Public Licence
//                                as published by the Free Software
// Foundation. This program is distributed in the hope it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// ***********************************************************************


#include <QApplication>
#include <stdio.h>
#include <stdlib.h>
#include <QFile>
#include <QTextStream>
#include <QFontDatabase>
#include <QSettings>


#include "xobjects/mainobject.h"
#include "utilities/utilities.h"
#include "xobjects/xsettings.h"

// Global Message handler
// All debug messages are send here for processing to
// log files, console and alike
void fgxErrorMessage(QtMsgType type, const char *msg)
{
    QString m = msg;
    switch (type)
    {
    case QtDebugMsg:
        outLog("*** FGx debug: "+m+" ***\n",0);
        break;
    case QtWarningMsg:
        outLog("*** FGx warning: "+m+" ***\n",0);
        break;
    case QtCriticalMsg:
        outLog("*** FGx critical: "+m+" ***\n",0);
        break;
    case QtFatalMsg:
        outLog("*** FGx fatal: "+m+" ***\n",0);
        abort();
        break;
    }
}

int main( int argc, char* argv[])
{

    QApplication app(argc, argv);
    QApplication::setOrganizationName(APP_ORG);
    QApplication::setOrganizationDomain(APP_DOMAIN);
    QApplication::setApplicationName(APP_SHORT);

    // from /version file via cmake
    QApplication::setApplicationVersion(APP_VERSION);


    //== Starts the application here as object, visible in SysTray
    MainObject mainObject;

    QSettings startsettings;

    //* check for dev_mode, install message handler
    if( XSettings::dev_mode() == false ){
        //qInstallMsgHandler(fgxErrorMessage);
    }


    return app.exec();


}






