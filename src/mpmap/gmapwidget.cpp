

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

#include "mpmap/mpmapxwidget.h"
#include "mpmap/xwebpage.h"


/*
QList<int, int> zoomLevelMap

zoomLevelMap[5000] = 1;
zoomLevelMap[1500] = 2;
zoomLevelMap[2000] = 3;
zoomLevelMap[1000] = 4;
zoomLevelMap[500] = 5;
zoomLevelMap[200] = 6;
zoomLevelMap[100] = 7;
zoomLevelMap[100] = 8;
zoomLevelMap[100] = 9;
zoomLevelMap[100] = 10;
zoomLevelMap[100] = 11;

zoomLevelMap[100] = 12;
zoomLevelMap[100] = 13;
zoomLevelMap[100] = 14;

*/
GMapWidget::GMapWidget(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

	mainObject  = mob;


	//======================================================
	//= Main Layout and Splitter
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

	//=============================
	//= Toolbar
    QToolBar *toolbar = new QToolBar();
	mainLayout->addWidget(toolbar, 0);

	QAction *actionInitMap = new QAction(this);
	actionInitMap->setText("Init Map");
	toolbar->addAction(actionInitMap);
	connect(actionInitMap, SIGNAL(triggered()), this, SLOT(init_map()));



	//= Map Type
    QActionGroup *groupMapType = new QActionGroup(this);
    groupMapType->setExclusive(true);
    //connect(groupMapType, QtCore.SIGNAL("triggered(QAction *)"), this, self.on_map_type)

    QToolButton *buttMapType = new QToolButton();
    toolbar->addWidget(buttMapType);
    buttMapType->setText("XXXX");
    buttMapType->setPopupMode(QToolButton::InstantPopup);
    buttMapType->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    QMenu *popTypeMenu = new QMenu(buttMapType);
    buttMapType->setMenu(popTypeMenu);

	//    for b in ['RoadMap', 'Satellite','Hybrid','Terrain']:
	//            act = QtGui.QAction(popTypeMenu)
	//            act->setText(b)
	//            act->setCheckable(True)
	//            if b == "Hybrid": #self.main.settings.value("map_mode", "Hybrid"):
	//                    act.setChecked(True)
	//                    self.buttMapType.setText(b)
	//
	//            popTypeMenu.addAction(act)
	//            self.groupMapType.addAction(act)

    toolbar->addSeparator();



	//== Lat Lng Labels
	toolbar->addWidget(new QLabel("Lat:"));
	lblLat = new QLabel();
	//lblLat.setStyleSheet(style)
	lblLat->setFixedWidth(80);
	lblLat->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	toolbar->addWidget(lblLat);

	//QLabel *lblLng = QLabel();
	//toolbar.addAction("Lng:")
	toolbar->addWidget(new QLabel("Lng:"));
	lblLng = new QLabel();
	//lblLng->setStyleSheet(style);
	lblLng->setFixedWidth(80);
	toolbar->addWidget(lblLng);



	//=================================================================
	// WebView
    webView = new QWebView();
	//XWebPage *webPage = new XWebPage();
	//webView->setPage(webPage);
	mainLayout->addWidget(webView, 100);

	//=== Register Qt Widget
	//webView->page()->mainFrame()->addToJavaScriptWindowObject("Qt", this);

	//=============
	//== Network cache /cookies
	networkDiskCache = new QNetworkDiskCache(this);
	networkDiskCache->setCacheDirectory(QDesktopServices::storageLocation(QDesktopServices::CacheLocation));
	webView->page()->networkAccessManager()->setCache(networkDiskCache);

	networkCookieJar = new QNetworkCookieJar(this);


	//= Net connections
	connect(webView, SIGNAL(loadStarted()), this, SLOT(start_progress()));
	connect(webView, SIGNAL(loadProgress(int)), this, SLOT(update_progress(int)));
	connect(webView, SIGNAL(loadFinished(bool)), this, SLOT(end_progress(bool)));

	//=====================
	//==Status Bar
    statusBar = new QStatusBar();
	mainLayout->addWidget(statusBar);

	//= Progress Bar
	progressBar = new QProgressBar();
	progressBar->setVisible(false);
	statusBar->addPermanentWidget(progressBar);


	//== Zoooomm
    statusBar->addPermanentWidget(new QLabel("Zoom:"));
	groupZoom = new QButtonGroup(this);
    groupZoom->setExclusive(true);
	connect(groupZoom, SIGNAL(buttonClicked(QAbstractButton*)),
			this, SLOT(on_zoom_action(QAbstractButton*))
    );
	for(int z=1; z < 15; z++){
		QToolButton *act = new QToolButton(this);
        act->setText(QString(" %1 ").arg(z));
        act->setProperty("zoom", QVariant(z));
        act->setCheckable(true);
		act->setAutoRaise(true);
		act->setDown(z == 12);
		statusBar->addPermanentWidget(act);
		groupZoom->addButton(act);
    }




	init_map();

}






//===========================================================================
// Called from JS
//===========================================================================
void GMapWidget::map_debug(QVariant mess){
	qDebug() << "< " << mess.toString();
}


//= JS - map_mouse_move
// - this is not firing ;-(
void GMapWidget::map_mouse_move(QVariant dLat, QVariant dLng){
    //qDebug("YES");
	//qDebug() << "map_mouse_move" << dLat.toString();
    lblLat->setText(dLat.toString());
    lblLng->setText(dLng.toString());
}


