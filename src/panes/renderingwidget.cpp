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
	//int m = 10;
	//mainLayout->setContentsMargins(0,0,0,0);
	
	
	
	
	//** MiddleLayout
	QHBoxLayout *middleLayout = new QHBoxLayout();
	mainLayout->addLayout(middleLayout);
	middleLayout->setSpacing(10);
	//int mm = 10;
	//middleLayout->setContentsMargins(mm,mm,mm,mm);
	
	
	//====================================================
	//== Left Layout
	//====================================================
	
	QVBoxLayout *leftLayout = new QVBoxLayout();
	leftLayout->setSpacing(10);
	middleLayout->addLayout(leftLayout, 2);
	
	//==================================================================
	//= Screen Options
	
	QHBoxLayout *screenLayout = new QHBoxLayout();
	screenLayout->setSpacing(10);
	leftLayout->addLayout(screenLayout);
	
	XGroupVBox *grpBoxScreen = new XGroupVBox(tr("Screen Options"));
	screenLayout->addWidget(grpBoxScreen);
	grpBoxScreen->setWhatsThis(tr("<b>Screen Options</b><br><br>Set preferred size for the FlightGear window."));
	
	//= Initial Size
	grpBoxScreen->addWidget(new QLabel("Initial Screen Size"));
	comboScreenSize = new QComboBox();
	comboScreenSize->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	comboScreenSize->addItem("800 x 600 (4:3)", "800x600");
	comboScreenSize->addItem("1024 x 768 (4:3)", "1024x768");
	comboScreenSize->addItem("1280 x 720 (16:9)", "1280x720");
	comboScreenSize->addItem("1280 x 800 (16:10)", "1280x800");
	comboScreenSize->addItem("1280 x 960 (4:3)", "1280x960");
	comboScreenSize->addItem("1280 x 1024 (5:4)", "1280x1024");
	comboScreenSize->addItem("1366 x 768 (16:9)", "1366x768");
	comboScreenSize->addItem("1440 x 900 (16:10)", "1440x900");
	comboScreenSize->addItem("1600 x 900 (16:9)", "1600x900");
	comboScreenSize->addItem("1680 x 1050 (16:10)", "1680x1050");
	comboScreenSize->addItem("1920 x 1200 (16:10)", "1920x1200");
	grpBoxScreen->addWidget(comboScreenSize);
	connect(comboScreenSize, SIGNAL(currentIndexChanged(int)),
			this, SLOT(on_screensize())
			);
	
	// set screen size manually, set input mask to 4 digits and numbers only
	QHBoxLayout *screenSizeBox = new QHBoxLayout();
	lineEditScreenSizeWLabel = new QLabel("Width: ");
	lineEditScreenSizeW = new QLineEdit(this);
	lineEditScreenSizeW->setText("");
	lineEditScreenSizeW->setMaxLength(4);
	lineEditScreenSizeW->setInputMask("9999");
	connect(lineEditScreenSizeW, SIGNAL(textChanged(QString)), this, SLOT(on_screensize_changed(QString)) );
	
	lineEditScreenSizeHLabel = new QLabel("Height: ");
	lineEditScreenSizeH = new QLineEdit(this);
	lineEditScreenSizeH->setText("");
	lineEditScreenSizeH->setMaxLength(4);
	lineEditScreenSizeH->setInputMask("9999");
	connect(lineEditScreenSizeH, SIGNAL(textChanged(QString)), this, SLOT(on_screensize_changed(QString)) );
	
	screenSizeBox->addWidget(lineEditScreenSizeWLabel, 1);
	screenSizeBox->addWidget(lineEditScreenSizeW, 1);
	screenSizeBox->addWidget(lineEditScreenSizeHLabel, 1);
	screenSizeBox->addWidget(lineEditScreenSizeH, 1);
	grpBoxScreen->addLayout(screenSizeBox);
	
	//= Full Screen
	checkBoxFullScreenStartup = new QCheckBox(tr("Fullscreen mode"));
	grpBoxScreen->addWidget(checkBoxFullScreenStartup);
	checkBoxFullScreenStartup->setWhatsThis(tr("<b>Full Screen</b><br><br>Will start FlightGear in Full Screen Mode (ESC to cancel mode)."));
	connect(checkBoxFullScreenStartup, SIGNAL(clicked()), this, SLOT(on_fullscreen_changed()) );
	
	//= Disable Splash
	checkBoxDisableSplashScreen = new QCheckBox(tr("Disable Splash Screen"));
	grpBoxScreen->addWidget(checkBoxDisableSplashScreen);
	connect(checkBoxDisableSplashScreen, SIGNAL(clicked()), this, SLOT(on_checkbox_splash_screen()));
	checkBoxDisableSplashScreen->setWhatsThis(tr("<b>Disable Splash Screen</b><br><br>Disables FlightGear startup screen."));
	
	//= Native menubar switch osx
	checkBoxUseNativeMenu = new QCheckBox(tr("Use native menubar (OSX only)"));
	grpBoxScreen->addWidget(checkBoxUseNativeMenu);
	connect(checkBoxUseNativeMenu, SIGNAL(clicked()), this, SLOT(on_checkbox_native_menubar()));
	checkBoxUseNativeMenu->setWhatsThis(tr("<b>Use native menu</b><br><br>Using native menubar in OSX instead of in-window one."));
	
	// switch off for other os
	if(MainObject::runningOs() != OS_MAC){
		checkBoxUseNativeMenu->setEnabled(false);
	}
	
	QVBoxLayout *screenMatLayout = new QVBoxLayout();
	screenMatLayout->setSpacing(10);
	screenLayout->addLayout(screenMatLayout);
	
	//==================================================================
	//= Materials File
	XGroupVBox *grpMaterials = new XGroupVBox(tr("Materials"));
	screenMatLayout->addWidget(grpMaterials);
	grpMaterials->setWhatsThis(tr("<b>Materials</b><br><br>Choose a materials file when you want \
								  to switch to another texture set."));
	
	comboMaterials = new QComboBox();
	comboMaterials->addItem("default", "default");
	comboMaterials->addItem("regions", "regions");
	comboMaterials->addItem("dds", "dds");
	comboMaterials->addItem("custom", "custom");
	connect(comboMaterials, SIGNAL(currentIndexChanged(int)), this, SLOT(set_materials()));
	
	pathMaterials = new QLineEdit("");
	pathMaterials->setFixedSize(QSize(240,20));
	pathMaterials->setDisabled(true);
	connect(pathMaterials, SIGNAL(textChanged(QString)), this, SLOT(set_custom_materials(QString)) );
	
	QHBoxLayout *materialsPathBox = new QHBoxLayout();
	materialsPathBox->addWidget(pathMaterials);
	
	grpMaterials->addWidget(comboMaterials);
	grpMaterials->addLayout(materialsPathBox);
	
	//====================================================
	//== Shader Quality
	
	grpShaderQuality = new XGroupGBox("Shader Quality Level");
	screenMatLayout->addWidget(grpShaderQuality);
	
	int row = 0;
	
	row++;
	//= Shader quality (range 0-5)
	sliderShaderQuality = new QSlider(Qt::Horizontal);
	sliderShaderQuality->setRange(0,5);
	sliderShaderQuality->setTickPosition(QSlider::TicksLeft);
	sliderShaderQuality->setTickInterval(1);
	sliderShaderQuality->setPageStep(6);
	sliderLabelShaderQuality = new QLabel("0");
	sliderLabelShaderQuality->setFixedWidth(10);
	sliderLabelShaderQuality->setAlignment(Qt::AlignRight);
	int sliderShaderQualityPosition = mainObject->X->getx("--prop:/sim/rendering/shaders/quality-level-internal=", true).toInt();
	sliderShaderQuality->setValue(sliderShaderQualityPosition);
	connect(sliderShaderQuality,SIGNAL(valueChanged(int)),this,SLOT(set_shader_quality(int)));
	grpShaderQuality->addWidget(sliderShaderQuality, row, 1, 1, 1);
	grpShaderQuality->addWidget(sliderLabelShaderQuality, row, 2, 1, 1);
	
	row++;
	//= Atmospheric light scattering
	checkBoxSkydome = new QCheckBox(tr("Atmospheric lighting"));
	grpShaderQuality->addWidget(checkBoxSkydome, row, 1, 1, 1);
	connect(checkBoxSkydome, SIGNAL(clicked()), this, SLOT(set_skydome()));
	checkBoxSkydome->setWhatsThis(tr("<b>Atmospheric light scattering</b><br><br>This shader is experimental and will disable other shaders.")); 
	
	
	//====================================================
	//== 3d Clouds
	
	grp3dClouds = new XGroupGBox("3d Clouds");
	grp3dClouds->setCheckable(true);
	leftLayout->addWidget(grp3dClouds);
	
	row = 0;
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
	
	
	//====================================================
	//== Right Layout
	//====================================================
	QVBoxLayout *rightLayout = new QVBoxLayout();
	middleLayout->addLayout(rightLayout, 1);
	
	//====================================================
	//== Anti Aliasing
	
	grpAntiAliasing = new XGroupHBox("Anti-aliasing");
	grpAntiAliasing->setCheckable(true);
	rightLayout->addWidget(grpAntiAliasing);
	
	comboAntiAliasing = new QComboBox();
	comboAntiAliasing->addItem("1", "1");
	comboAntiAliasing->addItem("2", "2");
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
	
	
	//====================================================
	//== Deferred Rendering (Rembrandt)
	
	grpRembrandt = new XGroupGBox("Deferred Rendering (Rembrandt)");
	rightLayout->addWidget(grpRembrandt);
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
	
	
	//== Connect Settings
	connect(this, SIGNAL(setx(QString,bool,QString)), mainObject->X, SLOT(set_option(QString,bool,QString)) );
	connect(mainObject->X, SIGNAL(upx(QString,bool,QString)), this, SLOT(on_upx(QString,bool,QString)));
	
	leftLayout->addStretch(20);
	rightLayout->addStretch(20);
	
	set_materials();
	

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

