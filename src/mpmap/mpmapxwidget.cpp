

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
MpMapXWidget::MpMapXWidget(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

	mainObject  = mob,

    setWindowTitle(tr("FGx Map"));
	setWindowIcon(QIcon(":/icon/mpmap"));


	//======================================================
	//= Main Layout and Splitter
	QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);


	QSplitter *splitter = new QSplitter();
	mainLayout->addWidget(splitter);


	//======================================================
	//= Map
	QWidget *mapWidget = new QWidget();
	splitter->addWidget(mapWidget);

	QVBoxLayout *mapLayout = new QVBoxLayout();
	mapLayout->setContentsMargins(0,0,0,0);
	mapLayout->setSpacing(0);
	mapWidget->setLayout(mapLayout);

	//=============================
	//= Toolbar
    QToolBar *toolbar = new QToolBar();
	mapLayout->addWidget(toolbar, 0);

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


	//=================================================================
	// WebView
    webView = new QWebView();
	mapLayout->addWidget(webView, 100);

	//=== Register Qt Widget
    webView->page()->mainFrame()->addToJavaScriptWindowObject("QtWidget", this);

	//=============
	//== Network cache /cookies
	networkDiskCache = new QNetworkDiskCache(this);
	networkDiskCache->setCacheDirectory(QDesktopServices::storageLocation(QDesktopServices::CacheLocation));

	networkCookieJar = new QNetworkCookieJar(this);


	//= Net connections
	connect(webView, SIGNAL(loadStarted()), this, SLOT(start_progress()));
	connect(webView, SIGNAL(loadProgress(int)), this, SLOT(update_progress(int)));
	connect(webView, SIGNAL(loadFinished(bool)), this, SLOT(end_progress(bool)));

	//=====================
	//==Status Bar
    statusBar = new QStatusBar();
	mapLayout->addWidget(statusBar);

	//= Progress Bar
	progressBar = new QProgressBar();
	progressBar->setVisible(false);
	statusBar->addPermanentWidget(progressBar);


	//== Zoooomm
    statusBar->addPermanentWidget(new QLabel("Zoom:"));
	groupZoom = new QButtonGroup(this);
    groupZoom->setExclusive(true);
    connect(groupZoom, SIGNAL(triggered(QAction *)),
            this, SLOT(on_zoom_action(QAction *))
    );
    for(int z=1; z < 8; z++){
		QToolButton *act = new QToolButton(this);
        act->setText(QString(" %1 ").arg(z));
        act->setProperty("zoom", QVariant(z));
        act->setCheckable(true);
        //act.setChecked(b[0] == 'Uk');
		statusBar->addPermanentWidget(act);
		groupZoom->addButton(act);
    }


	//== Lat Lng Labels
    statusBar->addPermanentWidget(new QLabel("Lat:"));
    lblLat = new QLabel();
    //lblLat.setStyleSheet(style)
    lblLat->setFixedWidth(140);
    lblLat->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    statusBar->addPermanentWidget(lblLat);

    //QLabel *lblLng = QLabel();
    //toolbar.addAction("Lng:")
    statusBar->addPermanentWidget(new QLabel("Lng:"));
    lblLng = new QLabel();
    //lblLng->setStyleSheet(style);
    lblLng->setFixedWidth(140);
    statusBar->addPermanentWidget(lblLng);



	//================================================================================
	//= Pilots widget
	//================================================================================
	pilotsWidget = new PilotsWidget(mainObject);
	splitter->addWidget(pilotsWidget);




	//====================================================================

	splitter->setStretchFactor(0, 5);
	splitter->setStretchFactor(1, 1);


	//= Read file if in dev_mode() - no need to "recompile" the resource file
	QFile file(	mainObject->settings->dev_mode()
				? XSettings::fgx_current_dir().append("/resources/google_map/gmap.html")
				: ":/gmap/gmap.html"
				);

	//qDebug() << "GOOGLEFILE" << file.fileName();
	//QFile *file = new QFile(":/google_map/gmap.html");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug("not open file");
            return;
       }
	QByteArray contents = file.readAll();
    webView->setHtml(contents);
