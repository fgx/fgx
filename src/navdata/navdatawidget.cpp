
#include <QtDebug>

#include <QVBoxLayout>
#include <QToolBar>

#include "navdatawidget.h"


#include "xwidgets/toolbargroup.h"



NavDataWidget::NavDataWidget(QWidget *parent) :
    QWidget(parent)
{


    this->navDataModel = new NavDataModel(this);




    QVBoxLayout *mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);

    QToolBar *toolbar = new QToolBar();
    mainLayout->addWidget(toolbar);

    //================================================================================
    ToolBarGroup *tbSearch = new ToolBarGroup();
    toolbar->addWidget(tbSearch);
    tbSearch->lblTitle->setText("Search");

    //==========================================
    //= Search text
    this->txtSearch = new QLineEdit();
    tbSearch->addWidget(this->txtSearch);
    connect(this->txtSearch, SIGNAL(returnPressed()),
            this, SLOT(on_return_pressed())
     );
    connect(this->txtSearch, SIGNAL(textChanged(QString)),
            this, SLOT(on_text_changed(QString))
     );
    this->txtSearch->setText("amm");

    //==========================================
    //= Tree
    this->tree = new QTreeView();
    mainLayout->addWidget(this->tree);
    this->tree->setModel(this->navDataModel);

    this->tree->setRootIsDecorated(false);
    this->tree->setUniformRowHeights(true);
    this->tree->setColumnWidth(NavDataModel::C_NTYPE, 20);
    this->tree->setColumnWidth(NavDataModel::C_IDENT, 100);
    //this->tree->header()->setStretchLastColumn(true);

}

void NavDataWidget::on_search()
{
    qDebug() << "on search";
}
void NavDataWidget::on_return_pressed()
{
     qDebug() << "on return";
     this->navDataModel->fetch( this->txtSearch->text() );
}

void NavDataWidget::on_text_changed(QString txt)
{
     qDebug() << "on text" << txt;
}
