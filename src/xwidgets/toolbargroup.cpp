#include "toolbargroup.h"

ToolBarGroup::ToolBarGroup(QWidget *parent) :
    QWidget(parent)
{

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);

    this->lblTitle = new QLabel();
    this->lblTitle->setStyleSheet("background-color: #dddddd; color: #444444; font-size: 8pt; padding: 2px;");
    mainLayout->addWidget(this->lblTitle);

    QWidget *bwid = new QWidget();
    mainLayout->addWidget(bwid);

    bottomLayout = new QHBoxLayout();
    bwid->setLayout(bottomLayout);

}

void ToolBarGroup::setTitle(QString title){
    this->lblTitle->setText(title);
}

void ToolBarGroup::addWidget(QWidget *widget){
    this->bottomLayout->addWidget(widget);
}
