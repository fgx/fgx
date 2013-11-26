
#include <QtDebug>
#include <QSettings>

#include <QToolButton>

#include "mapviewsmodel.h"

const QString MapViewsModel::SETTINGS_TAG = QString("map_views");

MapViewsModel::MapViewsModel(QObject *parent) :
    QStandardItemModel(parent)
{

    setColumnCount(5);

    QStandardItem *item;

    item = new QStandardItem("Label");
    this->setHorizontalHeaderItem(C_LABEL, item);

    item = new QStandardItem("Zoom");
    this->setHorizontalHeaderItem(C_ZOOM, item);

    item = new QStandardItem("Lat");
    this->setHorizontalHeaderItem(C_LAT, item);

    item = new QStandardItem("Lon");
    this->setHorizontalHeaderItem(C_LON, item);



    this->load();
}


void MapViewsModel::load()
{
    QSettings settings;
    int size = settings.beginReadArray( MapViewsModel::SETTINGS_TAG );
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        QString lbl = settings.value("label").toString();
        if(lbl.isEmpty()){
            lbl.append("- no label -");
        }
        QStandardItem *iLabel = new QStandardItem(lbl);
        iLabel->setIcon(QIcon(":/micon/map"));
        QList<QStandardItem *> row;
        row << iLabel
             << new QStandardItem(settings.value("lat").toString())
             << new QStandardItem(settings.value("lon").toString())
             << new QStandardItem(settings.value("zoom").toString());
        for(int i = 0; i < row.count(); i++){
            row.at(i)->setEditable(false);
        }
        this->appendRow(row);
    }
    settings.endArray();
}
void MapViewsModel::save()
{

}

void MapViewsModel::add_view(QString label, QString lat, QString lon, int zoom)
{

}

void MapViewsModel::remove_view(QString label)
{

}
