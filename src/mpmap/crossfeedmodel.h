#ifndef CROSSFEEDMODEL_H
#define CROSSFEEDMODEL_H

#include <QStandardItemModel>

#include "xobjects/mainobject.h"
class MainObject;

class CrossfeedModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit CrossfeedModel(QObject *parent);



signals:

public slots:

};

#endif // CROSSFEEDMODEL_H
