#ifndef AIRCRAFTWIDGET_H
#define AIRCRAFTWIDGET_H

#include <QString>
#include <QStringList>

#include <QtGui/QWidget>
#include <QtGui/QAbstractButton>
#include <QtGui/QButtonGroup>
#include <QtGui/QLineEdit>
#include <QtGui/QTreeWidget>
#include <QtGui/QLabel>
#include <QtGui/QStatusBar>

#include "xobjects/mainobject.h"


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

	QStringList aeroList;

	QLabel *aeroImageLabel;
	QButtonGroup *buttViewGroup;

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
	QString aircraft();

	QStringList get_args();

private:
	QTreeWidget *treeWidget;

    QStatusBar *statusBarTree;
    QStatusBar *statusBarAero;





signals:
	void set_arg(QString action, QString arg, QString value);

public slots:
	void load_tree();
	void load_aircraft_shell();
	//QStringList scan_xml_sets();

	void on_tree_selection_changed();

    void on_view_button_clicked(QAbstractButton *button);
	void on_refresh_cache();
};

#endif // AIRCRAFTWIDGET_H
