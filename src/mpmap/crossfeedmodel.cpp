#include "crossfeedmodel.h"

CrossfeedModel::CrossfeedModel(QObject *parent) :
    QStandardItemModel(parent)
{

    setColumnCount(7);

    QStringList headers;
    headers  << "Flight ID" << "Callsign" << "Alt" << "Heading" << "Speed" << "Aircraft" << "Foo";
    setHorizontalHeaderLabels(headers);


}
