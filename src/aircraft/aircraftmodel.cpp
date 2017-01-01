
#include <QtDebug>
#include <QStringList>
#include <QProgressDialog>
#include <QDomDocument>

#include "aircraft/aircraftmodel.h"


AircraftModel::AircraftModel(MainObject *mOb) :
    QStandardItemModel()
{
    this->mainObject = mOb;

    QStringList labels;
    labels << "Dir" << "Aero" << "Description" << "FDM" << "Authors" << "XML" << "FilePath" << "FilterDir" << "Filter";
    this->setHorizontalHeaderLabels(labels);
}

QString AircraftModel::cacheFileName(){
    return mainObject->data_file("aircraft_cache.txt");
}


bool AircraftModel::cache_exists(){
    return QFile::exists( this->cacheFileName() );
}


QList<QStandardItem*> AircraftModel::create_append_row(){
    QList<QStandardItem*> lst;
    for(int i = 0; i < this->columnCount(); i++){
        lst.append( new QStandardItem() );
    }
    this->appendRow(lst);
    return lst;
}

//=============================================================
/* @brief Load/reload the model */
void AircraftModel::load(bool reload_cache){

    qDebug() << "=========== aircraftModel::load reload="  << reload_cache;



    // check cache exists and load
    /*
    if(reload_cache == false && this->cache_exists() ){

        bool ok = this->read_cache();
        if(ok){
            // cache loaded so get outta here
            qDebug() << "load cache and done";
            //emit dataChanged(this->index(0, 0, QModelIndex()),
            //                 this->index(rowCount() -1 , columnCount() - 1, QModelIndex()));
            //this->data_changed();
            return;
        }
        reload_cache = true;
    }
    */
    QSize size(320,100);
    this->mainObject->progressDialog->resize(size);
    this->mainObject->progressDialog->setMinimumDuration(20);
    this->mainObject->progressDialog->reset();
    this->mainObject->progressDialog->setWindowIcon(QIcon(":/icon/load"));
    this->mainObject->progressDialog->setAutoClose(false);
    this->mainObject->progressDialog->setAutoReset(false);

    if(reload_cache) {

        this->mainObject->progressDialog->setWindowTitle("Scanning aircraft directories");
        this->mainObject->progressDialog->setRange(0, 0);
        this->mainObject->progressDialog->setValue(0);
        this->mainObject->progressDialog->show();


        //= List of model dirs to scan
        QStringList mdirs = this->mainObject->settings->value("custom_aircraft_dirs").toStringList();
        QString aircraft_base_path =  mainObject->X->aircraft_path();
        mdirs.prepend(aircraft_base_path);

        // remove cache file then open
        if (this->cache_exists()) {
            outLog("*** FGx aircrafts/hangar data reload: cache file exists!");
            QFile::remove( this->cacheFileName() );
            outLog("*** FGx aircrafts/hangar data reload: REMOVED Aircraft CACHE FILE");
        }
        //= Open Cache File for write
        QFile cacheFile( this->cacheFileName() );
        if(!cacheFile.open(QIODevice::WriteOnly | QIODevice::Text)){
            qDebug() << "TODO Open error cachce file=";
            outLog("*** FGx aircrafts/hangar : Failed to open cache file");
            return;
        }
        QTextStream out(&cacheFile);

        //= Walk the model dirs
        for(int i = 0; i < mdirs.length(); i++){

            QString adir = mdirs.at(i);
            QFileInfoList xmlSets = AircraftModel::get_xml_set_files(adir, true);
            if(xmlSets.length() == 0){
                continue;
            }

            this->mainObject->progressDialog->setLabelText(adir);
            this->mainObject->progressDialog->setRange(0, xmlSets.length() );

            for(int ix = 0; ix < xmlSets.length(); ix++){

                this->mainObject->progressDialog->setValue(ix);

                ModelInfo mi = AircraftModel::read_model_xml( xmlSets.at(ix).absoluteFilePath() );
                mi.filter_dir = adir;
                if(mi.ok){
                    QStringList cols;
                    cols  << mi.dir << mi.description << mi.fdm << mi.authors <<  mi.full_path << mi.filter_dir;
                    out << cols.join("\t") << "\n";
                } else {
                    qDebug() << " Problem not ok=" << mi.full_path;
                }

                if( this->mainObject->progressDialog->wasCanceled() ){
                    this->mainObject->progressDialog->hide();
                    return;
                }

            }  // xmlSets
        } // mdirs
        cacheFile.close();
    } // reloead_cache == true;

    this->read_cache();
    this->mainObject->progressDialog->hide();

}

