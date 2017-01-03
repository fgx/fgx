// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // xreply.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2016
//  Pete Morgan
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef XREPLY_H
#define XREPLY_H

#include <QString>
#include <QScriptValue>

class XReply
{
public:
    explicit XReply();
    QObject *origin;
    QString url;
    bool ok;
    QString raw;
    QScriptValue data;
    bool error;
    QString errorMess;

};

#endif // XREPLY_H
