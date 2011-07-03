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

class PilotsWidget : public QWidget
{
Q_OBJECT
public:
	explicit PilotsWidget(MainObject *mob, QWidget *parent = 0);

	enum COLS{
		C_CALLSIGN,
		C_AIRCRAFT,
		C_ALTITUDE,
		C_HEADING,
		C_PITCH,
		C_LAT,
		C_LNG,
		C_FLAG
	};

	MainObject *mainObject;

	QNetworkAccessManager *netMan;
	QNetworkReply  *reply;
	QString server_string;

	QCheckBox *checkBoxAutoRefresh;
	QComboBox *comboBoxHz;

	QTreeWidget *tree;

	QStatusBar *statusBar;



signals:

public slots:

	void fetch_pilots();

	void on_server_error(QNetworkReply::NetworkError);
	void on_server_ready_read();
	void on_server_read_finished();

	void on_check_autorefresh(int);
	void on_combo_changed(int);
};

#endif // PILOTSWIDGET_H
