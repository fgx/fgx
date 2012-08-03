// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // renderingwidget.cpp
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QRadioButton>

#include "renderingwidget.h"
#include "xwidgets/xgroupboxes.h"
#include "utilities/helpers.h"

RenderingWidget::RenderingWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{
	mainObject = mOb;
	
    //* Main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setSpacing(0);
	int m = 10;
	mainLayout->setContentsMargins(m,m,m,m);
	
	
	
	
	//** MiddleLayout
	QHBoxLayout *middleLayout = new QHBoxLayout();
	mainLayout->addLayout(middleLayout);
	middleLayout->setSpacing(10);
	int mm = 10;
	middleLayout->setContentsMargins(mm,mm,mm,mm);
	
	//====================================================
	//== Left Layout
	//====================================================
	QVBoxLayout *leftLayout = new QVBoxLayout();
	middleLayout->addLayout(leftLayout, 1);
	
	//====================================================
	//== Anti Aliasing
	
	grpAntiAliasing = new XGroupHBox("Anti-aliasing");
	grpAntiAliasing->setCheckable(true);
	leftLayout->addWidget(grpAntiAliasing);
	
	comboAntiAliasing = new QComboBox();
	comboAntiAliasing->addItem("1", "1");
	comboAntiAliasing->addItem("2", "2");
	comboAntiAliasing->addItem("3", "3");
	comboAntiAliasing->addItem("4 (default)", "4");
	comboAntiAliasing->addItem("5", "5");
	comboAntiAliasing->addItem("6", "6");
	comboAntiAliasing->addItem("7", "7");
	comboAntiAliasing->addItem("8", "8");
	comboAntiAliasing->setEditable(true);
	grpAntiAliasing->addWidget(comboAntiAliasing);
	
	connect(grpAntiAliasing, SIGNAL(toggled(bool)), this, SLOT(set_anti_aliasing()));
	connect(comboAntiAliasing, SIGNAL(currentIndexChanged(int)), this, SLOT(set_anti_aliasing_strength()));
	
	//====================================================
	//== Shader Quality
	
	grpShaderQuality = new XGroupHBox("Shader Quality Level");
	leftLayout->addWidget(grpShaderQuality);
	
	comboShaderQuality = new QComboBox();
	comboShaderQuality->addItem("0", "0");
	comboShaderQuality->addItem("1", "1");
	comboShaderQuality->addItem("2", "2");
	comboShaderQuality->addItem("3", "3");
	comboShaderQuality->addItem("4", "4");
	comboShaderQuality->addItem("5", "5");
	grpShaderQuality->addWidget(comboShaderQuality);
	
	connect(comboShaderQuality, SIGNAL(currentIndexChanged(int)), this, SLOT(set_shader_quality()));
	
	//====================================================
	//== Deferred Rendering (Rembrandt)
	
	grpRembrandt = new XGroupGBox("Defered Rendering (Rembrandt)");
	leftLayout->addWidget(grpRembrandt);
	grpRembrandt->setCheckable(true);
	
	int rrow = 0;
	labelMapsize = new QLabel("Graphics card memory consumption");
	grpRembrandt->addWidget(labelMapsize, rrow, 1, 1, 1);
	
	rrow++;
	comboShadowMapsize = new QComboBox();
	//comboShadowMapsize->setFixedWidth(100);
	comboShadowMapsize->addItem("1024", "1024");
	comboShadowMapsize->addItem("2048", "2048");
	comboShadowMapsize->addItem("4096", "4096");
	comboShadowMapsize->addItem("8192", "8192");
	grpRembrandt->addWidget(comboShadowMapsize, rrow, 1, 1, 1);
	
	rrow++;
	labelShadowQuality = new QLabel("Rendering Quality");
	grpRembrandt->addWidget(labelShadowQuality, rrow, 1, 1, 1);
	
	rrow++;
	comboShadowQuality = new QComboBox();
	//comboShadowQuality->setFixedWidth(100);
	comboShadowQuality->addItem("1", "1");
	comboShadowQuality->addItem("2", "2");
	comboShadowQuality->addItem("3", "3");
	comboShadowQuality->addItem("4", "4");
	grpRembrandt->addWidget(comboShadowQuality, rrow, 1, 1, 1);
	
	connect(grpRembrandt, SIGNAL(toggled(bool)), this, SLOT(set_rembrandt()));
	connect(comboShadowMapsize, SIGNAL(currentIndexChanged(int)), this, SLOT(set_rembrandt()));
	connect(comboShadowQuality, SIGNAL(currentIndexChanged(int)), this, SLOT(set_rembrandt()));
	
	
	//====================================================
	//== Right Layout
	//====================================================
	
	QVBoxLayout *rightLayout = new QVBoxLayout();
	rightLayout->setSpacing(20);
	middleLayout->addLayout(rightLayout, 2);
	
	//====================================================
	//== 3d Clouds
	
	grp3dClouds = new XGroupGBox("3d Clouds");
	grp3dClouds->setCheckable(true);
	rightLayout->addWidget(grp3dClouds);
	
	int row = 0;
	labelVisibility = new QLabel("Visibility (meters)");
	grp3dClouds->addWidget(labelVisibility, row, 1, 1, 1);
	
	row++;
	sliderVisibility = new QSlider(Qt::Horizontal);
	sliderVisibility->setRange(1,90);
	sliderVisibility->setTickPosition(QSlider::TicksLeft);
	sliderVisibility->setTickInterval(1);
	sliderVisibility->setPageStep(9);
	int sliderVisibilityPosition = mainObject->X->getx("--prop:/sim/rendering/clouds3d-vis-range=", true).toInt()/500;
	sliderVisibility->setValue(sliderVisibilityPosition);
	grp3dClouds->addWidget(sliderVisibility, row, 1, 1, 1);
	sliderLabelVisValue = new QLabel("0");
	connect(sliderVisibility,SIGNAL(valueChanged(int)),this,SLOT(set_vis_value(int)));
	grp3dClouds->addWidget(sliderLabelVisValue, row, 2, 1, 1);
	
	row++;
	labelDensity = new QLabel("Density (0.0 - 1.0)");
	grp3dClouds->addWidget(labelDensity, row, 1, 1, 1);
	
	row++;
	sliderDensity = new QSlider(Qt::Horizontal);
	sliderDensity->setRange(0,100);
	sliderDensity->setTickPosition(QSlider::TicksLeft);
	sliderDensity->setTickInterval(10);
	sliderDensity->setPageStep(10);
	double sliderDensityPosition = mainObject->X->getx("--prop:/sim/rendering/clouds3d-density=", true).toDouble()*100.0;
	int sliderDensityPositionInt = static_cast<int>(sliderDensityPosition);
	sliderDensity->setValue(sliderDensityPositionInt);
	grp3dClouds->addWidget(sliderDensity, row, 1, 1, 1);
	sliderLabelDenValue = new QLabel("0");
	sliderLabelDenValue->setFixedWidth(50);
	connect(sliderDensity,SIGNAL(valueChanged(int)),this,SLOT(set_den_value(int)));
	grp3dClouds->addWidget(sliderLabelDenValue, row, 2, 1, 1);
	
	connect(grp3dClouds, SIGNAL(toggled(bool)), this, SLOT(set_3dclouds_enabled()));
	
	
	//== Connect Settings
	connect(this, SIGNAL(setx(QString,bool,QString)), mainObject->X, SLOT(set_option(QString,bool,QString)) );
	connect(mainObject->X, SIGNAL(upx(QString,bool,QString)), this, SLOT(on_upx(QString,bool,QString)));
	
	leftLayout->addStretch(10);
	rightLayout->addStretch(10);
	

}

	//================================================================
	// Emit rendering settings

	void RenderingWidget::set_anti_aliasing()
	{
	emit setx("--prop:/sim/rendering/multi-sample-buffers=", grpAntiAliasing->isChecked(), "1");
	}

	void RenderingWidget::set_anti_aliasing_strength()
	{
	emit setx("--prop:/sim/rendering/multi-samples=", grpAntiAliasing->isChecked(), comboAntiAliasing->currentText());
	}

	void RenderingWidget::set_shader_quality()
	{
	emit setx("--prop:/sim/rendering/shaders/quality-level-internal=", true, comboShaderQuality->currentText());
	}
	
	void RenderingWidget::set_rembrandt()
	{
	emit setx("--prop:/sim/rendering/rembrandt/enabled=", grpRembrandt->isChecked(), "1");
	emit setx("--prop:/sim/rendering/shadows/map-size=", grpRembrandt->isChecked(), comboShadowMapsize->currentText());
	emit setx("--prop:/sim/rendering/shadows/num-cascades=", grpRembrandt->isChecked(), comboShadowQuality->currentText());
	}

	void RenderingWidget::set_3dclouds_enabled()
	{
	emit setx("--prop:/sim/rendering/clouds3d-enable=", grp3dClouds->isChecked(), "1");
	emit setx("--prop:/sim/rendering/clouds3d-vis-range=", grp3dClouds->isChecked(), sliderLabelVisValue->text());
	emit setx("--prop:/sim/rendering/clouds3d-density=", grp3dClouds->isChecked(), sliderLabelDenValue->text());
	}

	//=====================================================
	void RenderingWidget::on_upx( QString option, bool enabled, QString value)
	{
	Q_UNUSED(enabled);
	
	if(option == "--prop:/sim/rendering/multi-sample-buffers="){
		grpAntiAliasing->setChecked(enabled);
		
	}else if(option == "--prop:/sim/rendering/multi-samples="){
		comboAntiAliasing->setCurrentIndex(comboAntiAliasing->findData(value));
		
	}else if(option == "--prop:/sim/rendering/shaders/quality-level-internal="){
		comboShaderQuality->setCurrentIndex(comboShaderQuality->findData(value));
		
	}else if(option == "--prop:/sim/rendering/rembrandt/enabled="){
		grpRembrandt->setChecked(enabled);
		
	}else if(option == "--prop:/sim/rendering/shadows/map-size="){
		comboShadowMapsize->setCurrentIndex(comboShadowMapsize->findData(value));
		
	}else if(option == "--prop:/sim/rendering/shadows/num-cascades="){
		comboShadowQuality->setCurrentIndex(comboShadowQuality->findData(value));
		
	}else if(option == "--prop:/sim/rendering/clouds3d-enable="){
		grp3dClouds->setChecked(enabled);

	}else if(option == "--prop:/sim/rendering/clouds3d-vis-range="){
		sliderLabelVisValue->setText(value);

	}else if(option == "--prop:/sim/rendering/clouds3d-density="){
		sliderLabelDenValue->setText(value);
	}
		
	}


	//================================================================
	// Label helpers, put 3d clouds slider values to the labels

	void RenderingWidget::set_vis_value(int value)
	{
	sliderLabelVisValue->setText(QString::number(value*500) );
	emit setx("--prop:/sim/rendering/clouds3d-vis-range=", true, sliderLabelVisValue->text());
		
	}

	void RenderingWidget::set_den_value(int value)
	{
	sliderLabelDenValue->setText(QString::number(value*0.01) );
	emit setx("--prop:/sim/rendering/clouds3d-density=", true, sliderLabelDenValue->text());
	}


