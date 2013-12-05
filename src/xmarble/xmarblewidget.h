#ifndef XMARBLEWIDGET_H
#define XMARBLEWIDGET_H


#include <QStandardItemModel>

#include "flights/flightsmodel.h"
class FlightsModel;

//#include "marble/MarbleGlobal.h"
#include "marble/MarbleWidget.h"

#include "marble/GeoPainter.h"
#include "marble/GeoDataCoordinates.h"


//using namespace Marble;





class XMarbleWidget : public Marble::MarbleWidget
{

public:

   // explicit XMarbleWidget(QWidget *parent);

    FlightsModel *flightsModel;
    bool model_registered;

    //void radar_init(QStandardItemModel *mRadar);
    void register_flights_model(FlightsModel *flyMod);


	void radar_refresh();

    virtual void customPaint(Marble::GeoPainter* painter);

public slots:
    void do_update();

};


#endif // XMARBLEWIDGET_H
