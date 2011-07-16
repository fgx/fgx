
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>
#include <QtCore/QVariant>


#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QWidget>
#include <QtGui/QRadioButton>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QProgressDialog>


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
	//setWindowModality(Qt::WindowModal);
    //setWindowFlags(  Qt::WindowStaysOnTopHint);

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


	QLabel *lblHelp = new QLabel("<ul><li>Select the airport type to import</li><li>Importing can take a few minutes</li></ul>");
    mainVBox->addWidget(lblHelp, 1);
    lblHelp->setStyleSheet("background-color: #efefef; padding: 5px; border: 1px solid #000099;");

    //** IACO Only
    buttIcaoOnly = new QRadioButton();
    mainVBox->addWidget(buttIcaoOnly, 1);
    buttIcaoOnly->setText("Import ICAO airports only");
    buttIcaoOnly->setChecked(true);

    //** All Airports
    buttAllAiports = new QRadioButton();
    mainVBox->addWidget(buttAllAiports, 1);
    buttAllAiports->setText("Import all airports.");

	//** Seaports - hidden for now
    chkImportSeaports = new QCheckBox();
    chkImportSeaports->setText(tr("Import Seaports"));
    mainVBox->addWidget(chkImportSeaports, 1);
    chkImportSeaports->setDisabled(true);
	chkImportSeaports->hide();

	//** Heliports - hidden for now
    chkImportHeliports = new QCheckBox();
    chkImportHeliports->setText(tr("Import Heliports"));
    mainVBox->addWidget(chkImportHeliports, 1);
    chkImportHeliports->setDisabled(true);
	chkImportHeliports->hide();

    //= Airports from additional scenery paths
    chkBoxAdditionalAirports = new QCheckBox("Import from additional scenery paths.");
    // ***TBD*** Set up a checkbox choice to include all scenery paths, and keep in settings
    //mainVBox->addWidget(chkBoxAdditionalAirports, 1);
    //connect(checkBoxAdditionalAirports, SIGNAL(clicked()), this, SLOT(on_additional_airports()));

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
    //buttCancelImport->setDisabled(true);

    //* Import button
    buttImport = new QPushButton();
    buttonBox->addWidget(buttImport);
    buttImport->setText(tr("Import"));
	buttImport->setIcon(QIcon(":/icon/import"));
	connect(buttImport, SIGNAL(clicked()), this, SLOT(accept())  );


}






