
#include <QVBoxLayout>

#include "mapselectdialog.h"

MapSelectDialog::MapSelectDialog(MainObject *mob, QWidget *parent) :
    QDialog(parent)
{

    this->mainObject = mob;

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
    this->tree = new QTreeView();
    mainLayout->addWidget(this->tree);
    this->tree->setModel(this->mainObject->mapViewsModel);

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

    connect(this->tree, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(on_tree_double_clicked(QModelIndex))
    );

}

void MapSelectDialog::on_tree_double_clicked(QModelIndex)
{

}
