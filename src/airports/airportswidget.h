#ifndef AIRPORTSWIDGET_H
#define AIRPORTSWIDGET_H

#include <QtCore/QDirIterator>
#include <QtCore/QStringList>
#include <QXmlStreamReader>

#include <QtGui/QWidget>
#include <QtGui/QStandardItemModel>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QTreeView>
#include <QtGui/QTreeWidget>

#include <QtGui/QButtonGroup>
#include <QtGui/QRadioButton>
#include <QtGui/QProgressBar>
#include <QtGui/QStatusBar>


#include "xobjects/xsettings.h"

class AirportsWidget : public QWidget
{
Q_OBJECT
public:
    enum COLS{
        C_FAV = 0,
        C_CODE = 1,
        C_TOWER = 2,
        C_ELEVATION = 3,
		C_NAME = 4,
		C_XML = 5
    };

	explicit AirportsWidget(QWidget *parent = 0);

	XSettings settings;

    QStandardItemModel *model;
    QSortFilterProxyModel *proxyModel;
    QTreeView *treeView;

	QStatusBar *statusBarTree;
	//QProgressBar *progressAirportsLoad;

    QLineEdit *txtAirportsFilter;
    QButtonGroup *buttGroupFilter;

    QTreeWidget *treeWidgetRunways;

    void show_progress(bool state);
	void initialize();
	void scan_airports_xml();
	void load_tree();

signals:
	void set_arg(QString action, QString arg, QString value);

public slots:
    void import_airports_dialog();

	//void on_airport(QString, QString, QString, QString);
    //void update_airports_count();
    void on_update_filter();
	void on_aiport_selection_changed(const QItemSelection&, const QItemSelection&);
	void on_aiport_row_changed(QModelIndex, QModelIndex);

	void on_refresh_clicked();
};

#endif // AIRPORTSWIDGET_H
