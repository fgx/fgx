#ifndef PILOTSWIDGET_H
#define PILOTSWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QStatusBar>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include "xobjects/mainobject.h"

class MainObject;

class PilotsWidget : public QWidget
{
Q_OBJECT
public:
	explicit PilotsWidget(MainObject *mob, QWidget *parent = 0);

	enum COLS{
		C_CALLSIGN,
		C_AERO,
		C_ALTITUDE
	};

	MainObject *mainObject;

	QNetworkAccessManager *netMan;
	QNetworkReply  *reply;
	QString server_string;

	QTreeWidget *tree;

	QStatusBar *statusBar;



signals:

public slots:

	void fetch_pilots();

	void on_server_error(QNetworkReply::NetworkError);
	void on_server_ready_read();
	void on_server_read_finished();
};

#endif // PILOTSWIDGET_H
