
#include <QDebug>

#include <QDateTime>

#include "map/xmarblewidget.h"

/*
XMarbleWidget::XMarbleWidget(QWidget *parent) :
    Marble::MarbleWidget(parent)
{
    this->model_registered = false;
}
*/

void XMarbleWidget::register_flights_model(FlightsModel *flyMod)
{
    this->flightsModel = flyMod;
    this->model_registered = true;
}

/*
void XMarbleWidget::XMarbleWidget()
{
    this->radarModel = new QStandardItemModel(this);

    QStringList header_labels;
    header_labels << "Callsign" << "Model" <<  "Hdg" << "Alt" << "Lat" << "Lon" << "Flag" << "Count";
    radarModel->setHorizontalHeaderLabels(header_labels);

}
*/

void XMarbleWidget::customPaint(Marble::GeoPainter* painter)
{

     //qDebug() << "radar count" << radarModel->rowCount();

     return;
    //return;
    //painter->autoMapQuality();
	painter->save();

	//int f_height = 12;


	//== Draw Radar Widgets
    for(int idx=0; idx < this->flightsModel->rowCount(); idx++)
	{
        Marble::GeoDataCoordinates blip(this->flightsModel->item(idx, 5)->text().toFloat(),
                                        this->flightsModel->item(idx, 4)->text().toFloat(),
                                        0.0, Marble::GeoDataCoordinates::Degree
						);

		qreal xx;
		qreal yy;
		screenCoordinates(blip.longitude(Marble::GeoDataCoordinates::Degree),
						  blip.latitude(Marble::GeoDataCoordinates::Degree),
						  xx, yy
		);



		//painter->setPen(Qt::darkBlue);
		//painter->setBrush(Qt::green);

		//painter->setMatrix();
		painter->setOpacity(0.9);
		painter->setPen(Qt::black);
		painter->setBrush(Qt::black);
		painter->drawRect(xx + 8, yy - 7, 60, 22);
		painter->setOpacity(1.0);
		//painter->drawRoundRect(blip,100,20,5,5,false);

		painter->setPen(Qt::red);
		painter->setBrush(Qt::red);
		painter->drawEllipse(blip, 4, 4);
		//GeoDataCoordinates blip2(home);




		//qDebug() << xx << yy;
		//painter->drawText(blip, radarModel->item(idx, 0)->text());
		painter->setPen(Qt::white);
		QFont font = painter->font();
		font.setFamily("Arial");
		font.setBold(true);
		font.setPixelSize(10);
		painter->setFont(font);
        painter->drawText(xx + 12, yy + 3, this->flightsModel->item(idx, 0)->text());

		font = painter->font();
		//font.setFamily("Arial");
		font.setBold(true);
		font.setPixelSize(8);
		painter->setFont(font);
        painter->drawText(xx + 14, yy + 12, this->flightsModel->item(idx, 3)->text());



	} /* Loop radar */

	//GeoGraphicsItem *i = new GeoGraphicsItem();
	//i->

	painter->restore();
}


void XMarbleWidget::radar_refresh()
{
	update();
}
