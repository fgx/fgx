#include "xwidgets/xgroupboxes.h"



QString MAJOR_STYLE("QGroupBox{ font-weight: bold; color: #444444; }");
QString MINOR_STYLE("QGroupBox{ font-weight: normal; color: #666666; }");



//======================================================
/* Extended groupBox with a Vertical Layout */
XGroupVBox::XGroupVBox(QString title, QWidget *parent) :
    QGroupBox(parent)
{
	setTitle(title);
	xLayout = new QVBoxLayout();
	setLayout(xLayout);
	set_major(false);
}
void XGroupVBox::set_major(bool maj){
	setStyleSheet(maj ? MAJOR_STYLE : MINOR_STYLE);
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
	set_major(false);
}
void XGroupHBox::set_major(bool maj){
	setStyleSheet(maj ? MAJOR_STYLE : MINOR_STYLE);
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




//======================================================
/* Extended groupBox with a Grid Layout */
XGroupGBox::XGroupGBox(QString title, QWidget *parent) :
	QGroupBox(parent)
{
	setTitle(title);
	gridLayout = new QGridLayout();
	setLayout(gridLayout);


}

void XGroupGBox::addWidget(QWidget *widget, int row, int col, int row_span,  int col_span){
	gridLayout->addWidget(widget, row, col, row_span, col_span);
}
