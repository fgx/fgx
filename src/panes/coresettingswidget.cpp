
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>

#include "panes/coresettingswidget.h"
#include "xwidgets/xgroupvbox.h"

CoreSettingsWidget::CoreSettingsWidget(QWidget *parent) :
    QWidget(parent)
{

	QHBoxLayout *mainLayout = new QHBoxLayout();


	XGroupVBox *grpBoxScreen = new XGroupVBox(tr("Screen Options"));
	mainLayout->addWidget(grpBoxScreen);

	checkBoxFullScreenStartup = new QCheckBox(tr("Full screen at startup"));


}
