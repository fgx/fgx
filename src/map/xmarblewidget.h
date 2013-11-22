#ifndef XMARBLEWIDGET_H
#define XMARBLEWIDGET_H


#include <QStandardItemModel>

#include "xobjects/mainobject.h"
class MainObject;

#include "marble/MarbleGlobal.h"
#include "marble/MarbleWidget.h"

#include "marble/GeoPainter.h"
#include "marble/GeoDataCoordinates.h"


//using namespace Marble;





class XMarbleWidget : public Marble::MarbleWidget
{
public:

   // explicit XMarbleWidget(QWidget *parent);

    MainObject *mainObject;
    bool model_registered;

    //void radar_init(QStandardItemModel *mRadar);
    void register_mainobject(MainObject *mOb);


	void radar_refresh();

    virtual void customPaint(Marble::GeoPainter* painter);


};


#endif // XMARBLEWIDGET_H
