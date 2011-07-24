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
#include <QtGui/QTabWidget>


#include <QtGui/QStandardItemModel>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QTreeView>
#include <QtGui/QTreeWidget>

#include "xwidgets/xgroupboxes.h"

#include "xobjects/mainobject.h"
class MainObject;

#include "map/openlayerwidget.h"
class OpenLayerWidget;

#include "airports/metarwidget.h"
class MetarWidget;



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
		CI_LON = 7,
		CI_HEADING =8,
		CI_RUNWAYS = 9
		//CI_HDG = 10
	};

	enum STARTUP_POSTITION{
		USE_DEFAULT = 0,
		USE_AIRPORT = 1,
		USE_COORDINATES = 2
	};

	explicit AirportsWidget(MainObject *mOb, QWidget *parent = 0);

	MainObject *mainObject;


	QTabWidget *tabWidget;
	OpenLayerWidget *mapWidget;
	MetarWidget *metarWidget;

	XGroupVBox *groupBoxAirport;
	XGroupVBox *groupBoxUseCoordinates;

	QButtonGroup *buttonGroupFilter;
    QLineEdit *txtAirportsFilter;

	QStandardItemModel *model;
	QSortFilterProxyModel *proxyModel;
	QTreeView *treeViewAirports;

	QPushButton *buttonRefreshTree;
	QStatusBar *statusBarAirports;
	QToolButton *buttonOpenAirportsFolder;
	QLabel *labelAirportsFolder;

	QTreeWidget *treeWidgetAirportInfo;
	QStatusBar *statusBarAirportInfo;


	void scan_airports_xml();
	void show_progress(bool state);

	void load_airports_tree();

	void load_info_tree(QString airport_dir, QString airport_code);
	void load_tower_node(QString airport_dir, QString airport_code);
	int load_runways_node(QString airport_dir, QString airport_code);
	int load_parking_node(QString airport_dir, QString airport_code);


	QLineEdit *txtLat;
	QLineEdit *txtLon;
	QLineEdit *txtVOR;
	QLineEdit *txtNDB;
	QLineEdit *txtFIX;
	QLineEdit *txtOffset;
	QLineEdit *txtAltitude;
	QLineEdit *txtHeading;

	QString validate();

	QString current_airport();


signals:
	
	void setx(QString option, bool enabled, QString value);

public slots:

	void initialize();

	void on_update_airports_filter();

	void on_airport_tree_selected(QModelIndex currentIdx, QModelIndex previousIdx);
	void on_airport_info_selection_changed();

	void on_airport_info_double_clicked(QTreeWidgetItem*,int);
	void on_open_airports_folder();
	void on_show_metar();

	void on_reload_cache();

	//void on_tab_changed();

	void on_coordinates_changed();
	
	void on_upx( QString option, bool enabled, QString value);
	
};

#endif // AIRPORTSWIDGET_H
