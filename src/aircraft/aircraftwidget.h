#ifndef AIRCRAFTWIDGET_H
#define AIRCRAFTWIDGET_H

#include <QString>
#include <QStringList>

#include <QtGui/QWidget>
#include <QtGui/QAbstractButton>
#include <QtGui/QButtonGroup>
#include <QtGui/QLineEdit>
#include <QtGui/QTreeWidget>
#include <QtGui/QTabBar>
#include <QtGui/QLabel>
#include <QtGui/QStatusBar>

#include "xobjects/mainobject.h"
#include "aircraft/aircraftimport.h"

class AircraftWidget : public QWidget
{
Q_OBJECT
public:
	enum COLS{
		C_DIR = 0,
		C_XML = 1,
		C_AERO = 2,
		C_DESCRIPTION = 3,
		C_FDM = 4,
		C_AUTHOR = 5
	};
	enum VIEWS{
		V_LIST = 0,
		V_NESTED = 1
	};

	explicit AircraftWidget(MainObject *mOb, QWidget *parent = 0);

	MainObject *mainObject;

	QLabel *aeroImageLabel;
	QTabBar *tabsView;

	QLineEdit *txtNav1;
	QLineEdit *txtNav2;
	QLineEdit *txtAdf;
	QLineEdit *txtComm1;
	QLineEdit *txtComm2;

	void initialize();
	void save_settings();
	void load_settings();
	QString validate();
	void select_node(QString aero);
	QString selected_aircraft();

	QStringList get_args();
		AircraftImport *aircraftImport;

private:
	QTreeWidget *treeWidget;

    QStatusBar *statusBarTree;
    QStatusBar *statusBarAero;





signals:
	void set_arg(QString action, QString arg, QString value);

public slots:
	void load_tree();

	void on_tree_selection_changed();
	void on_reload_db_cache();
};

#endif // AIRCRAFTWIDGET_H
