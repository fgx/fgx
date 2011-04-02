
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


#include "airports/importairportswidget.h"
#include "aptdatparser.h"

/* Shows the import airports widget

   TODO: MessageOBx to confirm import and this will take a while
		 Close after import finished
   */

ImportAirportsWidget::ImportAirportsWidget(QDialog *parent) :
    QDialog(parent)
{


	setProperty("settings_namespace", QVariant("import_airports_window"));
	settings.restoreWindow(this);

    setWindowTitle("Import Airports");
	setWindowIcon(QIcon(":/icon/import"));
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


    QLabel *lblHelp = new QLabel("<ul><li>Select the airports to import</li><li>You can run this more than once</li></ul>");
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
    buttAllAiports->setDisabled(true);

    //** Seaports
    chkImportSeaports = new QCheckBox();
    chkImportSeaports->setText(tr("Import Seaports"));
    mainVBox->addWidget(chkImportSeaports, 1);
    chkImportSeaports->setDisabled(true);

    //** Heliports
    chkImportHeliports = new QCheckBox();
    chkImportHeliports->setText(tr("Import Heliports"));
    mainVBox->addWidget(chkImportHeliports, 1);
    chkImportHeliports->setDisabled(true);

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
    connect(buttImport, SIGNAL(clicked()),
            this, SLOT(on_import_button_clicked())
            );

	statusBar = new QStatusBar();
    outerContainer->addWidget(statusBar);

}



void ImportAirportsWidget::update_progress(int value){
    progress->setValue(value);
    progress->setLabelText(QString::number(value));
}

//*********************************************************************
//** Import Clicked
void ImportAirportsWidget::on_import_button_clicked(){

	//* TODO message you are about to do this and take a few moment etc (or background)
	if(1 == 0){
		/*
		QString pat_dat_file = settings.fg_root("/Airports/apt.dat.gz");
		QFileInfo fileInfo = QFileInfo(pat_dat_file);
		if(!fileInfo.exists()){
			statusBar->showMessage(QString("Cannot open '%1'").arg(pat_dat_file));
			return;
		}
		QFileInfo unpacked_file = settings.temp_dir("/apt.dat-unpacked.txt");


		statusBar->showMessage(QString("Unpacking tarball apt.dat.gz to temp dir") );
		// Unzip to temp file
		//TODO but does it work on WindoZE ?? need embedded zlib or is it in Qt ??
		QString command = QString("zcat %1").arg(unpacked_file);

		QProcess unzipProcess;
		unzipProcess.setStandardOutputFile(temp_file);
		unzipProcess.start(command);
		if (!unzipProcess.waitForStarted())
			return; // false;
		if (!unzipProcess.waitForFinished())
			return; // false;
		QByteArray result = unzipProcess.readAll();
		*/
	}
	QString fName = settings.fg_root("/Airports/apt.dat");
	statusBar->showMessage( QString("Importing '%1'").arg(fName) );

    AptDatParser *aptDatParser = new AptDatParser(this);
	aptDatParser->import_aptdat(fName, this);

}




void ImportAirportsWidget::closeEvent(QCloseEvent *event){
	Q_UNUSED(event);
	settings.saveWindow(this);
}
