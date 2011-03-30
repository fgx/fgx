#ifndef AIRCRAFTWIDGET_H
#define AIRCRAFTWIDGET_H

#include <QString>
#include <QStringList>

#include <QtGui/QWidget>
#include <QtGui/QAbstractButton>

#include <QtGui/QTreeWidget>
#include <QtGui/QLabel>

#include <QtGui/QStatusBar>

#include "xobjects/xsettings.h"

class AircraftWidget : public QWidget
{
Q_OBJECT
public:
	enum COLS{
		C_DIR = 0,
		C_XML = 1,
		C_AERO = 2,
		C_FDM = 3,
		C_DESCRIPTION = 4,
		C_AUTHOR = 5

	};


	explicit AircraftWidget(QWidget *parent = 0);

	XSettings settings;

	QLabel *aeroImageLabel;

private:
	QTreeWidget *treeWidget;

    QLabel *lblAircraftModel;
	QLabel *lblAuthor;
	QLabel *lblFdm;

    QStatusBar *statusBarTree;
    QStatusBar *statusBarAero;

	void initialize();
	void save_settings();
	void load_settings();
	bool validate();

signals:
	void set_arg(QString action, QString arg, QString value);

public slots:
	void load_aircraft();
	void load_aircraft_shell();
	QStringList aircraft_xml_set();

	void on_tree_selection_changed();
	void on_auto_coordination(bool state);

    void on_view_button_clicked(QAbstractButton *button);
};

#endif // AIRCRAFTWIDGET_H
