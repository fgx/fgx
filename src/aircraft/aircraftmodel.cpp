
#include <QtDebug>
#include <QStringList>
#include <QProgressDialog>
#include <QXmlQuery>

#include "aircraft/aircraftmodel.h"


AircraftModel::AircraftModel(MainObject *mOb) :
    QAbstractItemModel()
{

    this->mainObject = mOb;
    headerLabels << "Dir" << "Aero" << "Description" << "FDM" << "Authors" << "XML" << "FilePath" << "FilterDir" << "Filter";
}

QString AircraftModel::cacheFileName(){
    return mainObject->data_file("aircraft_cache.txt");
}

QModelIndex AircraftModel::index(int row, int column, const QModelIndex &parent) const
{
    if( rowCount() == 0 ){
        return QModelIndex();
    }
    if (row > rowCount() -1 ){
        qDebug() << "index()" << "got ya!";
        return QModelIndex();
    }
    return this->createIndex(row, column);
}
QModelIndex AircraftModel::parent(const QModelIndex &child) const {
     return QModelIndex();
}

int AircraftModel::rowCount(const QModelIndex &parent) const {
    return this->modelInfoList.length();
}

int AircraftModel::rowCount() const {
    return this->modelInfoList.length();
}

int AircraftModel::columnCount(const QModelIndex &idx) const {
     return this->headerLabels.length();
}

QVariant AircraftModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return this->headerLabels.at(section);
    }
    return QVariant();
}

