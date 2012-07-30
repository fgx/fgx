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
	mainLayout->setContentsMargins(m,m,m,300);
	
	
	
	
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
	comboAntiAliasing->addItem("2 (default)", "2");
	comboAntiAliasing->addItem("3", "3");
	comboAntiAliasing->addItem("4", "4");
	comboAntiAliasing->addItem("5", "5");
	comboAntiAliasing->addItem("6", "6");
	comboAntiAliasing->addItem("7", "7");
	comboAntiAliasing->addItem("8", "8");
	comboAntiAliasing->setEditable(true);
	grpAntiAliasing->addWidget(comboAntiAliasing);
	
	connect(grpAntiAliasing, SIGNAL(toggled(bool)), this, SLOT(set_anti_aliasing()));
	connect(comboAntiAliasing, SIGNAL(currentIndexChanged(int)), this, SLOT(set_anti_aliasing_strength()));
	
	grpAntiAliasing->xLayout->addStretch(20);
	
	//====================================================
	//== Shader Quality
	
	grpShaderQuality = new XGroupHBox("Shader Quality Level");
	leftLayout->addWidget(grpShaderQuality);
	
	comboShaderQuality = new QComboBox();
	comboShaderQuality->addItem("0", "0");
	comboShaderQuality->addItem("1", "1");
	comboShaderQuality->addItem("2", "2");
	comboShaderQuality->addItem("3 (default)", "3");
	comboShaderQuality->addItem("4", "4");
	comboShaderQuality->addItem("5", "5");
	grpShaderQuality->addWidget(comboShaderQuality);
	
	connect(comboShaderQuality, SIGNAL(currentIndexChanged(int)), this, SLOT(set_shader_quality()));

	grpShaderQuality->xLayout->addStretch(20);
	
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
	labelVisibility = new QLabel("Visibility");
	grp3dClouds->addWidget(labelVisibility, row, 1, 1, 1);
	
	row++;
	sliderVisibility = new QSlider(Qt::Horizontal);
	sliderVisibility->setRange(0,90); // x 500
	sliderVisibility->setTickPosition(QSlider::TicksLeft);
	sliderVisibility->setTickInterval(1);
	sliderVisibility->setPageStep(1);
	grp3dClouds->addWidget(sliderVisibility, row, 1, 1, 1);
	
	row++;
	labelDensity = new QLabel("Density");
	grp3dClouds->addWidget(labelDensity, row, 1, 1, 1);
	
	row++;
	sliderDensity = new QSlider(Qt::Horizontal);
	sliderDensity->setRange(0,100); // : 100
	sliderDensity->setTickPosition(QSlider::TicksLeft);
	sliderDensity->setTickInterval(1);
	grp3dClouds->addWidget(sliderDensity, row, 1, 1, 1);
	
	//connect(grpAntiAliasing, SIGNAL(toggled(bool)), this, SLOT(set_anti_aliasing()));
	//connect(comboAntiAliasing, SIGNAL(currentIndexChanged(int)), this, SLOT(set_anti_aliasing_strength()));
	

}

	//================================================================
	// Emit rendering settings

	void RenderingWidget::set_anti_aliasing()
	{
	emit setx("--prop:/sim/rendering/multi-sample-buffers=", true, "1");
	}

	void RenderingWidget::set_anti_aliasing_strength()
	{
	emit setx("--prop:/sim/rendering/multi-samples=", true, comboAntiAliasing->currentText());
	}

	void RenderingWidget::set_shader_quality()
	{
	emit setx("--prop:/sim/rendering/shaders/quality-level=", true, comboShaderQuality->currentText());
	}

