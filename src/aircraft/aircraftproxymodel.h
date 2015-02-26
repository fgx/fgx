#ifndef AIRCRAFTPROXYMODEL_H
#define AIRCRAFTPROXYMODEL_H

#include <QSortFilterProxyModel>

class AircraftProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit AircraftProxyModel(QObject *parent = 0);

signals:

public slots:

};

#endif // AIRCRAFTPROXYMODEL_H
