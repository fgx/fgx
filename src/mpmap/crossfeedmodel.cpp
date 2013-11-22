#include "crossfeedmodel.h"

CrossfeedModel::CrossfeedModel(QObject *parent) :
    QStandardItemModel(parent)
{

    setColumnCount(7);

    //QStringList headers;
    //headers  << "Flight ID" << "Callsign" << "Alt" << "Heading" << "Speed" << "Aircraft" << "Foo";
    //setHorizontalHeaderLabels(headers);

    QStandardItem *item;

    item = new QStandardItem("Flight ID");
    this->setHorizontalHeaderItem(C_FLIGHT_ID, item);

    item = new QStandardItem("Callsign");
    this->setHorizontalHeaderItem(C_FLIGHT_ID, item);

    /*
    tree->header()->setStretchLastSection(true);
    tree->header()->setResizeMode(QHeaderView::Stretch);
    tree->headerItem()->setText(C_CALLSIGN, "Callsign");
    tree->headerItem()->setText(C_AIRCRAFT, "Aircraft");
    tree->headerItem()->setText(C_ALTITUDE, "Alt");
    tree->headerItem()->setText(C_HEADING, "Hdg");
    tree->headerItem()->setText(C_PITCH, "Pitch");
    tree->headerItem()->setText(C_LAT, "Lat");
    tree->headerItem()->setText(C_LON, "Lon");
    tree->headerItem()->setText(C_FLAG, "Flag");
    tree->headerItem()->setText(C_COUNT, "Count");

    tree->headerItem()->setTextAlignment(C_ALTITUDE, Qt::AlignRight);
    tree->headerItem()->setTextAlignment(C_HEADING, Qt::AlignRight);
    tree->headerItem()->setTextAlignment(C_LAT, Qt::AlignRight);
    tree->headerItem()->setTextAlignment(C_LON, Qt::AlignRight);
    */

}
