#ifndef MODELINFO_H
#define MODELINFO_H

#include <QString>
#include <QStringList>

/* \brief Conveniance to hand around model/xml file properties */
class ModelInfo
{
public:
    ModelInfo();

    bool ok; // is true is xml parsed and data read
    QString aero;
    QString xml_file;
    QString dir; // short parentdir
    QString fdm;
    QString authors;
    QString description;
    QString file_path;
    QString filter_path; // for filter

};

#endif // MODELINFO_H
