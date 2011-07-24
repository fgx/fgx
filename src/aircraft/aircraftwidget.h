#ifndef AIRCRAFTWIDGET_H
#define AIRCRAFTWIDGET_H

#include <QString>
#include <QStringList>

#include <QtGui/QWidget>
#include <QtGui/QAbstractButton>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <QtGui/QTreeWidget>
#include <QtGui/QTabBar>
#include <QtGui/QLabel>
#include <QtGui/QStatusBar>
#include <QtGui/QToolButton>

#include "xobjects/mainobject.h"

class AircraftWidget : public QWidget
{
Q_OBJECT
public:
	//aero << directory << xml_file << description << fdm << author << file_path;
	enum COLS{
		C_DIR = 0,
		C_AERO,
		C_XML_SET,
		C_DESCRIPTION,
		C_FDM,
		C_AUTHOR,
		C_FILE_PATH
	};
	enum VIEWS{
		LIST_VIEW = 0,
		FOLDER_VIEW = 1
	};

	explicit AircraftWidget(MainObject *mOb, QWidget *parent = 0);

	MainObject *mainObject;



	QButtonGroup *groupUseAircraft;
	QLineEdit *txtAircraftPath;
	QToolButton *buttSelectPath;

	QLabel *aeroImageLabel;
	QCheckBox *checkViewNested;


	QLineEdit *txtNav1;
	QLineEdit *txtNav2;
	QLineEdit *txtAdf1;
	QLineEdit *txtAdf2;
	QLineEdit *txtCom1;
	QLineEdit *txtCom2;
	
	QCheckBox *checkBoxUseDefaultFuel;
	QCheckBox *checkBoxFuelFreeze;
	QLineEdit *txtTank1;
	QLineEdit *txtTank2;
	QLineEdit *txtTank3;

	void initialize();

	QString validate();
	void select_node(QString aero);
	QString selected_aircraft();

private:
	QTreeWidget *treeWidget;
    QStatusBar *statusBarTree;
	QLabel *labelAeroPath;
	QToolButton *buttonAeroPath;

signals:
	void setx( QString option, bool enabled,QString value);

public slots:
	void load_tree();
	void on_set_aircraft();
	void on_select_path();
	void on_tree_selection_changed();
	void on_reload_cache();

	void on_enable_fuel_freeze_clicked();

	void on_navs_changed();
	void on_fuel_changed();

	void on_upx(QString option, bool enabled, QString value);
	void on_open_aircraft_path();
};

#endif // AIRCRAFTWIDGET_H
