#ifndef AIRCRAFT_INSTALL_WIDGET_H
#define AIRCRAFT_INSTALL_WIDGET_H

#include <QMainWindow>
#include <QWidget>
#include <QTreeView>
#include <QButtonGroup>
#include <QPushButton>
#include <QAbstractButton>
#include <QLabel>
#include <QProgressBar>
#include <QAction>
#include <QLineEdit>
#include <QStackedWidget>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QNetworkAccessManager>

#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>

//#include "xwidgets/xstatusbar.h"
//class XStatusBar;

#include "xobjects/mainobject.h"
class MainObject;

#include "installer/downloadmanagerwidget.h"
class DownloadManagerWidget;

class AircraftInstallWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit AircraftInstallWidget(MainObject *mob, QMainWindow *parent = 0);

    //hLabels << "Dir" << "Aero" << "Description" << "Author" << "Size" << "Hash" << "Updated"  << "status" << "Filter";
    enum COLS{
        C_SUB_DIR = 0,
        C_AERO,
        C_DESCRIPTION,
        C_AUTHOR,
        C_ZIP_SIZE,
        C_ZIP_MD5,
        C_LAST_UPDATED,
        C_STATUS,
        C_FILTER
    };

    MainObject *mainObject;
    QNetworkAccessManager *netMan;

    QStandardItemModel *model;
    QSortFilterProxyModel *proxyModel;
    QLineEdit *txtFilter;

    QLabel *lblAero;
    QPushButton *buttDownload;

    //QAction *actEdit;
    QTreeView *tree;
    QTabBar *tabBar;

    DownloadManagerWidget *downManWidget;


    QList<QStandardItem*> create_model_row();

signals:
    void open_aero(int id);

public slots:

    void fetch_server();
    void load_data(QScriptValue);

    void on_butt_download_clicked();



    void on_status_update(QString zip_file, QString status);

    void on_request_finished(QNetworkReply *reply);


    void on_filter_text_changed(const QString);
    void on_clear_filter();

    void on_tree_selection_changed(QItemSelection, QItemSelection);

};






#endif // AIRCRAFT_INSTALL_WIDGET_H
