
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
	QStringList time_vals; //= are the values
	time_vals << "real" << "dawn" << "morning" << "noon" << "afternoon" << "dusk" << "evening" << "midnight";

	QStringList time_labels; //= are the labels
	time_labels << tr("Real time") << tr("Dawn") << tr("Morning") << tr("Noon") << tr("Afternoon") << tr("Dusk") << tr("Evening") << tr("Midnight");

	XGroupVBox *grpBoxTime = new XGroupVBox(tr("Time"));
	layTimeSeason->addWidget(grpBoxTime);
	buttonGroupTime = new QButtonGroup(this);
	buttonGroupTime->setExclusive(true);
	connect(buttonGroupTime, SIGNAL(buttonClicked(int)),
			this, SLOT(on_time_clicked())
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
	QStringList season_vals; //= values
	season_vals << "winter" << "summer";

	QStringList season_labels; // = labels
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

	//= Add some stretch to column bottom
	layTimeSeason->addStretch(100);

	//= Add A Spacer in Middle Column
	mainLayout->addStretch(1);


	//===============================================================
	// Right Column with Metar
	//===============================================================
	QVBoxLayout *laymetar = new QVBoxLayout();
	mainLayout->addLayout(laymetar, 2);

	//===============================================================
	// Metar group
	QStringList metar_vals; //= values
	metar_vals << "none" << "live" << "custom";
	QStringList metar_labels; //= labels
	metar_labels << tr("None (default weather)") << tr("Fetch METAR data live") << tr("Set static METAR below");

	XGroupVBox *grpBoxMetar = new XGroupVBox(tr("METAR"));
	laymetar->setAlignment(Qt::AlignTop);
	laymetar->addWidget(grpBoxMetar);
	buttonGroupMetar = new QButtonGroup(this);
	buttonGroupMetar->setExclusive(true);
	connect(buttonGroupMetar, SIGNAL(buttonClicked(int)), this, SLOT(on_weather()));

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


	connect(this, SIGNAL(setx(QString,bool,QString)), mainObject->X, SLOT(set_option(QString,bool,QString)) );
	connect(mainObject->X, SIGNAL(upx(QString,bool,QString)), this, SLOT(on_upx(QString,bool,QString)));

}




//== Validate
QString TimeWeatherWidget::validate(){
	if(	buttonGroupMetar->checkedButton()->property("value").toString() == "custom"
		&& txtMetar->toPlainText().length() == 0){

			return QString("No metar text");
	}

	return QString("");
}


//=========================================================================
//== Time clicked
void TimeWeatherWidget::on_time_clicked()
{
	emit setx("--timeofday=", true, buttonGroupTime->checkedButton()->property("value").toString());
}

//== Season Clicked
void TimeWeatherWidget::on_season_clicked(int idx)
{
	emit setx("--season=", true, buttonGroupSeason->checkedButton()->property("value").toString());
	Q_UNUSED(idx);
}

//== Metar Clicked
void TimeWeatherWidget::on_weather()
{
	QString w = buttonGroupMetar->checkedButton()->property("value").toString();
	emit setx("weather", true, w);
	emit setx("--disable-real-weather-fetch", w == "none", "");
	emit setx("--enable-real-weather-fetch", w == "live", "");
	emit setx("--metar=", w == "metar", txtMetar->toPlainText());


}


//=========================================================================
//= Update Settings
void TimeWeatherWidget::on_upx(QString option, bool enabled, QString value)
{
	Q_UNUSED(enabled);
	if(option == "--timeofday="){
		Helpers::select_radio(buttonGroupTime, value);

	}else if(option == "--season="){
		Helpers::select_radio(buttonGroupSeason, value);

	}else if(option == "weather"){
		Helpers::select_radio(buttonGroupMetar, value);
		txtMetar->setEnabled(value == "custom");
		if(txtMetar->isEnabled()){
			txtMetar->setFocus();
		}

	}else if(option == "--metar="){
		txtMetar->setPlainText(value);
	}

}
