
#include <QDebug>

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QRadioButton>

#include "timeweatherwidget.h"

TimeWeatherWidget::TimeWeatherWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{
	mainObject = mOb;

	QHBoxLayout *mainLayout = new QHBoxLayout();
	setLayout(mainLayout);

	//===============================================================
	// Left Column with Time / Season
	//===============================================================
	QVBoxLayout *layTimeSeason = new QVBoxLayout();
	mainLayout->addLayout(layTimeSeason, 1);

	//===============================================================
	// Time group
	QStringList time_vals; //* The command arg stored in property
	time_vals << "real" << "dawn" << "morning" << "noon" << "afternoon" << "dusk" << "evening" << "midnight";
	QStringList time_labels; //* The labels - subject ot transtation
	time_labels << tr("Real time") << tr("Dawn") << tr("Morning") << tr("Noon") << tr("Afternoon") << tr("Dusk") << tr("Evening") << tr("Midnight");

	QGroupBox *grpBoxTime = new QGroupBox(tr("Time"));
	layTimeSeason->addWidget(grpBoxTime);
	QVBoxLayout *layBoxTime = new QVBoxLayout();
	grpBoxTime->setLayout(layBoxTime);
	buttonGroupTime = new QButtonGroup(this);
	buttonGroupTime->setExclusive(true);

	for(int i=0;  i < time_vals.size(); i++){
		QRadioButton *butt = new QRadioButton();
		butt->setText(time_labels.at(i));
		butt->setProperty("value", QVariant(time_vals.at(i)));
		layBoxTime->addWidget(butt);
		buttonGroupTime->addButton(butt, i);
	}
	buttonGroupTime->button(0)->setChecked(true);

	//===============================================================
	// Season group
	QStringList season_vals; //* The command arg stored in property
	season_vals << "winter" << "summer";
	QStringList season_labels; //* The labels - subject ot transtation
	season_labels << tr("Winter") << tr("Summer");

	QGroupBox *grpBoxSeason = new QGroupBox(tr("Season"));
	layTimeSeason->addWidget(grpBoxSeason);
	QVBoxLayout *layBoxSeason = new QVBoxLayout();
	grpBoxSeason->setLayout(layBoxSeason);
	buttonGroupSeason = new QButtonGroup(this);
	buttonGroupSeason->setExclusive(true);
	for(int i=0;  i < season_vals.size(); i++){
		QRadioButton *buttS = new QRadioButton();
		buttS->setText(season_labels.at(i));
		buttS->setProperty("value", QVariant(season_vals.at(i)));
		layBoxSeason->addWidget(buttS);
		buttonGroupSeason->addButton(buttS, i);
	}
	buttonGroupSeason->button(0)->setChecked(true);

	//** Add some stretch to column bottom
	layTimeSeason->addStretch(100);

	//** Add A Spacer in Middle Column
	mainLayout->addStretch(1);


	//===============================================================
	// Right Column with Metar
	//===============================================================
	QVBoxLayout *laymetar = new QVBoxLayout();
	mainLayout->addLayout(laymetar, 2);

	//===============================================================
	// Metar group
	QStringList metar_vals; //* The command arg stored in property
	metar_vals << "none" << "live" << "custom";
	QStringList metar_labels; //* The labels - subject ot transtation
	metar_labels << tr("None") << tr("Fetch METAR live") << tr("Use METAR below");

	QGroupBox *grpBoxMetar = new QGroupBox(tr("METAR"));
	laymetar->addWidget(grpBoxMetar);
	QVBoxLayout *layBoxMetar = new QVBoxLayout();
	grpBoxMetar->setLayout(layBoxMetar);
	buttonGroupMetar = new QButtonGroup(this);
	buttonGroupMetar->setExclusive(true);
	connect(buttonGroupMetar, SIGNAL(buttonClicked(int)), this, SLOT(on_metar_clicked(int)));
	for(int i=0;  i < metar_vals.size(); i++){
		QRadioButton *buttM = new QRadioButton();
		buttM->setText(metar_labels.at(i));
		buttM->setProperty("value", QVariant(metar_vals.at(i)));
		layBoxMetar->addWidget(buttM);
		buttonGroupMetar->addButton(buttM, i);
	}
	buttonGroupMetar->button(0)->setChecked(true);

	txtMetar = new QPlainTextEdit();
	layBoxMetar->addWidget(txtMetar);


}

void TimeWeatherWidget::on_metar_clicked(int idx){
	txtMetar->setEnabled(idx == 2);
	if(idx == 2){
		txtMetar->setFocus();
	}

}

QStringList TimeWeatherWidget::get_args(){
	QStringList args;
	//* Weather/Metar fetch
	QString metar = buttonGroupMetar->checkedButton()->property("value").toString();
	if(metar == "live") {
		//* Enable real weather
		args << QString("--enable-real-weather-fetch");

	}else if(metar == "custom"){
		//* Use metar string ? do we need to parse ?
		args << QString("--metar=").append("\"").append(txtMetar->toPlainText()).append("\"");

	}else{
		args << QString("--disable-real-weather-fetch");
	}
	return args;
}


void TimeWeatherWidget::load_settings(){

	buttonGroupTime->button(mainObject->settings->value("timeofday", 0).toInt())->setChecked(true);

	buttonGroupSeason->button(mainObject->settings->value("season", 0).toInt())->setChecked(true);

	buttonGroupMetar->button(mainObject->settings->value("weather", 0).toInt())->setChecked(true);
	txtMetar->setPlainText(mainObject->settings->value("metar").toString());
	txtMetar->setEnabled(buttonGroupMetar->button(2)->isChecked());

}

void TimeWeatherWidget::save_settings(){

	//* Time
	mainObject->settings->setValue("timeofday", buttonGroupTime->checkedId());
	mainObject->settings->setValue("season", buttonGroupSeason->checkedId());
	//mainObject->settings->setValue("set_time", groupBoxSetTime->isChecked());

	//* Weather
	mainObject->settings->setValue("weather", buttonGroupMetar->checkedId());
	mainObject->settings->setValue("metar", txtMetar->toPlainText());

}
