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

#include "xobjects/mainobject.h"


class InstallWindow : public QWidget
{
Q_OBJECT
public:
    explicit InstallWindow(MainObject *mob, QWidget *parent = 0);

    MainObject *mainObject;

    QStandardItemModel *model;
    QSortFilterProxyModel *proxyModel;

    QTabWidget *tabWidget;
    QTreeView *treeView;

    QLineEdit *txtSvnCheckoutPath;

    QStatusBar *statusBar;

    //void closeEvent(QCloseEvent *event);
    void moveEvent(QMoveEvent *ev);

signals:

public slots:
    //void load_metar(QString apt);
    void on_init_svn();

};

#endif // INSTALL_WINDOW_H
