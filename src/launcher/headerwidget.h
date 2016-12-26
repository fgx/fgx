// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // headerwidget.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef XHEADERWIDGET_H
#define XHEADERWIDGET_H

#include <QTimer>


#include <QWidget>
#include <QLabel>
#include <QStatusBar>


#include "xobjects/mainobject.h"
class MainObject;


class HeaderWidget : public QWidget
{
Q_OBJECT
public:
    explicit HeaderWidget(MainObject *mob, QWidget *parent = 0);

    MainObject *mainObject;

    //QTimer *fadeTimer;

    //QWidget *headerWidget;
    QLabel *headerLabel;
    QLabel *callsignLabel;

    //QLabel *dtUTC;
    //QLabel *dtLocal;

    //QWidget *popWidget;
    QStatusBar *statusBar;

    void setHeader(QString text);
    void setCallsign(QString text);



signals:

public slots:

    void showMessage(QString message, int timeout);
    void showMessage(QString message);
    void cancel_message();

    //void update_time_labels();


};

#endif // XHEADERWIDGET_H
