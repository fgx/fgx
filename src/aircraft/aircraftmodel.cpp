
#include <QtDebug>
#include <QStringList>
#include <QProgressDialog>
#include <QXmlQuery>

#include "aircraft/aircraftmodel.h"


AircraftModel::AircraftModel(MainObject *mOb) :
    QAbstractItemModel()
{

    this->mainObject = mOb;


    //QStringList headerLabels;
     headerLabels << "Dir" << "Aero" << "Description" << "FDM" << "Authors" << "XML" << "FilePath" << "FilterDir" << "Filter";
    //model->setColumnCount(hLabels.length());
    //this->setHorizontalHeaderLabels(hLabels);
}

QString AircraftModel::cacheFileName(){
    return mainObject->data_file("aircraft_cache.txt");
}

QModelIndex AircraftModel::index(int row, int column, const QModelIndex &parent) const {
    //qDebug() << "index()" << row << "=" << column << parent;
    return this->createIndex(row, column);
}

int AircraftModel::rowCount(const QModelIndex &parent) const {
    //qDebug() << "rowCount()" << parent << this->modelInfoList.length();
    return this->modelInfoList.length();
}
int AircraftModel::rowCount(){
    //qDebug() << "rowCount()" << this->modelInfoList.length();
    return this->modelInfoList.length();
}

int AircraftModel::columnCount(const QModelIndex &idx) const {
    //qDebug() << "columnCount()" << idx << this->headerLabels.length();
    return this->headerLabels.length();
}

QModelIndex AircraftModel::parent(const QModelIndex &child) const {
    //qDebug() << "parent()" << child;
    return QModelIndex();
}

QVariant AircraftModel::data(const QModelIndex &index, int role) const {

    if (!index.isValid()){
        return QVariant();
    }

    if (role == Qt::DecorationRole){
        // Return an icon for col
        switch(index.column()){
            case C_AERO:
                return QIcon(":/icon/aircraft");
            case C_DIR:
                return QIcon(":/icon/base_folder");
        }
        //qDebug() << "data() DecorationRole" << index << " " << role;
        return QVariant();
    }

    //if (role == Qt::FontRole){
    //    // TODO .. the bold
    //}

    if (role != Qt::DisplayRole)
        //qDebug() << "data()" << index << " " << role;
        return QVariant();

    ModelInfo mi = this->modelInfoList.at(index.row());
    switch(index.column()){
        case C_AERO:
            return mi.aero();

        case C_AUTHORS:
            return mi.authors;


        case C_DESCRIPTION:
            return mi.description;

        case C_DIR:
            return mi.dir;

        case C_FDM:
            return mi.fdm;

        case C_FILE_PATH:
            return mi.full_path;

        case C_FILTER_DIR:
            return mi.filter_dir;

        case C_XML_FILE:
            return mi.xml_file();

        case C_FILTER:
            return mi.filter();

    }
    return QVariant("OOOPS");

}

QVariant AircraftModel::headerData(int section, Qt::Orientation orientation, int role) const {
    //qDebug() << "headerData" << section << " " << orientation << " " << role;
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return this->headerLabels.at(section);
    }
    return QVariant();
}

Qt::ItemFlags AircraftModel::flags(const QModelIndex &index) const {
    //qDebug() << "flags()" << index << QAbstractItemModel::flags(index) ;
    return QAbstractItemModel::flags(index);
}







bool AircraftModel::scan_dir(QString dir){

    qDebug() << "##Scan = ", dir;
    QFileInfoList xmlSets = AircraftModel::get_xml_set_files(dir, true);

    this->mainObject->progressDialog->setRange(0, xmlSets.length());

    for(int fi = 0; fi < xmlSets.length(); fi++){
        // get data from the model file
        ModelInfo mi = AircraftModel::read_model_xml(xmlSets.at(fi).absoluteFilePath());
        mi.filter_dir = dir;
        if(mi.dir == "KC135"){
            qDebug() << " @@@@@ xmlfile=" << mi.full_path << mi.aero() << mi.description << mi.authors;
        }
        if(mi.ok){
            this->modelInfoList.append(mi);
        } else {
            qDebug() << " Problem not ok=" << mi.full_path;
        }
        this->mainObject->progressDialog->setValue(fi);
        this->mainObject->progressDialog->setLabelText( dir + "\n" + mi.aero());
        if(this->mainObject->progressDialog->wasCanceled()){
            return true;
        }
    }
    return false;
}

bool AircraftModel::cache_exists(){
    return QFile::exists( this->cacheFileName() );
}


