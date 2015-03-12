// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // installwindow.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2015
// Pete Morgan
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef INSTALL_WINDOW_H
#define INSTALL_WINDOW_H

#include <QWidget>
#include <QTabWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QStatusBar>
#include <QLineEdit>
#include <QProgressBar>


#include "xobjects/mainobject.h"

#include "svnqt/svnqttypes.h"
#include "svnqt/context.h"
#include "svnqt/client.h"
#include "svnqt/client_commit_parameter.h"
#include "svnqt/revision.h"
#include "svnqt/status.h"
#include "svnqt/targets.h"
#include "svnqt/url.h"
#include "svnqt/wc.h"
#include "svnqt/client_parameter.h"
#include "svnqt/client_update_parameter.h"
#include "svnqt/smart_pointer.h"


class InstallWindow : public QWidget
{
Q_OBJECT
public:
    explicit InstallWindow(MainObject *mob, QWidget *parent = 0);

    MainObject *mainObject;
    //##svn::Client *svnClient;


    QStandardItemModel *model;
    QSortFilterProxyModel *proxyModel;
    QList<QStandardItem*> create_model_row();

    QTabWidget *tabWidget;
    QTreeView *treeView;

    QLineEdit *txtSvnCheckoutPath;

    QStatusBar *statusBar;
    QProgressBar *progressBar;


    //void closeEvent(QCloseEvent *event);
    void moveEvent(QMoveEvent *ev);

signals:

public slots:
    void svn_init();
    void svn_list_aircraft();

};

#endif // INSTALL_WINDOW_H