//== JS - map_click()
void GMapWidget::map_click(QVariant lat, QVariant lng){
	Q_UNUSED(lat);
	Q_UNUSED(lng);
	//qDebug() << "map_click()" << lat << lng;
}

//== JS - map_right_click()
void GMapWidget::map_right_click(QVariant lat, QVariant lng){
	Q_UNUSED(lat);
	Q_UNUSED(lng);
	//qDebug() << "map_right_click()" << lat << lng;
}






void GMapWidget::map_error(QVariant err){
	//qDebug("map_error()");
	Q_UNUSED(err);
}

void GMapWidget::marker_clicked(QVariant marker, QVariant mId){
	//qDebug("marker_clicked()");
		Q_UNUSED(marker);
			Q_UNUSED(mId);
}

void GMapWidget::marker_unselected(QVariant curr_idx, QVariant mLocationId){
	//qDebug("marker_unselected()");
		Q_UNUSED(curr_idx);
			Q_UNUSED(mLocationId);
}



//** JS
void GMapWidget::map_zoom_changed(QVariant zoom){
	//qDebug() << "map_zoom_changed" << zoom;
	return;
    buttZoom->setText(zoom.toString());
    int zoomInt = zoom.toInt();
	QList<QAbstractButton *> buttons = groupZoom->buttons();
	for (int i = 0; i < buttons.size(); ++i) {
		if (buttons.at(i)->property("zoom").toInt() == zoomInt){
			 buttons.at(i)->setChecked(true);
             return;
        }
     }
//    for a in self.groupZoom.actions():
//            checked = a.property("zoom").toString() == new_zoom_level
//            a.setChecked(checked)
      qDebug("map_zoom_changed()");
}

void GMapWidget::on_zoom_action(QAbstractButton *button){
	//Q_UNUSED(idx);
	//qDebug() << idx;
	QString js_str = QString("set_zoom(%1);").arg(button->property("zoom").toString());
	execute_js(js_str);
}

/*
The whole units of degrees will remain the same (i.e. in 121.135° longitude, start with 121°).
Multiply the decimal by 60 (i.e. .135 * 60 = 8.1).
The whole number becomes the minutes (8').
Take the remaining decimal and multiply by 60. (i.e. .1 * 60 = 6).
The resulting number becomes the seconds (6"). Seconds can remain as a decimal.
Take your three sets of numbers and put them together, using the symbols for degrees (°), minutes (‘), and seconds (") (i.e. 121°8'6" longitude)
*/
QString GMapWidget::to_lat(QVariant lat){
    QStringList latParts = lat.toString().split(".");
	//int deci = latParts[1].toInt();
    //qDebug() << deci;
	//float f = deci * 60;
    //qDebug() << deci << f;
    return latParts[1];
}


//*** Add Marker
void GMapWidget::add_marker(LatLng latLng, QString label){

    QString js_str = QString("add_marker(%1, %2, '%3');")
                     .arg( latLng.lat() ).arg( latLng.lng() ).arg( label );
	//qDebug() << js_str;
    this->execute_js(js_str);

}


void GMapWidget::zoom_to(QString lat, QString lng, QString zoom){
    QString js_str = QString("zoom_to(%1, %2, %3);").arg( lat ).arg( lng ).arg( zoom );
	//qDebug() << js_str;
    this->execute_js(js_str);

}

void GMapWidget::execute_js(QString js_str){
	//qDebug() << "> js= " << js_str;
	webView->page()->mainFrame()->evaluateJavaScript(js_str);
}




//===========================================================================
//== Web View Progress Slots
//===========================================================================
void GMapWidget::start_progress(){
	progressBar->setVisible(true);
}

void GMapWidget::update_progress(int v){
	progressBar->setValue(v);
}
void GMapWidget::end_progress(bool Ok){
	Q_UNUSED(Ok);
	progressBar->setVisible(false);
}

//===========================================================================
//== Initialisaztion
//===========================================================================
void GMapWidget::init_map(){

	static bool map_initialized = false;
	if(map_initialized == false){
		//= Read file if in dev_mode() - no need to "recompile" the resource file
		QFile file(
						mainObject->settings->dev_mode()
						? XSettings::fgx_current_dir().append("/resources/google_map/gmap.html")
						: ":/google_map/gmap.html"
					);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
			qDebug("not open file");
			return;
		}

		webView->page()->mainFrame()->addToJavaScriptWindowObject("Qt", this);
		QByteArray contents = file.readAll();
		//qDebug() << contents;
		webView->setHtml(contents);

	}
	map_initialized = true;
}

//= JS Callback
void GMapWidget::set_map_init(){
	//qDebug() << "JS map init";

}

//=======================================================
void GMapWidget::add_airport(QString airport){
	execute_js( QString("add_airport('%1')").arg(airport) );
}

void GMapWidget::show_airport(QString airport){
	execute_js( QString("show_airport('%1')").arg(airport) );
}

void GMapWidget::add_runway(QString airport, QString rwy1, QString lat1, QString lng1, QString rwy2, QString lat2, QString lng2){
   QString js_str = QString("add_runway('%1','%2', %3, %4, '%5', %6, %7);"
							).arg( airport
							).arg( rwy1 ).arg( lat1 ).arg( lng1
							).arg( rwy2 ).arg( lat2 ).arg( lng2
	);
	execute_js(js_str);
}
