#ifndef MAPCOREWIDGET_H
#define MAPCOREWIDGET_H

#include <QtCore/QVariant>
#include <QtCore/QString>

#include <QtWebKit/QWebView>
#include <QWebInspector>
#include <QtNetwork/QNetworkDiskCache>


#include <QtGui/QStatusBar>
#include <QtGui/QProgressBar>
#include <QtGui/QLabel>
#include <QtGui/QWidget>
#include <QtGui/QActionGroup>
#include <QtGui/QToolButton>
#include <QtGui/QSlider>
#include <QtGui/QDoubleSpinBox>
#include <QToolBar>

#include "xobjects/mainobject.h"
class MainObject;

#include "map/xmarblewidget.h"



QT_FORWARD_DECLARE_CLASS(QNetworkDiskCache)



class MapCoreWidget : public QWidget
{
Q_OBJECT
public:
	explicit MapCoreWidget(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;


	QString *mpmapFollowCallsign;
	QSlider *sliderZoom;
	QLabel *lblZoom;

	QToolBar *toolbarAirports;

	XMarbleWidget *marbleWidget;

private:

	QProgressBar *progressBar;
	QStatusBar *statusBar;
	QComboBox *comboServer;

	QLineEdit *txtLat;
	QLineEdit *txtLon;

	QButtonGroup *buttonGroupViewLayers;
	QCheckBox *chkViewStands;
	QCheckBox *chkViewRunwayLabels;
	QCheckBox *chkViewRunwayLines;

	QCheckBox *chkViewGridLines;


	QDoubleSpinBox *spinHeading;


	QToolButton *buttDebug;


signals:

	void setx(QString option, bool enabled, QString value);

public slots:




	void on_slider_zoom(int zoom);
	void on_butt_zoom_in();
	void on_butt_zoom_out();
	void on_map_zoom_changed(int);


	void refresh_map();
	void map_focus(QString lon, QString lat);

	//======================================
	void closeEvent(QCloseEvent *event);



};

#endif // MAPCOREWIDGET_H
