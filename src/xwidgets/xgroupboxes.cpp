#include "xwidgets/xgroupboxes.h"

//======================================================
/* Extended groupBox with a Vertical Layout */
XGroupVBox::XGroupVBox(QString title, QWidget *parent) :
    QGroupBox(parent)
{
	setTitle(title);
	xLayout = new QVBoxLayout();
	setLayout(xLayout);
}

void XGroupVBox::addWidget(QWidget *widget){
	xLayout->addWidget(widget);
}
void XGroupVBox::addWidget(QWidget *widget, int stretch){
	xLayout->addWidget(widget, stretch);
}
void XGroupVBox::addLayout( QBoxLayout *lay){
	xLayout->addLayout(lay);
}

//======================================================
/* Extended groupBox with a Horizontal Layout */
XGroupHBox::XGroupHBox(QString title, QWidget *parent) :
	QGroupBox(parent)
{
	setTitle(title);
	xLayout = new QHBoxLayout();
	setLayout(xLayout);
}

void XGroupHBox::addWidget(QWidget *widget){
	xLayout->addWidget(widget);
}

void XGroupHBox::addWidget(QWidget *widget, int stretch){
	xLayout->addWidget(widget, stretch);
}
void XGroupHBox::addLayout( QBoxLayout *lay){
	xLayout->addLayout(lay);
}

