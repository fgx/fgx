
#include <QtDebug>
#include <QKeyEvent>

#include "flightspaintlayer.h"

// This stuff is verbatim as a start from
// http://techbase.kde.org/Projects/Marble/LayerInterface

FlightsPaintLayer::FlightsPaintLayer(MarbleWidget* widget, MODE mode) : m_widget(widget), m_index(0)
{
    // nothing to do
    this->map_mode = mode;
    this->flightsModel = 0;
}

void FlightsPaintLayer::register_flights_model(FlightsModel *fmod)
{
    this->flightsModel = fmod;
}


QStringList FlightsPaintLayer::renderPosition() const
{
    // We will paint in exactly one of the following layers.
    // The current one can be changed by pressing the '+' key
    QStringList layers = QStringList() << "SURFACE" << "HOVERS_ABOVE_SURFACE";
    //layers << "ORBIT" << "USER_TOOLS" << "STARS";

    int index = m_index % layers.size();
    //qDebug() << QStringList() << layers.at(index);
    return QStringList() << layers.at(index);
}

//= THis is unused atmo  its a cut + plaster
bool FlightsPaintLayer::eventFilter(QObject *obj, QEvent *event)
{
    // Adjust the current layer when '+' is pressed
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Plus) {
            ++m_index;
            return true;
        }
    }

    return false;
}



bool FlightsPaintLayer::render( GeoPainter *painter, ViewportParams *viewport,
    const QString& renderPos, GeoSceneLayer * layer )
{


    painter->setRenderHint(QPainter::Antialiasing, true);

    //== Draw Radar Widgets
    for(int idx=0; idx < this->flightsModel->rowCount(); idx++)
    {
        if(this->map_mode == MINI_MODE){
            if(this->flightsModel->item(idx, FlightsModel::C_CURRENT)->text() == "1"){
                painter->setPen( QPen(QBrush(QColor::fromRgb(255,0,0,255)), 3.0, Qt::SolidLine, Qt::RoundCap ) );
            }else{
                painter->setPen( QPen(QBrush(QColor::fromRgb(0,0,50,255)), 3.0, Qt::SolidLine, Qt::RoundCap ) );
            }

        }else{
            painter->setPen( QPen(QBrush(QColor::fromRgb(255,0,0,255)), 3.0, Qt::SolidLine, Qt::RoundCap ) );
        }
        // Yes,, LON, LAT is order !!
        Marble::GeoDataCoordinates blip(this->flightsModel->item(idx, FlightsModel::C_LON)->text().toFloat(),
                                        this->flightsModel->item(idx, FlightsModel::C_LAT)->text().toFloat(),
                                        0.0, //this->flightsModel->item(idx, FlightsModel::C_ALTITUDE)->text().toInt() * 10, << mad idea to make flights at 100 * 10 and exagerated position
                                        Marble::GeoDataCoordinates::Degree
                        );
        painter->drawEllipse(blip, 5, 5);

        QString callsign = this->flightsModel->item(idx, FlightsModel::C_CALLSIGN)->text();
        QList <XPosition> trails = this->flightsModel->flightPositions[callsign]->positions;
        //qDebug() << callsign << trails.length();

        if(this->map_mode == NORMAL_MODE){
            for(int tidx = 0; tidx < trails.length(); tidx++){

                XPosition t_blip  = trails.at( tidx );
                painter->setPen( QPen(QBrush(t_blip.alt_color), 1.0, Qt::SolidLine, Qt::RoundCap ) );
                painter->drawEllipse(t_blip.coord, 2, 2);
            }
        }
    }

    return true;
}