bool AircraftModel::read_cache(){

    if( !this->cache_exists() ) {
        return false;
    }
    this->mainObject->progressDialog->setWindowTitle("Loading cache");
    this->mainObject->progressDialog->setLabelText("standby...");
    this->mainObject->progressDialog->setRange(0, 0);
    int c = 0;

    //=== Load Base Package
    QFile dataFile(this->cacheFileName() );
    if (!dataFile.open(QIODevice::ReadOnly | QIODevice::Text)){
           qDebug() << "no aircraft.txt";
           return false;
    }

    QList<QStandardItem*> row;
    QTextStream in(&dataFile);
    QString line = in.readLine();
    c = 0;

    while(!line.isNull()){

        QStringList cols = line.split("\t");
        if (cols.size() <= 2) {
            //TODO - warn discarding line
            line = in.readLine();
            continue;
        }
        QFileInfo fInfo(cols.at(4));
        QString aero = fInfo.fileName();
        aero.chop(8);

        row = this->create_append_row();
        row.at(C_DIR)->setText(cols.at(0));
        row.at(C_DIR)->setIcon(QIcon(":/icon/base_folder"));

        row.at(C_AERO)->setText(aero);
        row.at(C_AERO)->setIcon(QIcon(":/icon/aircraft"));
        QFont f = row.at(C_AERO)->font();
        f.setBold(true);
        row.at(C_AERO)->setFont(f);

        row.at(C_DESCRIPTION)->setText(cols.at(1));
        row.at(C_FDM)->setText(cols.at(2));
        row.at(C_AUTHORS)->setText(cols.at(3));

        row.at(C_XML_FILE)->setText(fInfo.fileName());
        row.at(C_FILE_PATH)->setText(cols.at(4));
        row.at(C_FILTER_DIR)->setText(cols.at(5));

        QString filter_str = aero.append(cols.at(2));
        row.at(C_FILTER)->setText( filter_str );

        c++;
        line = in.readLine();

    }

    return true;
}



/* \brief Returns all -set.xml files in a directory. Recus is for custom folder and selecting a parent */
QFileInfoList AircraftModel::get_xml_set_files(QString dir_path, bool recus){
    QDir dir( dir_path );
    QStringList filters;
    filters << "*-set.xml";
    QFileInfoList setList =  dir.entryInfoList(filters);
    if(recus){
        QFileInfoList allentries = dir.entryInfoList();
        for(int i = 0; i < allentries.length(); i++){
            if(allentries.at(i).isDir()){
                QFileInfoList subset = AircraftModel::get_xml_set_files(allentries.at(i).absoluteFilePath(), false);
                for(int ii = 0; ii < subset.length(); ii++){
                    setList.append(subset.at(ii));
                }
            }
        }
    }
    return setList;
}

/* \brief Parses the xml-set file by querying DOM..argh!!! */
ModelInfo AircraftModel::read_model_xml(QString xml_set_path){

    QFileInfo fInfo(xml_set_path);

    ModelInfo mi;
    mi.ok = false;
    mi.full_path = xml_set_path;
    mi.dir = fInfo.dir().dirName();

    QFile xmlFile( xml_set_path );
    if (xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)){

        /* Previously, the filewas read to UTF-8 string, left for ref >>
         * The file content is converted to UTF-8.
             Some files are Windows, encoding and throw error with QxmlQuery etc
             Its a hack and don't quite understand whats happening.. said pedro
        */
        //QString xmlString = QString( xmlFile.readAll() ).toUtf8();

        QDomDocument dom;
        QString errMsg;
        int lineNo;
        int colNo;

        bool loaded = dom.setContent(&xmlFile, false, &errMsg, &lineNo, &colNo);
        if (!loaded){
            qDebug() << "\t> not loaded " << lineNo << "=" << colNo << " " << errMsg;
            return mi;
        }

        QDomNodeList nodes = dom.elementsByTagName("sim");
        if(nodes.count() == 0) {
            return mi;
        }

        QDomNode n = nodes.at(0);
        mi.description = n.firstChildElement("description").text().simplified();
        mi.authors = n.firstChildElement("author").text().simplified();
        mi.fdm = n.firstChildElement("flight-model").text().simplified();
        if (mi.fdm == "null"){
            mi.fdm = "";
        }
        mi.ok = true;

        xmlFile.close();

    } // xmlFile.open()

    return mi;
}

