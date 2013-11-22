// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // pilotswidget.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-201
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef FLIGHTSWIDGET_H
#define FLIGHTSWIDGET_H


#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QDomDocument>

#include <QWidget>
#include <QTreeWidget>
#include <QStatusBar>
#include <QCheckBox>
#include <QComboBox>


#include "xobjects/mainobject.h"
class MainObject;

#include "utilities/utilities.h"

#include "map/openlayerwidget.h"

#include "aircraft/xaero.h"

class FlightsWidget : public QWidget
{
Q_OBJECT
public:
    explicit FlightsWidget(MainObject *mob, QWidget *parent = 0);



	MainObject *mainObject;

	OpenLayerWidget *mapWidget;
	QDomDocument dom;

	QNetworkAccessManager *netMan;
	QNetworkReply  *reply;
	QString server_string;

	QCheckBox *checkBoxFollowSelected;

	QCheckBox *checkBoxAutoRefresh;
	QComboBox *comboBoxHz;

	QTreeWidget *tree;

	QStatusBar *statusBar;

	QButtonGroup *buttonGroupCols;

	void set_map_widget(OpenLayerWidget *mapWidget);

signals:

	//void radar(QString callsign, QString lat, QString lon, QString heading, QString altitude, bool is_tower);
	//void clear_radar();
	void aircraft_selected(XAero aero);

public slots:

	void fetch_pilots();

	void on_server_error(QNetworkReply::NetworkError);
	void on_server_ready_read();
	void on_server_read_finished();

	void on_check_autorefresh(int);
	void on_combo_changed(int);
	void on_item_doubled_clicked(QTreeWidgetItem*,int);

	void on_show_cols(QAbstractButton*);

};

#endif // FLIGHTSWIDGET_H
