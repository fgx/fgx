// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // messagebox.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QMessageBox>

class MessageBox : public QMessageBox
{
Q_OBJECT
public:
    explicit MessageBox(QWidget *parent = 0);

    void showWindowMessage(QString alertMessage);


signals:

public slots:

};

#endif // MESSAGEBOX_H
