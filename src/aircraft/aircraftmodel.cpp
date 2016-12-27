
#include <QtDebug>
#include <QStringList>
#include <QProgressDialog>
#include <QXmlQuery>

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

bool AircraftModel::scan_dir(QString dir){

    qDebug() << "##Scan = ", dir;
    QFileInfoList xmlSets = AircraftModel::get_xml_set_files(dir, true);

    this->mainObject->progressDialog->setRange(0, xmlSets.length());

    for(int fi = 0; fi < xmlSets.length(); fi++){
        // get data from the model file
        ModelInfo mi = AircraftModel::read_model_xml(xmlSets.at(fi).absoluteFilePath());
        this->modelInfoList.append(mi);
        this->mainObject->progressDialog->setValue(fi);
        this->mainObject->progressDialog->setLabelText(mi.aero);
        if(this->mainObject->progressDialog->wasCanceled()){
            return true;
        }
    }
    return false;
}




//=============================================================
/* @brief Load/reload the model */
void AircraftModel::load_aircraft(bool  reload_cache){

    bool cancelled = false;


    QSize size(320,100);
    this->mainObject->progressDialog->resize(size);
    this->mainObject->progressDialog->setWindowIcon(QIcon(":/icon/load"));
    this->mainObject->progressDialog->show();

    if (QFile::exists(mainObject->data_file("aircrafts.txt"))) {
        bool ok = this->read_cache();
        if(!ok){
            reload_cache = true;
        } else {
            return;
        }
    } else {
        reload_cache = true;
    }


    if(reload_cache) {

        QStringList mdirs = this->mainObject->settings->value("custom_aircraft_dirs").toStringList();

        QString aircraft_base_path =  mainObject->X->aircraft_path();
        mdirs.append(aircraft_base_path);

        for(int i = 0; i < mdirs.size(); i++){
            cancelled = this->scan_dir(mdirs.at(i));
            if(cancelled){
                this->mainObject->progressDialog->hide();
                return;
            }
            qDebug() << "=" << this->modelInfoList.length();
        }
        bool ok = this->write_cache();
    }
    this->mainObject->progressDialog->hide();

}

bool AircraftModel::read_cache(){

    if( QFile::exists(mainObject->data_file("aircrafts.txt")) == false) {
        return false;
    }

    int c = 0;

    QList<QStandardItem*> row;

    this->removeRows(0, this->rowCount());


    //=== Load Base Package
    QFile dataFile(mainObject->data_file(("aircraft.txt")));
    if (!dataFile.open(QIODevice::ReadOnly | QIODevice::Text)){
           qDebug() << "no aircraft.txt";
           return false;
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
    return true;
}



QList<QStandardItem*> AircraftModel::create_model_row(){
    QList<QStandardItem*> lst;
    for(int i = 0; i < this->columnCount(); i++){
        lst.append( new QStandardItem() );
    }
    this->appendRow(lst);
    return lst;
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
    mi.file_path = xml_set_path;
    //mi.dir_path = fInfo.dir().absolutePath();
    mi.dir = fInfo.dir().dirName();

    // the model = filename without --set.xml
    mi.aero = fInfo.fileName();
    mi.aero.chop(8);


    QFile xmlFile( xml_set_path );
    if (xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)){

        /* The file content is converted to UTF-8.
             Some files are Windows, encoding and throw error with QxmlQuery etc
             Its a hack and don't quite understand whats happening.. said pedro
        */
        QString xmlString = QString(xmlFile.readAll()).toUtf8();

        QXmlQuery query;
        query.setFocus(xmlString);
        //query.setFocus(&xmlFile); << Because file is not QTF8 using sting instead
        query.setQuery("PropertyList/sim");
        if (query.isValid()){

            QString res;
            query.evaluateTo(&res);
            xmlFile.close();

            QDomDocument dom;
            dom.setContent("" + res + "");
            QDomNodeList nodes = dom.elementsByTagName("sim");

            QDomNode n = nodes.at(0);
            mi.description = n.firstChildElement("description").text();
            mi.authors = n.firstChildElement("author").text().trimmed().replace(("\n"),"");
            mi.fdm = n.firstChildElement("flight-model").text();
            mi.ok = true;

        } /* !query.isValid() */
    } /*  xmlFile.open() */

    return mi;
}

bool AircraftModel::write_cache(){

    // Removing cache file, if exists()
    if (QFile::exists(mainObject->data_file("aircrafts.txt"))) {
        outLog("*** FGx aircrafts/hangar data reload: cache file exists!");
        QFile::remove(mainObject->data_file("airports.txt"));
        outLog("*** FGx aircrafts/hangar data reload: REMOVED Aircraft CACHE FILE");
    }

    //= Cache File
    QFile cacheFile( mainObject->data_file("aircraft.txt") );
    if(!cacheFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "TODO Open error cachce file=ssssssssssssssssssssssssssssssssssssssss";
        return true;
    }

    QTextStream out(&cacheFile);

    for(int i = 0; i < this->modelInfoList.length(); i++){
        QStringList cols;
        ModelInfo mi = this->modelInfoList.at(i);
        cols  << mi.dir << mi.aero << mi.description << mi.fdm << mi.authors << mi.xml_file << mi.file_path << mi.filter_path;
        out << cols.join("\t") << "\n";
    }

    cacheFile.close();
    return false;
}
