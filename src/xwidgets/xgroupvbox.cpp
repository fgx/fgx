#include "xgroupvbox.h"

/* Extended groupBox with a Vertical Layout */

XGroupVBox::XGroupVBox(QString title, QWidget *parent) :
    QGroupBox(parent)
{
	setTitle(title);
	xLayout = new QVBoxLayout();
}

void XGroupVBox::addWidget(QWidget *widget){
	xLayout->addWidget(widget);
}
