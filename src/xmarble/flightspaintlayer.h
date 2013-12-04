#ifndef FLIGHTSPAINTLAYER_H
#define FLIGHTSPAINTLAYER_H

#include <marble/MarbleWidget.h>
#include <marble/MarbleMap.h>
#include <marble/MarbleModel.h>
#include <marble/GeoPainter.h>
#include <marble/LayerInterface.h>

using namespace Marble;

class FlightsPaintLayer : public QObject, public LayerInterface
{
   // Q_OBJECT
public:
    // Constructor
    FlightsPaintLayer(MarbleWidget* widget);

    // Implemented from LayerInterface
    virtual QStringList renderPosition() const;

    // Implemented from LayerInterface
    virtual bool render( GeoPainter *painter, ViewportParams *viewport,
                        const QString& renderPos = "NONE", GeoSceneLayer * layer = 0 );

    // Overriding QObject
    virtual bool eventFilter(QObject *obj, QEvent *event);

    GeoDataCoordinates approximate(const GeoDataCoordinates &base, qreal angle, qreal dist) const;

private:
    MarbleWidget* m_widget;

    int m_index;
};


#endif // FLIGHTSPAINTLAYER_H
