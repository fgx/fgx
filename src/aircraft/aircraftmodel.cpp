
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
/*
QModelIndex AircraftModel::index(int row, int column, const QModelIndex &parent) const
{
    if( rowCount() == 0 ){
        return QModelIndex();
    }
    if (row > rowCount() - 1 ){
        //sqDebug() << "index()" << "got ya!";
        return QModelIndex();
    }
    //qDebug() << "data()" << row << "  " << column;
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
int AircraftModel::columnCount() const {
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
    //qDebug() << "data()" << index << " " << role;
    if (!index.isValid()){
        qDebug() << "data() invalid index";
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

*/


/*
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
            qDebug() << " @@@@@ xmlfile=" << mi.full_path <<  mi.description << mi.authors;
        }
        if(mi.ok){
            this->modelInfoList.append(mi);
        } else {
            qDebug() << " Problem not ok=" << mi.full_path;
        }
        this->mainObject->progressDialog->setValue(fi);
        this->mainObject->progressDialog->setLabelText( dir + "\n" );
        if(this->mainObject->progressDialog->wasCanceled()){
            return true;
        }
    }
    return false;
}
*/
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

    if(reload_cache) {

        QSize size(320,100);
        this->mainObject->progressDialog->resize(size);
        this->mainObject->progressDialog->setMinimumDuration(20);
        this->mainObject->progressDialog->reset();
        this->mainObject->progressDialog->setWindowIcon(QIcon(":/icon/load"));
        this->mainObject->progressDialog->setAutoClose(false);
        this->mainObject->progressDialog->setAutoReset(false);
        this->mainObject->progressDialog->setWindowTitle("Scanning aircraft directories");
        this->mainObject->progressDialog->setRange(0, 0);
        this->mainObject->progressDialog->setValue(0);
        this->mainObject->progressDialog->show();


        //= List of model dirs to scan
        QStringList mdirs = this->mainObject->settings->value("custom_aircraft_dirs").toStringList();
        QString aircraft_base_path =  mainObject->X->aircraft_path();
        mdirs.prepend(aircraft_base_path);
        qDebug() << "Dirs = " << mdirs;

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
            return;
        }
        QTextStream out(&cacheFile);

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

                //qDebug() << ">" << ix << " " << adir << " " << mi.aero() << " " << mi.fdm << " " << mi.authors ;

                if(mi.dir == "KC135"){
                    qDebug() << " @@@@@ xmlfile=" << mi.full_path << mi.description << mi.authors;
                }
                if(mi.ok){
                    //this->modelInfoList.append(mi);
                    QStringList cols;
                    cols  << mi.dir << mi.description << mi.fdm << mi.authors <<  mi.full_path << mi.filter_dir;
                    out << cols.join("\t") << "\n";
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
        cacheFile.close();
        qDebug() << " @@@@@ Read Done ";

        // No items so get outta here
        //if (modelInfoList.length() == 0) {
        //    qDebug() << " no items so exit";
        //    this->mainObject->progressDialog->cancel();
        //    return;
        //}

        qDebug() << "= TOTAL items scanned === ";
        //bool ok = this->write_cache();
    } // reloead_cache == true;
    //this->read_cache();
    //this->dataChanged(this->createIndex(0, 0), this->createIndex(this->rowCount() - 1, this->columnCount(QModelIndex()) - 1));
    //this->data_changed();
    this->mainObject->progressDialog->hide();

}

bool AircraftModel::read_cache(){

    //this->modelInfoList.clear();
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
        //qDebug() << line;
        // cols  << mi.dir << mi.description << mi.fdm << mi.authors <<  mi.full_path << mi.filter_dir;
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
        //row.at(C_FILTER_PATH)->setText(mainObject->X->aircraft_path());

        QString filter_str = aero.append(cols.at(2));
        //filter_str.append(cols.at(2) );
        row.at(C_FILTER)->setText( filter_str );
        //row.at(C_BASE)->setText("1");

        /*
        ModelInfo mi = ModelInfo();
        mi.dir = cols.at(0);
        mi.description = cols.at(1);
        mi.fdm = cols.at(2);
        mi.authors = cols.at(3);
        mi.full_path = cols.at(4);
        mi.filter_dir = cols.at(5);

        this->modelInfoList.append(mi);
        */
        c++;
        line = in.readLine();

    }

    qDebug() << "LOADed CACHE items=" << c;
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

/* \brief Writes internal modelInfoList to cache */
/*
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
    QTextStream out(&cacheFile);

    for(int i = 0; i < this->modelInfoList.length(); i++){
        ModelInfo mi = this->modelInfoList.at(i);
        QStringList cols;
        cols  << mi.dir << mi.description << mi.fdm << mi.authors <<  mi.full_path << mi.filter_dir;
        out << cols.join("\t") << "\n";
    }

    cacheFile.close();
    qDebug() << "Wrote Cache" << this->modelInfoList.length();
    return true;
}
*/
