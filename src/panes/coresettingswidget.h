#ifndef CORESETTINGSWIDGET_H
#define CORESETTINGSWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>

class CoreSettingsWidget : public QWidget
{
Q_OBJECT
public:
	explicit CoreSettingsWidget(QWidget *parent = 0);

	QComboBox *comboScreenSize;
	QCheckBox *checkBoxSplashScreen;
	QCheckBox *checkBoxFullScreenStartup;
	QCheckBox *checkBoxEnableAutoCoordination;
	QCheckBox *checkBoxShowMpMap;

signals:

public slots:

};

#endif // CORESETTINGSWIDGET_H
