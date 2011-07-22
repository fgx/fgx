#ifndef PILOTSWIDGET_H
#define PILOTSWIDGET_H

#include <QtCore/QTimer>


#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>



#include <QtGui/QWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>


#include "xobjects/mainobject.h"
class MainObject;

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
		C_FLAG = 7
	};

	MainObject *mainObject;

	//QTimer *timer;

	QNetworkAccessManager *netMan;
	QNetworkReply  *reply;
	QString server_string;

	QCheckBox *checkBoxAutoRefresh;
	QComboBox *comboBoxHz;

	QTreeWidget *tree;

	QStatusBar *statusBar;

	QButtonGroup *buttonGroupCols;

signals:

	void radar(QString callsign, QString lat, QString lon, QString heading, QString altitude);
	//void aircraft_selected(QString callsign);
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
