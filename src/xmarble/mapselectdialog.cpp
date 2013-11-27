
#include <QVBoxLayout>

#include "mapselectdialog.h"

MapSelectDialog::MapSelectDialog(MainObject *mob, QWidget *parent) :
    QDialog(parent)
{

    this->mainObject = mob;

    this->actionGroup = new QActionGroup(this);
    this->actionGroup->setExclusive(false);
    connect(this->actionGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(on_action(QAction*))
    );

    this->setWindowFlags(Qt::Popup);
    //this->setWindowModality(QtCore.Qt.ApplicationModal)

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);

    QLabel *lbl = new QLabel();
    lbl->setStyleSheet("background-color: white; color: #009900; font-weight: bold;");
    lbl->setText("Select Map");
    mainLayout->addWidget(lbl);


    //=====================================================
    //== Tree
    this->tree = new QTreeWidget();
    mainLayout->addWidget(this->tree);

    this->tree->setRootIsDecorated(false);
    this->tree->setUniformRowHeights(false);

    this->tree->setColumnWidth(MapViewsModel::C_LAT, 50);
    this->tree->setColumnWidth(MapViewsModel::C_LON, 50);
    this->tree->setColumnWidth(MapViewsModel::C_ZOOM, 50);

    bool hidd = false;
    this->tree->setColumnHidden(MapViewsModel::C_LAT, hidd);
    this->tree->setColumnHidden(MapViewsModel::C_LON, hidd);
    this->tree->setColumnHidden(MapViewsModel::C_ZOOM, hidd);

    this->setMinimumWidth(300);
    this->setMinimumHeight(600);

    this->load_tree();
}

void MapSelectDialog::load_tree()
{
    MapViewsModel *model = this->mainObject->mapViewsModel ;
    QAction *a;
    this->tree->clear();
    for(int idx=0; idx < model->rowCount(); idx++)
    {
        QString view = model->item(idx, MapViewsModel::C_VIEW)->text();
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(MapViewsModel::C_VIEW,    view);
        item->setText(MapViewsModel::C_LAT,     model->item(idx, MapViewsModel::C_LAT)->text());
        item->setText(MapViewsModel::C_LON,     model->item(idx, MapViewsModel::C_LON)->text());
        item->setText(MapViewsModel::C_ZOOM,    model->item(idx, MapViewsModel::C_ZOOM)->text());
        this->tree->addTopLevelItem(item);

        QToolButton *buttOpen = new QToolButton();
        buttOpen->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        buttOpen->setPopupMode(QToolButton::InstantPopup);
        buttOpen->setAutoRaise(true);
        buttOpen->setText( view );
        buttOpen->setIcon(QIcon(":/micon/map"));
        this->tree->setItemWidget(item, MapViewsModel::C_BUTT, buttOpen);

        QMenu *menu = new QMenu();
        buttOpen->setMenu(menu);

        a = menu->addAction("Open in this tab");
        a->setProperty("action","this_tab");
        a->setProperty("view", view);
        this->actionGroup->addAction(a);

        a = menu->addAction("Open in new tab");
        a->setProperty("action","new_tab");
        a->setProperty("view", view);
        this->actionGroup->addAction(a);

        menu->addSeparator();

        a = menu->addAction("Remove this view");
        a->setProperty("action","remove");
        a->setProperty("view", view);
        this->actionGroup->addAction(a);
   }
}

void MapSelectDialog::on_action(QAction *act)
{
    QString a = act->property("action").toString();
    QString v = act->property("view").toString();
    if (a == "remove"){
        QList<QStandardItem *> fitems = this->mainObject->mapViewsModel->findItems(v, Qt::MatchExactly, MapViewsModel::C_VIEW);
        qDebug() << fitems.count();
        this->mainObject->mapViewsModel->remove_view(v);
        this->load_tree();
    }
    emit open_map_view(a, v);

}
