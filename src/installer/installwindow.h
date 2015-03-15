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

#include "install_aircrafttreewidget.h"

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


};

#endif // INSTALL_WINDOW_H
