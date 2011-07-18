

//#include <QtCore/QDebug>

#include <QtCore/QByteArray>
#include <QtCore/QFile>
#include <QtCore/QStringList>


#include <QtGui/QVBoxLayout>
#include <QtGui/QToolBar>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QToolButton>
#include <QtGui/QMenu>
#include <QtGui/QLabel>
#include <QtGui/QSplitter>

#include <QtWebKit/QWebFrame>
#include <QtGui/QDesktopServices>

#include "mpmap/xopenlayer.h"




XOpenLayerWidget::XOpenLayerWidget(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mob;
	
	setWindowTitle(tr("FGx OpenLayer Map"));
	setWindowIcon(QIcon(":/icon/mpmap"));
	
	setProperty("settings_namespace", QVariant("xopenlayerwidget_window"));
	mainObject->settings->restoreWindow(this);

	//======================================================
	//= Main Layout and Splitter
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
	
	QLabel *label_notice = new QLabel();
	label_notice->setText("This openlayer widget is currently work in progress ...");
	label_notice->setStyleSheet("background-color: #eeeedd; color: #000099; padding: 5px;");
	mainLayout->addWidget(label_notice, 0);
	
	QSplitter *splitter = new QSplitter();
	mainLayout->addWidget(splitter, 10);
	
	webView = new QWebView();
	mainLayout->addWidget(webView, 500);
	
	init_map();
	
}
	//===========================================================================
	//== Initialisaztion
	//===========================================================================
	void XOpenLayerWidget::init_map(){
		
		static bool map_initialized = false;
		if(map_initialized == false){
			//= Read file if in dev_mode() - no need to "recompile" the resource file
			QFile file(
					   mainObject->settings->dev_mode()
					   ? XSettings::fgx_current_dir().append("/resources/openlayers/fgx-map/fgx-map.html")
					   : ":/fgx-map/fgx-map"
					   );
			if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
				qDebug("FGx Map: can not open map file");
				return;
			}
			
			//webView->page()->mainFrame()->addToJavaScriptWindowObject("Qt", this);
			QByteArray contents = file.readAll();
			qDebug() << contents;
			webView->setHtml(contents);
			
		}
		map_initialized = true;
	}
	