//=============================================================
/* @brief Load/reload the model */
void AircraftModel::load(bool reload_cache){

    bool cancelled = false;
    qDebug() << "aircraftModel::load" << reload_cache;

    QSize size(320,100);
    this->mainObject->progressDialog->resize(size);
    this->mainObject->progressDialog->setWindowIcon(QIcon(":/icon/load"));
    this->mainObject->progressDialog->show();

    // check cache exists and load
    if(reload_cache == false && this->cache_exists() ){

        bool ok = this->read_cache();
        if(ok){
            // cache loaded so get outta here
            this->mainObject->progressDialog->hide();
            qDebug() << "load cache and done";
            return;
        }
        reload_cache = true;
    }

    if(reload_cache) {

        // make a list of dirs to scan
        QStringList mdirs = this->mainObject->settings->value("custom_aircraft_dirs").toStringList();
        QString aircraft_base_path =  mainObject->X->aircraft_path();
        mdirs.prepend(aircraft_base_path);

        this->mainObject->progressDialog->setWindowTitle("Scanning aircraft directories");

        // scan each directory
        for(int i = 0; i < mdirs.size(); i++){
            cancelled = this->scan_dir(mdirs.at(i));
            if(cancelled){
                // Cancel button was pressed
                this->mainObject->progressDialog->hide();
                qDebug() << "scan cancelled";
                return;
            }

        }
        qDebug() << "= TOTAL items scanned === " << this->modelInfoList.length();
        bool ok = this->write_cache();
    }
    //this->read_cache();
    this->mainObject->progressDialog->hide();

}

bool AircraftModel::read_cache(){

    this->modelInfoList.clear();
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
        //qDebug() << line;
        // cols  << mi.dir << mi.description << mi.fdm << mi.authors <<  mi.full_path << mi.filter_dir;
        ModelInfo mi = ModelInfo();
        mi.dir = cols.at(0);
        mi.description = cols.at(1);
        mi.fdm = cols.at(2);
        mi.authors = cols.at(3);
        mi.full_path = cols.at(4);
        mi.filter_dir = cols.at(5);

        this->modelInfoList.append(mi);

        c++;
        line = in.readLine();

    }

    qDebug() << "LOADed CACHE items=" << this->modelInfoList.count();
    return true;
}



/* \brief Returns all -set.xml files in a directory. Recus is for custom folder and selecting a parent */
QFileInfoList AircraftModel::get_xml_set_files(QString dir_path, bool recus){
    if(recus){
        qDebug() << "#### RECUSS ###" << dir_path;
    }
    QDir dir( dir_path );
    QStringList filters;
    filters << "*-set.xml";
    QFileInfoList setList =  dir.entryInfoList(filters);
    if(recus){
        QFileInfoList allentries = dir.entryInfoList();
        for(int i = 0; i < allentries.length(); i++){
            //qDebug() << "--" << allentries.at(i).absoluteFilePath();
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
    //qDebug() << "set=" << xml_set_path;

    QFile xmlFile( xml_set_path );
    if (xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)){

        /* The file content is converted to UTF-8.
             Some files are Windows, encoding and throw error with QxmlQuery etc
             Its a hack and don't quite understand whats happening.. said pedro
        */
        QString xmlString = QString( xmlFile.readAll() ).toUtf8();
        //qDebug() << "=" << mi.dir << "=" << xmlString.length();
        QXmlQuery query;
        query.setFocus(xmlString);
        //query.setFocus(&xmlFile); << Because file is not QTF8 using sting instead
        query.setQuery("PropertyList/sim");
        if ( query.isValid() ){

            QString res;
            query.evaluateTo(&res);


            QDomDocument dom;
            dom.setContent("" + res + "");
            QDomNodeList nodes = dom.elementsByTagName("sim");

            QDomNode n = nodes.at(0);
            mi.description = n.firstChildElement("description").text().replace('\t', "");
            mi.authors = n.firstChildElement("author").text().trimmed().replace("\n","").replace('\t', "");
            mi.fdm = n.firstChildElement("flight-model").text().replace("\t", "");
            //qDebug() << "fdm" << mi.fdm;
            if (mi.fdm == "null"){
                mi.fdm = "";
            }
            mi.ok = true;

        } else {
            mi.ok = false;

        }
    } /*  xmlFile.open() */
    xmlFile.close();
    return mi;
}

/* \brief Writes internal modelInfoList to cache */
bool AircraftModel::write_cache(){

    // Removing cache file, if exists()
    if (this->cache_exists()) {
        outLog("*** FGx aircrafts/hangar data reload: cache file exists!");
        QFile::remove( this->cacheFileName() );
        outLog("*** FGx aircrafts/hangar data reload: REMOVED Aircraft CACHE FILE");
    }

    //= Open Cache File for write
    QFile cacheFile( this->cacheFileName() );
    if(!cacheFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "TODO Open error cachce file=ssssssssssssssssssssssssssssssssssssssss";
        return false;
    }
    this->mainObject->progressDialog->setWindowTitle("Writing cache");
    this->mainObject->progressDialog->setLabelText("standby...");
    this->mainObject->progressDialog->setRange(0, this->modelInfoList.length());
    QTextStream out(&cacheFile);

    for(int i = 0; i < this->modelInfoList.length(); i++){
        this->mainObject->progressDialog->setValue(i);
        QStringList cols;
        ModelInfo mi = this->modelInfoList.at(i);
        cols  << mi.dir << mi.description << mi.fdm << mi.authors <<  mi.full_path << mi.filter_dir;
        if(mi.full_path.length() == 0){
            qDebug() << cols;
        }
        out << cols.join("\t") << "\n";
    }

    cacheFile.close();

    qDebug() << "Wrote Cache" << this->modelInfoList.length();
    return true;
}
