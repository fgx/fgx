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
#include <QCheckBox>
#include <QLineEdit>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QSortFilterProxyModel>
#include <QTabBar>
#include <QLabel>
#include <QStatusBar>
#include <QToolButton>

#include "xobjects/mainobject.h"

/*! \class AircraftWidget
* \brief The grid widget panel
*/

class AircraftWidget : public QWidget
{
Q_OBJECT
public:
    //directory << aero << description << fdm << author << xml_file << file_path;
    enum COLS{
        C_DIR,
        C_AERO,
        C_DESCRIPTION,
        C_FDM,
        C_AUTHOR,
        C_XML_FILE,
        C_FILE_PATH,
        C_FILTER
    };
    /*enum VIEWS{
        LIST_VIEW = 1,
        FOLDER_VIEW = 0
    };*/

    explicit AircraftWidget(MainObject *mOb, QWidget *parent = 0);

    MainObject *mainObject;

    QCheckBox *checkBoxUseCustomHangar;

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
    QList<QStandardItem*> create_model_row();


private:
    QStandardItemModel *model;
    QSortFilterProxyModel *proxyModel;
    QTreeView *treeView;
    QLineEdit *txtFilter;

    QStatusBar *statusBarTree;
    QLabel *labelAeroPath;
    QToolButton *buttonAeroPath;

signals:
    void setx( QString option, bool enabled,QString value);

public slots:
    void load_tree();
    void on_select_path();
    void on_custom_hangar_path();
    void on_tree_selection_changed();
    void on_reload_cache();

    void on_enable_fuel_freeze_clicked();

    void on_navs_changed();
    void on_fuel_changed();

    void on_upx(QString option, bool enabled, QString value);
    void on_open_aircraft_path();

    void on_filter_text_changed(const QString);
    void on_clear_filter();

};

#endif // AIRCRAFTWIDGET_H
