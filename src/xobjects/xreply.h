// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // xopt.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2016
//  Pete Morgan
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef XREPLY_H
#define XREPLY_H

#include <QString>

class XReply
{
public:
    explicit XReply();
    QString origin;
    QString url;
    bool ok;
    QString data;

};

#endif // XREPLY_H
