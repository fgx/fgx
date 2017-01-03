
#include "xstatusbar.h"

XStatusBar::XStatusBar(QWidget *parent) :
    QWidget(parent)
{

    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    statusBar = new QStatusBar(this);
    statusBar->setSizeGripEnabled(false);
    mainLayout->addWidget(statusBar, 5);

    progressBar = new QProgressBar(this);
    progressBar->setTextVisible(false);
    mainLayout->addWidget(progressBar, 1);

    buttRefresh = new QToolButton();
    buttRefresh->setAutoRaise(true);
    buttRefresh->setIcon(QIcon(":/icon/refresh"));
    buttRefresh->setToolButtonStyle(Qt::ToolButtonIconOnly);
    mainLayout->addWidget(buttRefresh, 0);
    connect(buttRefresh, SIGNAL(clicked(bool)), this, SLOT(on_refresh_clicked()));

}
void XStatusBar::on_refresh_clicked(){
    emit refresh();
}

void XStatusBar::set_busy(bool state, QString mess){

    if(state){
        progressBar->setRange(0,0);
        progressBar->setVisible(true);
        statusBar->showMessage(mess, 0);
        return;
    }
    progressBar->setRange(0, 100);
    progressBar->setVisible(false);
    statusBar->showMessage(mess, 2000);
}
