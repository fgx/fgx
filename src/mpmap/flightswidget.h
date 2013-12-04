// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // pilotswidget.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-201
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef FLIGHTSWIDGET_H
#define FLIGHTSWIDGET_H


#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QSortFilterProxyModel>
#include <QModelIndex>
#include <QTreeView>



#include <QWidget>

#include <QStatusBar>
#include <QCheckBox>
#include <QComboBox>
#include <QLCDNumber>


#include "xobjects/mainobject.h"
class MainObject;

#include "utilities/utilities.h"
//#include "map/openlayerwidget.h"

#include "aircraft/xaero.h"

#include "mpmap/flightsmodel.h"
class FlightsModel;


/*! \class FlightsWidget
 * \brief The FlightsWidget contains a QTreeView and associated controls to view flights
 *
 *
 * @author: Peter Morgan
 */
class FlightsWidget : public QWidget
{
Q_OBJECT
public:
    explicit FlightsWidget(MainObject *mob, QWidget *parent = 0);

    bool first_run;

	MainObject *mainObject;


	QCheckBox *checkBoxFollowSelected;

	QCheckBox *checkBoxAutoRefresh;
	QComboBox *comboBoxHz;

    QLCDNumber *lcdLag;

    QTreeView *tree;
    QSortFilterProxyModel *proxyModel;

	QStatusBar *statusBar;

	QButtonGroup *buttonGroupCols;


signals:

    void aircraft_selected(XAero);

public slots:


	void on_check_autorefresh(int);
	void on_combo_changed(int);

	void on_show_cols(QAbstractButton*);

    void on_tree_double_clicked(const QModelIndex &);
    void do_update();
};

#endif // FLIGHTSWIDGET_H
