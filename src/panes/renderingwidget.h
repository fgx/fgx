// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // renderingwidget.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef RENDERINGWIDGET_H
#define RENDERINGWIDGET_H


#include <QtCore/QString>
#include <QtCore/QMap>


#include <QtGui/QWidget>
#include <QtGui/QButtonGroup>
#include <QtGui/QPlainTextEdit>


#include "xobjects/mainobject.h"
#include "xwidgets/xgroupboxes.h"

class MainObject;


class RenderingWidget : public QWidget
{
Q_OBJECT
public:
	explicit RenderingWidget(MainObject *mOb, QWidget *parent = 0);

	MainObject *mainObject;
	
	XGroupHBox *grpAntiAliasing;
	QComboBox *comboAntiAliasing;
	
	XGroupGBox *grpShaderQuality;
	QSlider *sliderShaderQuality;
	QLabel *sliderLabelShaderQuality;
	
	XGroupGBox *grpRembrandt;
	QLabel *labelMapsize;
	QLabel *labelShadowQuality;
	QComboBox *comboShadowMapsize;
	QComboBox *comboShadowQuality;
	
	XGroupGBox *grp3dClouds;
	QLabel *labelVisibility;
	QSlider *sliderVisibility;
	QLabel *sliderLabelVisValue;
	QLabel *labelDensity;
	QSlider *sliderDensity;
	QLabel *sliderLabelDenValue;
	
	XGroupHBox *grpBoxScreen;
	QComboBox *comboScreenSize;
	QCheckBox *checkBoxDisableSplashScreen;
	QCheckBox *checkBoxFullScreenStartup;
	QHBoxLayout *screenSizeBox;
	QLineEdit *lineEditScreenSizeW;
	QLineEdit *lineEditScreenSizeH;
	QLabel *lineEditScreenSizeWLabel;
	QLabel *lineEditScreenSizeHLabel;
	QCheckBox *checkBoxUseNativeMenu;
	
	XGroupHBox *grpMaterials;
	QComboBox *comboMaterials;
	QLineEdit *pathMaterials;


signals:
	void setx(QString option, bool enabled, QString value);

public slots:
	
	void set_anti_aliasing();
	void set_anti_aliasing_strength();
	
	void set_shader_quality(int value);
	
	void set_materials();
	void set_custom_materials(QString path);
	
	void set_rembrandt();
	
	void set_3dclouds_enabled();
	void set_vis_value(int value);
	void set_den_value(int value);
	
	void on_screensize();
	void on_fullscreen_changed();
	void on_screensize_changed(QString);
	void on_checkbox_splash_screen();
	void on_checkbox_native_menubar();
	
	void on_upx(QString option, bool enabled, QString value);

};

#endif // RENDERINGWIDGET_H
