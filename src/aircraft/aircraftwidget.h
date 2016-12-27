// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // aircraftwidget.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef AIRCRAFTWIDGET_H
#define AIRCRAFTWIDGET_H

#include <QString>
#include <QStringList>

#include <QWidget>
#include <QAbstractButton>
#include <QButtonGroup>
#include <QActionGroup>
#include <QCheckBox>
#include <QLineEdit>
#include <QTreeView>
#include <QStandardItem>
#include <QSortFilterProxyModel>
#include <QTabBar>
#include <QLabel>
#include <QStatusBar>
#include <QToolButton>

#include "xobjects/mainobject.h"
class MainObject;
#include "xwidgets/toolbargroup.h"
class ToolBarGroup;



#include "aircraft/aircraftproxymodel.h"
class AircraftProxyModel;

/*! \class AircraftWidget
* \brief Aircraft panel with a left/right splitter, model/proxy/tree vs selected
* \todo need to abtract out the aircraft custom directories, ie built in and custom
* \todo qt5 need to serialise airarft cache to json
* \todo future is run next vs master vs others and detect versions..
* \todo download aircraft from remote.. WIP at https://github.com/fgx/fgx-installer
*/

class AircraftWidget : public QWidget
{
Q_OBJECT
public:


    explicit AircraftWidget(MainObject *mOb, QWidget *parent = 0);

    MainObject *mainObject;

    ToolBarGroup *grpCustomDirs;

    /* \brief Toggle buttons to show hide aircraft */
    QButtonGroup *buttGroupShowDirs;
    /* \brief Delete dropdown actions */
    QActionGroup *actGroupDeleteCustomDirs;
    /* \brief List of pointers to custon Buttons */
    QList<QToolButton*> lstCustomDirButtons;
    QLineEdit *txtFilter;
    QToolButton * buttShowBase;

    QLabel *lblAero;
    QLabel *lblAeroDescription;
    QLabel *lblAeroXml;
    QLabel *aeroImageLabel;
    QCheckBox *checkViewNested;
    QToolButton *buttOpenAeroDir;


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


    //void load_custom_aircraft();

private:

    AircraftProxyModel  *proxyModel;
    QTreeView *treeView;


    QStatusBar *statusBar;

    QToolButton *buttonAeroPath;

    QSplitter *splitter;

signals:
    void setx( QString option, bool enabled,QString value);

public slots:
    void load_aircraft(bool reload_cache);
    void on_reload_cache();

    void on_tree_selection_changed();


    void on_enable_fuel_freeze_clicked();

    void on_navs_changed();
    void on_fuel_changed();

    void on_upx(QString option, bool enabled, QString value);
    void on_debug_mode();


    void on_open_aircraft_path();
    void on_view_aircraft_cache();

    void on_filter_text_changed(const QString);
    void on_clear_filter();

    void on_splitter_moved();

    void on_add_custom_dir();
    void on_remove_custom_dir(QAction*);
    void load_custom_dir_buttons();
    void on_toggle_directory();


};

#endif // AIRCRAFTWIDGET_H
