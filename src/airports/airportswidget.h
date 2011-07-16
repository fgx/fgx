#ifndef AIRPORTSWIDGET_H
#define AIRPORTSWIDGET_H

#include <QtCore/QDirIterator>
#include <QtCore/QStringList>
#include <QtCore/QHash>

#include <QtGui/QWidget>
#include <QtGui/QButtonGroup>
#include <QtGui/QRadioButton>
#include <QtGui/QPushButton>
#include <QtGui/QProgressBar>
#include <QtGui/QStatusBar>
#include <QtGui/QRadioButton>
#include <QtGui/QToolButton>


#include <QtGui/QStandardItemModel>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QTreeView>
#include <QtGui/QTreeWidget>

#include "xwidgets/xgroupboxes.h"
#include "xobjects/mainobject.h"

class AirportsWidget : public QWidget
{
Q_OBJECT
public:
	enum AIPORTS_TREE_COLS{
		CA_CODE = 0,
		CA_NAME = 1,
		CA_DIR = 2
    };
	enum AIRPORT_INFO_TREE_COLS{
		CI_NODE = 0,
		CI_LABEL = 1,
		CI_TYPE = 2,
		CI_SETTING_KEY = 3,
		CI_WIDTH = 4,
		CI_LENGTH  = 5,
		CI_LAT = 6,
		CI_LNG = 7,
		CI_ALIGNMNET =8,
		CI_RUNWAYS = 9
	};

	enum STARTUP_POSTITION{
		USE_DEFAULT = 0,
		USE_AIRPORT = 1,
		USE_COORDINATES = 2
	};

	explicit AirportsWidget(MainObject *mOb, QWidget *parent = 0);

	MainObject *mainObject;

	QButtonGroup *buttonGroupUse;

	XGroupVBox *groupBoxAirport;
	XGroupVBox *groupBoxUseCoordinates;

	QButtonGroup *buttonGroupFilter;
    QLineEdit *txtAirportsFilter;

	QStandardItemModel *model;
	QSortFilterProxyModel *proxyModel;
	QTreeView *treeViewAirports;

	QPushButton *buttonRefreshTree;
	QStatusBar *statusBarAirports;

	QTreeWidget *treeWidgetAirportInfo;
	QStatusBar *statusBarAirportInfo;

	QToolButton *buttonViewMap;

	void scan_airports_xml();
	void show_progress(bool state);

	void load_airports_tree();

	void load_info_tree(QString airport_dir, QString airport_code);
	int load_runways_node(QString airport_dir, QString airport_code);
	int load_parking_node(QString airport_dir, QString airport_code);

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

	QString current_airport();


signals:
	void setx(QString option, bool enabled, QString value);

public slots:

	void initialize();

	void on_update_airports_filter();

	void on_airport_tree_selected(QModelIndex currentIdx, QModelIndex previousIdx);

	void on_reload_cache();


	void on_buttonGroupUse();

	void on_view_map();

	void on_upx( QString option, bool enabled, QString value);
};

#endif // AIRPORTSWIDGET_H
