#ifndef AIRCRAFT_INSTALL_WIDGET_H
#define AIRCRAFT_INSTALL_WIDGET_H

#include <QMainWindow>
#include <QWidget>
#include <QTreeView>
#include <QButtonGroup>
#include <QAbstractButton>
#include <QLabel>
#include <QProgressBar>
#include <QAction>
#include <QStackedWidget>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QNetworkAccessManager>

#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>

//#include "xwidgets/xstatusbar.h"
//class XStatusBar;

//#include "installer/servercall.h"

#include "installer/downloadmanagerwidget.h"
class DownloadManagerWidget;

class AircraftInstallWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit AircraftInstallWidget(QMainWindow *parent = 0);

    //hLabels << "Dir" << "Aero" << "Description" << "Author" << "Size" << "Hash" << "Updated" << "Download" << "status";
    enum COLS{
        C_SUB_DIR = 0,
        C_AERO,
        C_DESCRIPTION,
        C_AUTHOR,
        C_ZIP_SIZE,
        C_ZIP_MD5,
        C_LAST_UPDATED,
        C_ACTION,
        C_ACTION_LBL
    };

    QNetworkAccessManager *netMan;

    QStandardItemModel *model;
    QSortFilterProxyModel *proxyModel;

    //QAction *actEdit;
    QTreeView *tree;
    QTabBar *tabBar;

    DownloadManagerWidget *downManWidget;

    //ServerCall *server;
    //XStatusBar *statusBar;
    QButtonGroup *buttGroupInstall;
    QButtonGroup *buttGroupInfo;

    QList<QStandardItem*> create_model_row();

signals:
    void open_aero(int id);

public slots:

    void fetch_server();
    void on_server_data(QScriptValue);

    void on_install_button_clicked(QAbstractButton*);
    void on_info_button_clicked(QAbstractButton*);

    void on_status_update(QString zip_file, QString status);

    void on_request_finished(QNetworkReply *reply);
    //void on_q_count(int count);
    //void on_add();
    //void on_edit();
    //void on_delete();



};






#endif // AIRCRAFT_INSTALL_WIDGET_H
