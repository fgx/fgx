
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

    item = new QStandardItem("Map View");
    this->setHorizontalHeaderItem(C_BUTT, item);

    item = new QStandardItem("Zoom");
    this->setHorizontalHeaderItem(C_ZOOM, item);

    item = new QStandardItem("Lat");
    this->setHorizontalHeaderItem(C_LAT, item);

    item = new QStandardItem("Lon");
    this->setHorizontalHeaderItem(C_LON, item);

    item = new QStandardItem("View");
    this->setHorizontalHeaderItem(C_VIEW, item);


    this->load();
}


void MapViewsModel::load()
{
    QSettings settings;
    this->removeRows(0, this->rowCount());
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
        row << new QStandardItem()
            << new QStandardItem(settings.value("lat").toString())
            << new QStandardItem(settings.value("lon").toString())
            << new QStandardItem(settings.value("zoom").toString())
            << iLabel;
        for(int i = 0; i < row.count(); i++){
            row.at(i)->setEditable(false);
        }
        this->appendRow(row);
    }
    settings.endArray();
}
void MapViewsModel::save()
{

    QSettings settings;
    int rc = this->rowCount();
    settings.beginWriteArray( MapViewsModel::SETTINGS_TAG );

        for(int i = 0; i < rc; i++){
            settings.setArrayIndex(i);
            settings.setValue("label", this->item(i, C_VIEW)->text());
            settings.setValue("lat", this->item(i, C_LAT)->text());
            settings.setValue("lon", this->item(i, C_LON)->text());
            settings.setValue("zoom", this->item(i, C_ZOOM)->text());
       }
    settings.endArray();
    settings.sync();
}

void MapViewsModel::add_view(QString label, QString lat, QString lon, int zoom)
{

}

void MapViewsModel::remove_view(QString label)
{
    QList<QStandardItem *> fitems = this->findItems(label, Qt::MatchExactly, MapViewsModel::C_VIEW);
   // qDebug() << fitems.count();
    if(fitems.count() > 0){
        this->removeRow(fitems.at(0)->row());
        this->save();
    }
}
