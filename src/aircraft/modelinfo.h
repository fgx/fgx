#ifndef MODELINFO_H
#define MODELINFO_H

#include <QString>
#include <QStringList>
#include <QFileInfo>

/* \brief Conveniance to hand around model/xml file properties */
class ModelInfo
{
public:
    ModelInfo();

    bool ok; // is true is xml parsed and data read
    QString dir; // short parentdir
    QString fdm;
    QString authors;
    QString description;
    QString full_path;
    QString filter_dir;

};

#endif // MODELINFO_H
