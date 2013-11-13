// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // importairportsdialog.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef IMPORTAIRPORTSWIDGET_H
#define IMPORTAIRPORTSWIDGET_H


#include <QDialog>
#include <QRadioButton>
#include <QCheckBox>
#include <QPushButton>
#include <QProgressDialog>

#include "xobjects/xsettings.h"


class ImportAirportsDialog : public QDialog
{
Q_OBJECT
public:
	explicit ImportAirportsDialog(QWidget *parent = 0);

    QPushButton *buttImport;
    QPushButton *buttCancelImport;

signals:

public slots:

};

#endif // IMPORTAIRPORTSWIDGET_H
