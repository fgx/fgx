#include "modelinfo.h"

ModelInfo::ModelInfo()
{
}



QString ModelInfo::xml_file(){
    QFileInfo fInfo(this->full_path);
    return fInfo.fileName();
}

QString ModelInfo::aero(){
    QString a = this->xml_file();
    a.chop(8);
    return a;
}


QString ModelInfo::filter(){
    return this->aero().append(this->description.replace(" ", ""));
}
