/*
 *  main.cpp
 *  FGx
 *
 *  Created by Yves Sablonier, Zurich
 *  © 2011 --- GPL2
 *
 */

#include <QApplication>
#include <stdio.h>
#include <stdlib.h>
#include <QFile>
#include <QTextStream>
#include <QFontDatabase>


#include "xobjects/mainobject.h"
#include "utilities/utilities.h"
#include "xobjects/xsettings.h"

//* message handler
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
	QApplication::setOrganizationName("FGx");
	QApplication::setOrganizationDomain("googlecode.com");
	QApplication::setApplicationName("FGx");
	QApplication::setApplicationVersion("2.4.0a2");
	
	//=====================================================
	//== Add fonts to database, set app fonts
	QFontDatabase::addApplicationFont(":/font/logisoso");
	QFontDatabase::addApplicationFont(":/font/freeuniversalregular");
	QFontDatabase::addApplicationFont(":/font/freeuniversalbold");
	
	QFont universalregular("FreeUniversal", 12);
	QFont universalbold("FreeUniversal", 12);
	universalbold.setBold(false);
	universalbold.setLetterSpacing(QFont::AbsoluteSpacing, 0.7);
	app.setFont(universalbold);
	
	//=====================================================
	//== global styles
	QString styles;
	styles.append("QWidget { font-family: FreeUniversal }");
	styles.append("QLineEdit { background-color: white }");
	styles.append("QGroupBox { font-size: 12px }");
	styles.append("QPushButton { font-family: FreeUniversal; font-size: 12px; padding: 5px; }");
	styles.append("QStatusBar { font-family: Andale mono, Lucida Console, monospace; font-size: 12px; font-weight: normal }");
	styles.append("LauncherWindow { background-color: #eeeeee }");
	styles.append("QMenuBar { color: #eeeeee; background-color: #333333 }");
	styles.append("QMenu::item { color: #333333; background-color: #eeeeee }");
	styles.append("QMenu::item::selected { color: #eeeeee; background-color: #999999 }");
	styles.append("QTreeView::item{ padding: 2px;}");
	styles.append("QPlainTextEdit { font-family: Andale mono, Lucida Console, monospace; font-size: 12px; font-weight: normal }");
				
	app.setStyleSheet(styles);
	
	//== Start the application here
	MainObject mainObject;

	//* check for dev_mode, install message handler
	if( XSettings::dev_mode() == false ){
		qInstallMsgHandler(fgxErrorMessage);
	}

	return app.exec();
	

}






