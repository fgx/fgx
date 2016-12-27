
#include <QtDebug>
#include <QStringList>
#include <QProgressDialog>

#include "aircraftmodel.h"
#include "aircraft/aircraftdata.h"

AircraftModel::AircraftModel(MainObject *mOb) :
    QStandardItemModel()
{

    this->mainObject = mOb;

    QStringList hLabels;
    hLabels << "Dir" << "Aero" << "Description" << "FDM" << "Authors" << "XML" << "FilePath" << "FilterDir" << "Filter" << "BASE";
    //model->setColumnCount(hLabels.length());
    this->setHorizontalHeaderLabels(hLabels);


}


/* @brief Load Custom Aircraft Paths */
void AircraftModel::load_custom_aircraft(){

    this->mainObject->progressDialog->setWindowTitle("Importing Custom Aicraft");

    QList<QStandardItem*> row;

    // Get paths as lsit from settings
    QStringList custom_dirs = this->mainObject->settings->value("custom_aircraft_dirs").toStringList();

    for(int i = 0; i < custom_dirs.size(); i++){

        // get the xml-sets in dir, and recus eg might be a single aircraft or a dir of aircraft
        QFileInfoList xmlSets = AircraftData::get_xml_set_files(custom_dirs.at(i), true);
        //qDebug() << xmlSets;
        this->mainObject->progressDialog->setRange(0, xmlSets.length());

        for(int fi = 0; fi < xmlSets.length(); fi++){


            // get data from the model file
            ModelInfo mi = AircraftData::read_model_xml(xmlSets.at(fi).absoluteFilePath());

            this->mainObject->progressDialog->setValue(fi);
            this->mainObject->progressDialog->setLabelText(mi.aero);

            // Add model row
            row = this->create_model_row();
            row.at(C_DIR)->setText(mi.dir);
            row.at(C_DIR)->setIcon(QIcon(":/icon/custom_folder"));

            row.at(C_AERO)->setText(mi.aero);
            row.at(C_AERO)->setIcon(QIcon(":/icon/aircraft"));
            QFont f = row.at(C_AERO)->font();
            f.setBold(true);
            row.at(C_AERO)->setFont(f);

            row.at(C_DESCRIPTION)->setText(mi.description);
            row.at(C_FDM)->setText(mi.fdm);
            row.at(C_AUTHOR)->setText(mi.authors);
            row.at(C_XML_FILE)->setText(mi.xml_file);
            row.at(C_FILE_PATH)->setText(mi.file_path);
            row.at(C_FILTER_PATH)->setText(custom_dirs.at(i));
            row.at(C_BASE)->setText("0");

            QString filter_str = mi.aero; // filter is the aero+description
            filter_str.append( mi.description );
            row.at(C_FILTER)->setText( filter_str );
        }
    }
}



//=============================================================
/* @brief Load/reload the model */
void AircraftModel::load_aircraft(bool  reload_cache){


    QSize size(320,100);
    this->mainObject->progressDialog->resize(size);
    this->mainObject->progressDialog->setWindowIcon(QIcon(":/icon/load"));
    this->mainObject->progressDialog->show();

    if(reload_cache) {
        bool cancelled = AircraftData::import(this->mainObject->progressDialog, mainObject);
        if(cancelled){
            this->mainObject->progressDialog->hide();
            return;
        }
    }

    int c = 0;

    QList<QStandardItem*> row;

    this->removeRows(0, this->rowCount());

    this->load_custom_aircraft();

    //=== Load Base Package
    QFile dataFile(mainObject->data_file(("aircraft.txt")));
    if (!dataFile.open(QIODevice::ReadOnly | QIODevice::Text)){
           qDebug() << "no aircraft.txt";
           return;
    }
    QTextStream in(&dataFile);
    QString line = in.readLine();
    c = 0;

    while(!line.isNull()){

        QStringList cols = line.split("\t");
        if (cols.size() <= C_FILTER_PATH) {
            //TODO - warn discarding line
            line = in.readLine();
            continue;
        }
        qDebug() << line;
        // Add model row
        row = this->create_model_row();
        row.at(C_DIR)->setText(cols.at(C_DIR));
        row.at(C_DIR)->setIcon(QIcon(":/icon/base_folder"));

        row.at(C_AERO)->setText(cols.at(C_AERO));
        row.at(C_AERO)->setIcon(QIcon(":/icon/aircraft"));
        QFont f = row.at(C_AERO)->font();
        f.setBold(true);
        row.at(C_AERO)->setFont(f);

        row.at(C_DESCRIPTION)->setText(cols.at(C_DESCRIPTION));
        row.at(C_FDM)->setText(cols.at(C_FDM));
        row.at(C_AUTHOR)->setText(cols.at(C_AUTHOR));
        row.at(C_XML_FILE)->setText(cols.at(C_XML_FILE));
        row.at(C_FILE_PATH)->setText(cols.at(C_FILE_PATH));
        row.at(C_FILTER_PATH)->setText(cols.at(C_FILTER_PATH));
        //row.at(C_FILTER_PATH)->setText(mainObject->X->aircraft_path());

        QString filter_str = cols.at(C_AERO);
        filter_str.append(cols.at(C_DESCRIPTION) );
        row.at(C_FILTER)->setText( filter_str );
        row.at(C_BASE)->setText("1");
        c++;
        line = in.readLine();
    }
    this->mainObject->progressDialog->hide();
}



QList<QStandardItem*> AircraftModel::create_model_row(){
    QList<QStandardItem*> lst;
    for(int i = 0; i < this->columnCount(); i++){
        lst.append( new QStandardItem() );
    }
    this->appendRow(lst);
    return lst;
}
