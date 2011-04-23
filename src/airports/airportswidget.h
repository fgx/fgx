#ifndef AIRPORTSWIDGET_H
#define AIRPORTSWIDGET_H

#include <QtCore/QDirIterator>
#include <QtCore/QStringList>


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

#include "xobjects/mainobject.h"

class AirportsWidget : public QWidget
{
Q_OBJECT
public:
	enum AIPORTS_TREE_COLS{
		CA_CODE = 1,
		CA_NAME
    };
	enum AIRPORT_INFO_TREE_COLS{
		CI_NODE = 0,
		CI_LABEL,
		CI_KEY
	};

	enum STARTUP_POSTITION{
		USE_DEFAULT = 0,
		USE_AIRPORT = 1,
		USE_COORDINATES = 2
	};

	explicit AirportsWidget(MainObject *mOb, QWidget *parent = 0);

	MainObject *mainObject;

	QButtonGroup *buttonGroupUse;

	QGroupBox *groupBoxAirport;
	QGroupBox *groupBoxUseCoordinates;

	QButtonGroup *buttonGroupFilter;
    QLineEdit *txtAirportsFilter;

	QStandardItemModel *model;
	QSortFilterProxyModel *proxyModel;
	QTreeView *treeViewAirports;

	QPushButton *buttonRefreshTree;
	QStatusBar *statusBarAirports;

	QTreeWidget *treeWidgetAirportInfo;
	QStatusBar *statusBarAirportInfo;


	void scan_airports_xml();
	void show_progress(bool state);

	void load_airports_tree();

	void on_airport_selected(QString);
	int load_runways_node(QString airport_code);
	int load_parking_node(QString airport_code);

	QLineEdit *txtLat;
	QLineEdit *txtLng;
	QLineEdit *txtAltitude;
	QLineEdit *txtHeading;
	QLineEdit *txtRoll;
	QLineEdit *txtPitch;
	QLineEdit *txtAirspeed;

	void save_settings();
	void load_settings();
	QString validate();
	QStringList get_args();



signals:
	void set_arg(QString action, QString arg, QString value);

public slots:

	void initialize();

	void on_filter_button(QAbstractButton*);
	void on_filter_airports(QString);
	void on_airport_selected(QModelIndex, QModelIndex);

	void on_import_aptdat_clicked();
	void on_rescan_xml_clicked();

	void on_buttonGroupUse();
};

#endif // AIRPORTSWIDGET_H
