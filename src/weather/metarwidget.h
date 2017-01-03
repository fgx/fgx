// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // metarwidget.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef METARWIDGET_H
#define METARWIDGET_H

#include <QWidget>
#include <QPlainTextEdit>


#include "network/netman.h"
class NetManager;

#include "xwidgets/xstatusbar.h"
class XStatusBar;

#include "xobjects/mainobject.h"
class MainObject;




class MetarWidget : public QWidget
{
Q_OBJECT
public:
    explicit MetarWidget(MainObject *mob, QWidget *parent = 0);

    MainObject *mainObject;
    NetManager *netMan;

    QString apt_code;

    QPlainTextEdit *txtMetar;
    QPlainTextEdit *txtJson;
    XStatusBar *statusBar;


    void closeEvent(QCloseEvent *event);
    //void moveEvent(QMoveEvent *ev);

signals:

public slots:
    void show_metar(QString apt_code);
    void load_metar(QString apt_code);
    void fetch_metar();
    void clear_metar();
    void on_reply(XReply xreply);
    void on_refresh();


};

#endif // METARWIDGET_H
