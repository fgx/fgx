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


	QLabel *aeroImageLabel;
	QTabBar *tabsView;
	QCheckBox *checkBoxUseDefault;

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

private:
	QTreeWidget *treeWidget;

    QStatusBar *statusBarTree;
    QStatusBar *statusBarAero;





signals:
	void set_arg(QString action, QString arg, QString value);

public slots:
	void load_tree();
	void on_use_default_clicked();
	void on_tree_selection_changed();
	void on_reload_db_cache();

};

#endif // AIRCRAFTWIDGET_H
