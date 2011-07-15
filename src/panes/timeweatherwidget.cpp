
//#include <QDebug>

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QRadioButton>

#include "timeweatherwidget.h"
#include "xwidgets/xgroupboxes.h"
#include "utilities/helpers.h"

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

	XGroupVBox *grpBoxTime = new XGroupVBox(tr("Time"));
	layTimeSeason->addWidget(grpBoxTime);
	buttonGroupTime = new QButtonGroup(this);
	buttonGroupTime->setExclusive(true);
	connect(buttonGroupTime, SIGNAL(buttonClicked(int)),
			this, SLOT(on_time_clicked(int))
	);

	for(int i=0;  i < time_vals.size(); i++){
		QRadioButton *butt = new QRadioButton();
		butt->setText(time_labels.at(i));
		butt->setProperty("value", QVariant(time_vals.at(i)));
		grpBoxTime->addWidget(butt);
		buttonGroupTime->addButton(butt, i);
	}
	buttonGroupTime->button(0)->setChecked(true);

	//===============================================================
	// Season group
	QStringList season_vals; //* The command arg stored in property
	season_vals << "winter" << "summer";
	QStringList season_labels; //* The labels - subject ot transtation
	season_labels << tr("Winter") << tr("Summer");

	XGroupVBox *grpBoxSeason = new XGroupVBox(tr("Season"));
	layTimeSeason->addWidget(grpBoxSeason);
	buttonGroupSeason = new QButtonGroup(this);
	buttonGroupSeason->setExclusive(true);
	connect(buttonGroupSeason, SIGNAL(buttonClicked(int)),
			this, SLOT(on_season_clicked(int))
	);

	for(int i=0;  i < season_vals.size(); i++){
		QRadioButton *buttS = new QRadioButton();
		buttS->setText(season_labels.at(i));
		buttS->setProperty("value", QVariant(season_vals.at(i).toLower()));
		grpBoxSeason->addWidget(buttS);
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
	metar_labels << tr("None (default weather)") << tr("Fetch METAR data live") << tr("Set static METAR below");

	XGroupVBox *grpBoxMetar = new XGroupVBox(tr("METAR"));
	laymetar->setAlignment(Qt::AlignTop);
	laymetar->addWidget(grpBoxMetar);
	buttonGroupMetar = new QButtonGroup(this);
	buttonGroupMetar->setExclusive(true);
	connect(buttonGroupMetar, SIGNAL(buttonClicked(int)), this, SLOT(on_metar_clicked()));

	for(int i=0;  i < metar_vals.size(); i++){
		QRadioButton *buttM = new QRadioButton();
		buttM->setText(metar_labels.at(i));
		buttM->setProperty("value", QVariant(metar_vals.at(i)));
		grpBoxMetar->addWidget(buttM);
		buttonGroupMetar->addButton(buttM, i);
	}
	buttonGroupMetar->button(0)->setChecked(true);

	txtMetar = new QPlainTextEdit();
	txtMetar->setMaximumHeight(80);
	grpBoxMetar->addWidget(txtMetar);
	laymetar->addStretch(20);


	connect(this, SIGNAL(setx(QString,bool,QString)), mainObject->S, SLOT(set_option(QString,bool,QString)) );
	connect(mainObject->S, SIGNAL(upx(QString,bool,QString)), this, SLOT(on_upx(QString,bool,QString)));

}




//== Validate
QString TimeWeatherWidget::validate(){
	if(	buttonGroupMetar->checkedButton()->property("value").toString() == "custom"
		&& txtMetar->toPlainText().length() == 0){

			return QString("No metar text");
	}

	return QString("");
}


//== Load Settings
void TimeWeatherWidget::load_settings(){

	return;
	Helpers::select_radio(buttonGroupTime, mainObject->settings->value("timeofday", "").toString());

	Helpers::select_radio(buttonGroupSeason, mainObject->settings->value("season", "").toString());



	Helpers::select_radio(buttonGroupMetar, mainObject->settings->value("weather", "").toString());
	txtMetar->setPlainText(mainObject->settings->value("metar").toString());
	txtMetar->setEnabled(buttonGroupMetar->button(2)->isChecked());

}


//== Save Settings
void TimeWeatherWidget::save_settings(){

	return;
	//* Time
	mainObject->settings->setValue("timeofday", buttonGroupTime->checkedButton()->property("value").toString());
	mainObject->settings->setValue("season", buttonGroupSeason->checkedButton()->property("value").toString());

	//* Weather
	mainObject->settings->setValue("weather", buttonGroupMetar->checkedButton()->property("value").toString());
	mainObject->settings->setValue("metar", txtMetar->toPlainText());
}

//=========================================================================
//== Time clicked
void TimeWeatherWidget::on_time_clicked(int idx)
{
	setx("--timeofday=", true, buttonGroupTime->checkedButton()->property("value").toString());
}

//= Season Clicked
void TimeWeatherWidget::on_season_clicked(int idx)
{
	setx("--season=", true, buttonGroupSeason->checkedButton()->property("value").toString());
}

//= Metar Clicked
void TimeWeatherWidget::on_metar_clicked(){
	//qDebug() << buttonGroupMetar->checkedButton()->property("weather").toString();
	QString w = buttonGroupMetar->checkedButton()->property("weather").toString();
	emit setx("weather", true, "");
	emit setx("--disable-real-weather-fetch", w == "none", "");
	emit setx("--enable-real-weather-fetch", w == "live", "");
	emit setx("--metar=", w == "metar", txtMetar->toPlainText());


	//if(w == "none"){
	//
	//	txtMetar->setEnabled(true);
	//	txtMetar->setFocus();
	//}else{
	//	txtMetar->setEnabled(false);
	//}
}


//=========================================================================
//= Update Settings
void TimeWeatherWidget::on_upx(QString option, bool enabled, QString value)
{

	if(option == "--timeofday="){
		Helpers::select_radio(buttonGroupTime, value);


	}else if(option == "--season="){
		Helpers::select_radio(buttonGroupSeason, value);
	}


	//=== Weather
	/*
	QString weather_method = settings->value("weather").toString();
	if(weather_method == "live") {
		//= real weather
		args << QString("--enable-real-weather-fetch");

	}else if(weather_method == "custom"){
		//= custom metar
		args << QString("--metar=").append("\"").append(settings->value("metar").toString()).append("\"");

	}else{
		//= no weather
		args << QString("--disable-real-weather-fetch");
	}
	*/
}
