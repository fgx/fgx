// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // importairportsdialog.cpp
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information


#include <QFileInfo>
#include <QProcess>
#include <QVariant>


#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QRadioButton>
#include <QIcon>
#include <QLabel>
#include <QCheckBox>
#include <QProgressDialog>


#include "airports/importairportsdialog.h"

/* Shows the import airports widget

   TODO: MessageOBx to confirm import and this will take a while
		 Close after import finished
   */

ImportAirportsDialog::ImportAirportsDialog(QWidget *parent) :
	QDialog(parent)
{



    setWindowTitle("Import Airports");
	setWindowIcon(QIcon(":/icon/import"));

    setFixedWidth(400);

    QVBoxLayout *outerContainer = new QVBoxLayout();
    this->setLayout(outerContainer);
    outerContainer->setContentsMargins(0,0,0,0);
    outerContainer->setSpacing(10);

    QVBoxLayout *mainVBox = new QVBoxLayout();
    outerContainer->addLayout(mainVBox);
    int m = 20;
    mainVBox->setContentsMargins(m,m,m,m);
    mainVBox->setSpacing(10);


	QLabel *lblHelp = new QLabel("Importing can take a few seconds !");
    mainVBox->addWidget(lblHelp, 1);
    lblHelp->setStyleSheet("background-color: #efefef; padding: 5px; border: 1px solid #000099;");

    //*****************************************************************
    //** Bottom Button Box
    QHBoxLayout *buttonBox = new QHBoxLayout();
    mainVBox->addLayout(buttonBox);
    buttonBox->addStretch(10);


    //* Cancel Import button
    buttCancelImport = new QPushButton();
    buttonBox->addWidget(buttCancelImport);
	buttCancelImport->setText(tr("Cancel"));
	buttCancelImport->setIcon(QIcon(":/icon/black"));
    connect(buttCancelImport, SIGNAL(clicked()), this, SLOT(reject()));

    //* Import button
    buttImport = new QPushButton();
    buttonBox->addWidget(buttImport);
    buttImport->setText(tr("Import"));
	buttImport->setIcon(QIcon(":/icon/import"));
	connect(buttImport, SIGNAL(clicked()), this, SLOT(accept())  );


}






