#include "crossfeedmodel.h"

CrossfeedModel::CrossfeedModel(MainObject *mob, QObject *parent) :
    QStandardItemModel(parent)
{
    this->mainObject = mob;

    setColumnCount(7);

    QStringList headers;
    headers  << "Flight ID" << "Callsign" << "Alt" << "Heading" << "Speed" << "Aircraft" << "Foo";
    setHorizontalHeaderLabels(headers);


}
