#include "toolbargroup.h"

ToolBarGroup::ToolBarGroup(QWidget *parent) :
    QWidget(parent)
{



            //self.buttonGroup = None
            //self.is_group = False
            //self.group_var = None
            //self.callback = None
            //self.show_icons = True
            //self.bg_color = '#333333'

            // Main Layout
            this->mainLayout = new QVBoxLayout();
            this->mainLayout->setContentsMargins( 0, 0, 0, 0 );
            this->mainLayout->setSpacing( 0 );
            this->setLayout( this->mainLayout );

            // Label
            this->lblTitle = new QLabel();
            QString lbl_sty("background: #eeeeee; ");
            lbl_sty.append(" color: #333333; font-size: 7pt; padding: 1px;");
            this->lblTitle->setStyleSheet( lbl_sty );
            this->lblTitle->setAlignment( Qt::AlignCenter );
            this->mainLayout->addWidget( this->lblTitle );

            // Toolbar
            this->toolbar = new QToolBar();
            this->toolbar->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
            this->toolbar->setFixedHeight( 30 );

            this->mainLayout->addWidget( this->toolbar );


}

void ToolBarGroup::addWidget(QWidget *wid)
{
    this->toolbar->addWidget(wid);
}
