#ifndef PILOTSWIDGET_H
#define PILOTSWIDGET_H


#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QDomDocument>

#include <QtGui/QWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>


#include "xobjects/mainobject.h"
class MainObject;

#include "map/openlayerwidget.h"

#include "aircraft/xaero.h"

class PilotsWidget : public QWidget
{
Q_OBJECT
public:
	explicit PilotsWidget(MainObject *mob, QWidget *parent = 0);

	enum COLS{
		C_CALLSIGN = 0,
		C_AIRCRAFT = 1,
		C_ALTITUDE =2,
		C_HEADING = 3,
		C_PITCH = 4,
		C_LAT = 5,
		C_LON = 6,
		C_FLAG = 7,
		C_COUNT = 8
	};

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

#endif // PILOTSWIDGET_H