Qt::ItemFlags AircraftModel::flags(const QModelIndex &index) const {
    return QAbstractItemModel::flags(index);
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

    // Value for each Col cell
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





bool AircraftModel::scan_dir(QString dir){
    return false;
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
    qDebug() << "=========== aircraftModel::load reload="  << reload_cache;

    QSize size(320,100);
    this->mainObject->progressDialog->resize(size);
    this->mainObject->progressDialog->reset();
    this->mainObject->progressDialog->setWindowIcon(QIcon(":/icon/load"));
    this->mainObject->progressDialog->setAutoClose(false);
    this->mainObject->progressDialog->setAutoReset(false);
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

        this->modelInfoList.clear();
        this->mainObject->progressDialog->setWindowTitle("Scanning aircraft directories");

        //= List of model dirs to scan
        QStringList mdirs = this->mainObject->settings->value("custom_aircraft_dirs").toStringList();
        QString aircraft_base_path =  mainObject->X->aircraft_path();
        mdirs.prepend(aircraft_base_path);
        qDebug() << "Dirs = " << mdirs;


        this->mainObject->progressDialog->setRange(0, 0);
        this->mainObject->progressDialog->setValue(0);

        //= Walk the model dirs
        for(int i = 0; i < mdirs.length(); i++){

            QString adir = mdirs.at(i);
            this->mainObject->progressDialog->setLabelText(adir);

            qDebug() << " > Scan = " << adir;
            QFileInfoList xmlSets = AircraftModel::get_xml_set_files(adir, true);
            qDebug() << " < got  xmlSets = " << xmlSets.length();
            //xmlSets.append(xSets);
            //xHashList[mdirs.at(i)] = xSets;
            if(xmlSets.length() == 0){
                qDebug() << "No xmlsets = << continue";
                continue;
            } else {
                qDebug() << " GOt xmlsets = " << xmlSets.length() << " = " << adir;
            }

            this->mainObject->progressDialog->setRange(0, xmlSets.length() );

            for(int ix = 0; ix < xmlSets.length(); ix++){
                //qDebug() << "-----------------------\nix=" << ix;
                this->mainObject->progressDialog->setValue(ix);

                ModelInfo mi = AircraftModel::read_model_xml( xmlSets.at(ix).absoluteFilePath() );
                mi.filter_dir = adir;
                //mi.dir = xmlSets.at(ix).absoluteDir().dirName();

                //qDebug() << ">" << ix << " " << adir << " " << mi.aero() << " " << mi.fdm << " " << mi.authors ;


                if(mi.dir == "KC135"){
                    qDebug() << " @@@@@ xmlfile=" << mi.full_path << mi.aero() << mi.description << mi.authors;
                }
                if(mi.ok){
                    this->modelInfoList.append(mi);
                   } else {
                    qDebug() << " Problem not ok=" << mi.full_path;
                }


                //this->mainObject->progressDialog->setLabelText( mdirs.at(i) + "\n" + mi.aero());
                if( this->mainObject->progressDialog->wasCanceled() ){
                    this->mainObject->progressDialog->hide();
                    qDebug() << " cancelled in set scan";
                    return;
                } else {
                    //qDebug() << " = " << mdirs.at(i) << " = " << ix ;
                }

            }  // xmlSets
        } // mdirs
        qDebug() << " @@@@@ Read Done " << modelInfoList.length();

        // No items so get outta here
        if (modelInfoList.length() == 0) {
            qDebug() << " no items so exit";
            this->mainObject->progressDialog->cancel();
            return;
        }
         qDebug() << " Readt to save " << modelInfoList.length();
        /*
        QHashIterator<QString, QFileInfoList> it(xHashList);
        //for(int fi = 0; fi < xmlSets.length(); fi++){
        while(it.hasNext()) {
            QFileInfoList xmlSets = it.value();
            if(xmlSets.length() > 0) {
                for(int fi = 0; fi < xmlSets.length(); fi++){
                    // get data from the model file
                    ModelInfo mi = AircraftModel::read_model_xml(xmlSets.at(fi).absoluteFilePath());
                    //;
                    if(mi.dir == "KC135"){
                        qDebug() << " @@@@@ xmlfile=" << mi.full_path << mi.aero() << mi.description << mi.authors;
                    }
                    if(mi.ok){
                        this->modelInfoList.append(mi);
                    } else {
                        qDebug() << " Problem not ok=" << mi.full_path;
                    }
                    this->mainObject->progressDialog->setValue(fi);
                    this->mainObject->progressDialog->setLabelText( it.key() + "\n" + mi.aero());
                    if(this->mainObject->progressDialog->wasCanceled()){
                        return;
                    }
                }
            }
        }
        */
        //qDebug() << " === sets Count = " << this->modelInfoList.length();

        // scan each directory
        /*
        for(int i = 0; i < mdirs.size(); i++){
            cancelled = this->scan_dir(mdirs.at(i));
            if(cancelled){
                // Cancel button was pressed
                this->mainObject->progressDialog->hide();
                qDebug() << "scan cancelled";
                return;
            }

        }*/
        qDebug() << "= TOTAL items scanned === " << this->modelInfoList.length();
        //bool ok = this->write_cache();
    } // reloead_cache == true;
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
    //if(recus){
    //    qDebug() << "#### RECUSS ###" << dir_path;
    //}
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
            if(i == 10){
                //return setList;
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
    //qDebug() << "\t> read_model_xml=" << xml_set_path;

    QFile xmlFile( xml_set_path );
    if (xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)){

        /* The file content is converted to UTF-8.
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
            //qDebug() << "\t  fdm" << mi.fdm;
            mi.fdm = "";
        }
        mi.ok = true;

        xmlFile.close();
        //qDebug() << "\t = Closed";
    } else {
        qDebug() << "\t = Not Opened";
    }
    /*  xmlFile.open() */
    //qDebug() << "\t< read_model_xml=" << mi.ok;


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
    if(this->modelInfoList.length() == 0) {
        return false;
    }
    this->mainObject->progressDialog->setWindowTitle("Writing cache");
    this->mainObject->progressDialog->setLabelText("standby...");
    this->mainObject->progressDialog->setRange(0, this->modelInfoList.length());
    QTextStream out(&cacheFile);

    for(int i = 0; i < this->modelInfoList.length(); i++){
        this->mainObject->progressDialog->setValue(i);
        ModelInfo mi = this->modelInfoList.at(i);
        QStringList cols;
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
