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
	QApplication::setApplicationName("FGx FlightGear Launcher");
	QApplication::setApplicationVersion("2.4.0b1");
	
	//=====================================================
	//== Add fonts to database and set default application font
	
	// This font comes very close to a DIN font, but we do not use yet
	//QFontDatabase::addApplicationFont(":/font/logisoso");
	
	// Default font for the whole application
	QFontDatabase::addApplicationFont(":/font/freeuniversalregular");
	QFontDatabase::addApplicationFont(":/font/freeuniversalbold");
	
	// Setting a default size too, and spacing, because we need it on screen
	QFont universal("FreeUniversal", 14);
	universal.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
	app.setFont(universal);
	
	//=====================================================
	//== Global styles
	
	// yellow: #ffff00, blue: #007eff, light-gray (default background): #fffee0
	// darker gray: #999999 (box titles), lightgray #dddddd (lines and borders)
	
	QString styles("");
	styles.append("QWidget, QLabel { font-family: FreeUniversal; font-size: 12px; }");
	styles.append("QLineEdit { background-color: #fffee0; border: 1px solid #dddddd }");
	styles.append("QGroupBox { background-color: #ffffff; border-top: 24px solid #ffffff;}");
	styles.append("QGroupBox::title {subcontrol-origin: margin; subcontrol-position: top left; border: 0px; color: #fffee0; background-color: #999999; padding: 4px;}");
	styles.append("QPushButton { font-family: FreeUniversal; padding: 5px; }");
	styles.append("QComboBox, QLineEdit, QSpinBox, QTextEdit, QListView { border: 0px; margin: 0px; padding: 0px; background-color: #fffee0; selection-color: #ffffff;  selection-background-color: #007eff; }");
	styles.append("QListView {show-decoration-selected: 1;}");
	styles.append("QListView::item:hover {background: #eeeeee; }");
	
	styles.append("LauncherWindow { background-color: #eeeeee }");
	
	//== Other Tabs
	styles.append("QTabWidget::pane { background-image: url(:/artwork/background-gray-tabbar); border: 0px; padding-top: 10px; }");
	styles.append("QTabBar { background-color: #b0b0b0 }");
	//styles.append("QTabBar::tab::hover { background-color: #ffffff }");
	//styles.append("QTabBar::tab:selected {  }");
	//styles.append("QTabBar::tab {  padding: 6px; border-right: 5px solid #eeeeee }");


	//== Launcher Tabs
	styles.append("QTabWidget#launcher_tabs::pane { background-image: url(:/artwork/background-gray-tabbar); border: 0px; padding-top: 20px; }");
	styles.append("QTabWidget#launcher_tabs > QTabBar { background-color: #b0b0b0 }");
	styles.append("QTabWidget#launcher_tabs > QTabBar::tab::hover { background-color: #ffffff }");
	styles.append("QTabWidget#launcher_tabs > QTabBar::tab:selected { background-image: url(:/artwork/boxtop-black-tabbar); }");
	styles.append("QTabWidget#launcher_tabs > QTabBar::tab { background-image: url(:/artwork/boxtop-black-inactive); padding: 6px; border-right: 5px solid #eeeeee }");


	styles.append("QMenuBar { color: #eeeeee; background-color: #333333 }");
	styles.append("QMenu::item { color: #333333; background-color: #eeeeee }");
	styles.append("QMenu::item::selected { color: #eeeeee; background-color: #b0b0b0 }");
	styles.append("QTreeView::item{ padding: 2px;}");
	styles.append("QPlainTextEdit { font-family: Andale mono, Lucida Console, monospace; font-size: 12px; background-color: #fffee0; border: 1px solid #dddddd }");
	
	styles.append("ExeControls { background-color: #ffffff; border: 0px; margin-top: 22px; }");
	styles.append("ExeControls::title { color: #000000; background-color: yellow; margin-top: 0px; margin-left: 0px; }");
	
	app.setStyleSheet(styles);
	
	//== Start the application here
	MainObject mainObject;

	//* check for dev_mode, install message handler
	if( XSettings::dev_mode() == false ){
		qInstallMsgHandler(fgxErrorMessage);
	}

	return app.exec();
	

}






