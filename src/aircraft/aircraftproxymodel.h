#ifndef AIRCRAFTPROXYMODEL_H
#define AIRCRAFTPROXYMODEL_H


#include <QStringList>
#include <QSortFilterProxyModel>

class AircraftProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit AircraftProxyModel(QObject *parent = 0);

    QStringList dirs;
    void set_dir(QString dir, bool show);

    bool filterAcceptsRow(int source_row, const QModelIndex & source_parent ) const;
signals:

public slots:

};

#endif // AIRCRAFTPROXYMODEL_H
