
#include <QtDebug>
#include <QStandardItemModel>
#include "aircraftproxymodel.h"

AircraftProxyModel::AircraftProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

// This adds or removed entries to the filter
void AircraftProxyModel::set_dir(QString dir, bool show){

    int idx = this->dirs.indexOf(dir);

    if(show == false){
        if( idx != -1){
            this->dirs.removeAt(idx);
        }
    } else {
        if( idx == -1){
            this->dirs.append(dir);
        }
    }
    this->invalidateFilter();
}

bool AircraftProxyModel::filterAcceptsRow(int source_row, const QModelIndex & source_parent ) const {

    QModelIndex dirIdx = this->sourceModel()->index(source_row, 7);
    QString dir = this->sourceModel()->data(dirIdx,  Qt::DisplayRole).toString();
    //int idx = this->dirs.indexOf(dir);
    if( this->dirs.indexOf(dir) == -1){
        return false;
    }
    return QSortFilterProxyModel::filterAcceptsRow( source_row, source_parent);
}