void RenderingWidget::on_checkbox_native_menubar()
{
	emit setx("--prop:/sim/menubar/native=", checkBoxUseNativeMenu->isChecked(), "1");
}

//=====================================================
void RenderingWidget::on_upx( QString option, bool enabled, QString value)
	{
	Q_UNUSED(enabled);
	QString valueLeft(""), valueRight(""); // locally, for screen geometry values handler only
	
	if(option == "--prop:/sim/rendering/multi-sample-buffers="){
		grpAntiAliasing->setChecked(enabled);
		
	}else if(option == "--prop:/sim/rendering/multi-samples="){
		comboAntiAliasing->setCurrentIndex(comboAntiAliasing->findData(value));
		
	}else if(option == "--prop:/sim/rendering/shaders/quality-level-internal="){
		sliderLabelShaderQuality->setText(value);
		int sliderShaderQualityPosition = mainObject->X->getx("--prop:/sim/rendering/shaders/quality-level-internal=", true).toInt();
		sliderShaderQuality->setValue(sliderShaderQualityPosition);
		
	}else if(option == "--prop:/sim/rendering/shaders/skydome="){
		checkBoxSkydome->setChecked(enabled);
		
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

	}else if(option == "--full-screen"){
		checkBoxFullScreenStartup->setChecked(enabled);
		comboScreenSize->setDisabled(enabled);
		
	}else if(option == "--geometry="){
		comboScreenSize->setCurrentIndex(comboScreenSize->findData(value));
		valueLeft = value.section('x', 0, 0);
		valueRight = value.section('x', 1, 1);
		lineEditScreenSizeW->setText(valueLeft);
		lineEditScreenSizeH->setText(valueRight);
		
	}else if(option == "--disable-splash-screen"){
		checkBoxDisableSplashScreen->setChecked(enabled);
		
	}else if(option == "--prop:/sim/menubar/native="){
		checkBoxUseNativeMenu->setChecked(enabled);
		
	}else if(option == "--materials-file="){
		checkBoxUseNativeMenu->setChecked(enabled);
		
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

//================================================================
// Shader Quality label helpers, put shader quality slider values to the labels

void RenderingWidget::set_shader_quality(int value)
{
	sliderLabelShaderQuality->setText(QString::number(value));
	emit setx("--prop:/sim/rendering/shaders/quality-level-internal=", true, sliderLabelShaderQuality->text());
}

//=====================================
// ScreenSize combobox changed
void RenderingWidget::on_screensize()
{	
	QString value(""), valueLeft(""), valueRight("");
	value = comboScreenSize->itemData(comboScreenSize->currentIndex()).toString();
	valueLeft = value.section('x', 0, 0);
	valueRight = value.section('x', 1, 1);
	lineEditScreenSizeW->setText(valueLeft);
	lineEditScreenSizeH->setText(valueRight);
}

//=====================================
// ScreenSize changed
void RenderingWidget::on_screensize_changed(QString)
{
	emit setx( "--geometry=",
			  true,
			  lineEditScreenSizeW->text().append("x").append(lineEditScreenSizeH->text()));
}

//=====================================
// FullScreen changed
void RenderingWidget::on_fullscreen_changed()
{	
	emit setx( "--enable-fullscreen", checkBoxFullScreenStartup->isChecked(), "");
}


//=====================================
// SplashScreen Changed
void RenderingWidget::on_checkbox_splash_screen()
{
	emit setx("--disable-splash-screen", checkBoxDisableSplashScreen->isChecked(), "");
}

//================================================================
// Set Skydome Shader (Atmospheric Scattering, experimental)
void RenderingWidget::set_skydome()
{
	emit setx("--prop:/sim/rendering/shaders/skydome=", checkBoxSkydome->isChecked(), "1");
}

//================================================================
// Set Materials file
void RenderingWidget::set_materials()
{
	QString value = comboMaterials->itemData(comboMaterials->currentIndex()).toString();
	if (value == "default") {
		pathMaterials->setDisabled(true);
		pathMaterials->setText("Materials/default/materials.xml");
		emit setx("--materials-file=", true, pathMaterials->text());

	}if (value == "regions") {
		pathMaterials->setDisabled(true);
		pathMaterials->setText("Materials/regions/materials.xml");
		emit setx("--materials-file=", true, pathMaterials->text());

	}if (value == "dds") {
		pathMaterials->setDisabled(true);
		pathMaterials->setText("Materials/dds/materials.xml");
		emit setx("--materials-file=", true, pathMaterials->text());

	}if (value == "custom") {
		pathMaterials->setDisabled(false);
		emit setx("--materials-file=", true, pathMaterials->text());
	}
}

void RenderingWidget::set_custom_materials(QString path)
{
	emit setx("--materials-file=", true, path);
}