//        qDebug("OKKK");
}



void MpMapXWidget::map_mouse_move(QVariant dLat, QVariant dLng){
    //qDebug("YES");
    //qDebug() << lat.toString();
    lblLat->setText(dLat.toString());
    lblLng->setText(dLng.toString());
}


void MpMapXWidget::map_error(QVariant err){
    qDebug("map_error()");
}

void MpMapXWidget::marker_clicked(QVariant marker, QVariant mId){
    qDebug("marker_clicked()");
}

void MpMapXWidget::marker_unselected(QVariant curr_idx, QVariant mLocationId){
    qDebug("marker_unselected()");
}

//** JS
void MpMapXWidget::map_right_click(QVariant lat, QVariant lng){

    qDebug("map_right_click()");
}

//** JS
void MpMapXWidget::map_zoom_changed(QVariant zoom){
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

void MpMapXWidget::on_zoom_action(QAction *act){

    QString js_str = QString("set_zoom(%1);").arg(act->property("zoom").toString());
     qDebug() << act->property("zoom").toString() << js_str;
    this->execute_js(js_str);
}

/*
The whole units of degrees will remain the same (i.e. in 121.135° longitude, start with 121°).
Multiply the decimal by 60 (i.e. .135 * 60 = 8.1).
The whole number becomes the minutes (8').
Take the remaining decimal and multiply by 60. (i.e. .1 * 60 = 6).
The resulting number becomes the seconds (6"). Seconds can remain as a decimal.
Take your three sets of numbers and put them together, using the symbols for degrees (°), minutes (‘), and seconds (") (i.e. 121°8'6" longitude)
*/
QString MpMapXWidget::to_lat(QVariant lat){
    QStringList latParts = lat.toString().split(".");
    int deci = latParts[1].toInt();
    //qDebug() << deci;
    float f = deci * 60;
    //qDebug() << deci << f;
    return latParts[1];
}


//*** Add Marker
void MpMapXWidget::add_marker(LatLng latLng, QString label){

    QString js_str = QString("add_marker(%1, %2, '%3');")
                     .arg( latLng.lat() ).arg( latLng.lng() ).arg( label );
    qDebug() << js_str;
    this->execute_js(js_str);

}
//*************************************************************************************************
//*** Add Runway
void MpMapXWidget::add_runway(float lat1, float lng1, float lat2, float lng2, QString label){

    QString js_str = QString("add_runway(%1, %2, %3, %4, '%5');")
                     .arg( lat1 ).arg( lng1 )
                     .arg( lat2 ).arg( lng2 )
                     .arg( label );
    qDebug() << js_str;
    this->execute_js(js_str);
}
void MpMapXWidget::add_runway(QString lat1, QString lng1, QString lat2, QString lng2, QString label){
    add_runway( lat1.toFloat(), lng1.toFloat(), lat2.toFloat(), lng2.toFloat(), label);
}
void MpMapXWidget::add_runway(LatLng p1, LatLng p2, QString label){
    this->add_runway(p1.lat(), p1.lng(), p2.lat(), p2.lng(), label);
}

void MpMapXWidget::zoom_to(QString lat, QString lng, QString zoom){
    QString js_str = QString("zoom_to(%1, %2, %3);").arg( lat ).arg( lng ).arg( zoom );
    qDebug() << js_str;
    this->execute_js(js_str);

}

void MpMapXWidget::execute_js(QString js_str){
    webView->page()->mainFrame()->evaluateJavaScript(js_str);
}




//======================================================
//== Progress Slots
void MpMapXWidget::start_progress(){
	progressBar->setVisible(true);
}

void MpMapXWidget::update_progress(int v){
	progressBar->setValue(v);
}
void MpMapXWidget::end_progress(bool Ok){
	Q_UNUSED(Ok);
	progressBar->setVisible(false);
}
