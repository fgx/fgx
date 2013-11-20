#ifndef XMARBLEWIDGET_H
#define XMARBLEWIDGET_H


#include <QStandardItemModel>


#include "marble/MarbleGlobal.h"
#include "marble/MarbleWidget.h"

#include "marble/GeoPainter.h"
#include "marble/GeoDataCoordinates.h"


//using namespace Marble;





class XMarbleWidget : public Marble::MarbleWidget
{
public:

	QStandardItemModel *radarModel;
	void radar_init(QStandardItemModel *mRadar);
	void radar_refresh();

    virtual void customPaint(Marble::GeoPainter* painter);


};


#endif // XMARBLEWIDGET_H
