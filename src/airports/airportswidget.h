#ifndef AIRPORTSWIDGET_H
#define AIRPORTSWIDGET_H

#include <QtCore/QDirIterator>
#include <QtCore/QStringList>
#include <QXmlStreamReader>

#include <QtGui/QWidget>
#include <QtGui/QButtonGroup>
#include <QtGui/QRadioButton>
#include <QtGui/QPushButton>
#include <QtGui/QProgressBar>
#include <QtGui/QStatusBar>
#include <QtGui/QGroupBox>
#include <QtGui/QRadioButton>


#include <QtGui/QStandardItemModel>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QTreeView>
#include <QtGui/QTreeWidget>

#include "xobjects/xsettings.h"

class AirportsWidget : public QWidget
{
Q_OBJECT
public:
    enum COLS{
        C_FAV = 0,
		C_ICAO = 1,
        C_TOWER = 2,
        C_ELEVATION = 3,
		C_NAME = 4,
		C_XML = 5
    };

	explicit AirportsWidget(QWidget *parent = 0);

	XSettings settings;

	QRadioButton *radioButtonUseDefault;
	QRadioButton *radioButtonUseAirport;
	QRadioButton *radioButtonUseCoordinates;

	QGroupBox *groupBoxAirport;
	QGroupBox *groupBoxUseCoordinates;

    QStandardItemModel *model;
    QSortFilterProxyModel *proxyModel;
	QTreeView *treeViewAirports;
	QPushButton *buttonRefreshTree;
	QStatusBar *statusBarAirports;
	QStatusBar *statusBarRunways;
	//QProgressBar *progressAirportsLoad;

    QLineEdit *txtAirportsFilter;
    QButtonGroup *buttGroupFilter;

    QTreeWidget *treeWidgetRunways;

    void show_progress(bool state);

	void scan_airports_xml();
	void load_airports_tree();
	void load_runways(QString airportXmlFile);

	QLineEdit *txtLat;
	QLineEdit *txtLng;
	QLineEdit *txtAltitude;
	QLineEdit *txtHeading;
	QLineEdit *txtRoll;
	QLineEdit *txtPitch;
	QLineEdit *txtAirspeed;

	QStringList get_args();

	void save_settings();
	void load_settings();
	QString validate();



signals:
	void set_arg(QString action, QString arg, QString value);

public slots:

	void initialize();

    void on_update_filter();
	void on_aiport_row_changed(QModelIndex, QModelIndex);

	void on_refresh_clicked();

	void on_buttonGroupUse();
};

#endif // AIRPORTSWIDGET_H
