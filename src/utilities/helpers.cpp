// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // helpers.cpp
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information


#include <QtDebug>

#include <QList>
#include <QAbstractButton>
#include <QVariant>
#include <QStringList>

#include "helpers.h"

Helpers::Helpers()
{
}


void Helpers::select_radio(QButtonGroup *buttonGroup, QString value){
	QList<QAbstractButton *> buttons = buttonGroup->buttons();
	for(int idx = 0; idx < buttons.size(); idx++){
		if(buttons.at(idx)->property("value").toString() == value){
			buttons.at(idx)->setChecked(true);
			return;
		}
	}
	// select first anyway
	buttons.at(0)->setChecked(true);
}


void Helpers::select_combo(QComboBox *combo, QString value){
	int idx = combo->findData( value );
	combo->setCurrentIndex( idx == -1 ? 0 : idx );
}

/*
			 lat="N52 18.101"
			 lon="E04 47.433"
Degrees Minutes.m to Decimal Degrees
.d = M.m / 60
Decimal Degrees = Degrees + .d

*/
QString Helpers::hmm_to_decimal(QString hmm)
{
	QStringList parts = hmm.split(" ");
	QString h = parts.at(0);
    double deg = 0.0; // just to remove compiler warning

	if( h.contains("N") ){
		h.replace("N", "");
		deg = h.toDouble();

	}else if(h.contains("S")){
		h.replace("S", "");
		deg = h.toDouble() * -1;

	}else if(h.contains("E")){
		h.replace("E", "");
		deg = h.toDouble();

	}else if(h.contains("W")){
		h.replace("W", "");
		deg = h.toDouble() * -1;
	}

	double ms;
	double calc = 60.0;
	double partsone = parts.at(1).toDouble();
	if(deg > 0){
		ms = deg + partsone * calc / 3600;
	}else{
		ms = deg - partsone * calc / 3600;
	}
	QString nn = QString::number ( ms, 'g', 10 );
	return nn;
}
