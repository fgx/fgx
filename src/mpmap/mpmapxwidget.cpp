

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

#include <QtWebKit/QWebFrame>
#include <QtGui/QDesktopServices>

#include "mpmap/mpmapxwidget.h"



MpMapXWidget::MpMapXWidget(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

	mainObject  = mob,

	setWindowTitle(tr("FGx Multiplayer Map"));
	setWindowIcon(QIcon(":/icon/mpmap"));


	setProperty("settings_namespace", QVariant("mpmapxwidget__window"));
	mainObject->settings->restoreWindow(this);


	//======================================================
	//= Main Layout and Splitter
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);


	splitter = new QSplitter();
	mainLayout->addWidget(splitter, 200);


	//======================================================
	//= Map
	mapWidget = new OpenLayerWidget(mainObject);
	splitter->addWidget(mapWidget);
	mapWidget->load_map("radar");


	//================================================================================
	//= Pilots widget
	//================================================================================
	pilotsWidget = new PilotsWidget(mainObject);
	splitter->addWidget(pilotsWidget);
	pilotsWidget->set_map_widget(mapWidget);

	//====================================================================

	splitter->setStretchFactor(0, 2);
	splitter->setStretchFactor(1, 1);
	splitter->restoreState(mainObject->settings->value("mpmapxwidget_splitter").toByteArray());
	connect(splitter, SIGNAL(splitterMoved(int,int)), this, SLOT(on_splitter_moved()));

	/*
	connect(pilotsWidget,	SIGNAL(radar(QString,QString,QString, QString, QString, bool)),
			mapWidget,		SLOT(show_radar(QString,QString,QString, QString, QString, bool)));
	connect(pilotsWidget, SIGNAL(clear_radar()),
			mapWidget, SLOT(clear_radar())
	);

	connect(pilotsWidget,	SIGNAL(aircraft_selected(XAero)),
			this,			SLOT(focus_aero(XAero)));

	*/

}

void MpMapXWidget::initialize(){
	//TODO make this cleaner
	if(pilotsWidget->checkBoxAutoRefresh->isChecked()){
		pilotsWidget->fetch_pilots();
	}
}

//= window close
void MpMapXWidget::closeEvent(QCloseEvent *event){
	mainObject->settings->saveWindow(this);
	event->accept();
}

//void MpMapXWidget::on_freeze_map(bool freeze)
//{
	//mapWidget->webView->setUpdatesEnabled(freeze);
	//qDebug() << "freeze=" << freeze;
//}


void MpMapXWidget::add_airport(QString airport){
	Q_UNUSED(airport);
	//mapWidget->add_airport(airport);
}

void MpMapXWidget::show_airport(QString airport){
	mapWidget->zoom_to_airport(airport);
}
void MpMapXWidget::add_runway(QString airport, QString rwy1, QString rwy2, QString lat1, QString lng1, QString lat2, QString lng2){
	mapWidget->add_runway(airport, rwy1, rwy2, lat1, lng1, lat2, lng2);
}

void MpMapXWidget::focus_aero(XAero aero){
	mapWidget->zoom_to_latlon(aero.lat, aero.lon, 10);
}


void MpMapXWidget::on_splitter_moved()
{
	mainObject->settings->setValue("mpmapxwidget_splitter", splitter->saveState());
}
