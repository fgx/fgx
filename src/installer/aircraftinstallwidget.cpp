

#include <QtDebug>

#include <QTimer>
#include <QList>

#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QToolBar>
#include <QToolButton>
#include <QHeaderView>
#include <QPushButton>

#include "xobjects/xsettings.h"
#include "installer/aircraftinstallwidget.h"

AircraftInstallWidget::AircraftInstallWidget(QMainWindow *parent) :
    QMainWindow(parent)
{

    //==================================
    //= Setup Models
    model = new QStandardItemModel(this);
    QStringList hLabels;
    hLabels << "Dir" << "Aero" << "Description" <<  "Author" <<  "Size" << "Hash" << "Updated" << "Download" << "Status";
    model->setHorizontalHeaderLabels(hLabels);

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);


    //==================================
    //= NetworkManaget
    netMan = new QNetworkAccessManager(this);
    connect(this->netMan, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(on_request_finished(QNetworkReply*))
            );


    QWidget *mainWidget = new QWidget();
    this->setCentralWidget(mainWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    mainWidget->setLayout(mainLayout);


    /*
    QToolBar *toolbar = new QToolBar();
    mainLayout->addWidget(toolbar);

    toolbar->addAction( QIcon(":/icon/rel_add"), "Add", this,  SLOT(on_add()) );
    actEdit = toolbar->addAction( QIcon(":/icon/rel_edit"), "Edit", this,  SLOT(on_edit()) );
    //actEdit->setDisabled(true);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    */
    QString tree_style("QTreeView::item:hover {background-color: #dddddd;}");

    tree = new QTreeView();
    mainLayout->addWidget(tree);

    tree->setModel(this->model);
    tree->setUniformRowHeights(true);
    tree->setAlternatingRowColors(true);
    tree->setRootIsDecorated(false);
    tree->setStyleSheet(tree_style);


    /* tree->headerItem()->setText(C_AERO, "Aero");
    tree->headerItem()->setText(C_DESCRIPTION, "Description");
    tree->headerItem()->setText(C_STATUS, "Status");
    tree->headerItem()->setText(C_ZIP_SIZE, "Size");
    tree->headerItem()->setText(C_ZIP_MD5, "Hash");
    tree->headerItem()->setText(C_ZIP_FILE, "Zip File");
    tree->headerItem()->setText(C_ID, "ID");
    tree->headerItem()->setText(C_AUTHOR, "Author");
    tree->headerItem()->setText(C_SUB_DIR, "Dir");
    tree->headerItem()->setText(C_XML_SET, "XML File");
    tree->headerItem()->setText(C_LAST_UPDATED, "Updated");
    tree->headerItem()->setText(C_ACTION, "Action");
    tree->headerItem()->setText(C_ACTION_LBL, "Package Status");
    */
    tree->header()->setStretchLastSection(true);

    tree->setColumnWidth(C_SUB_DIR, 80);
    tree->setColumnWidth(C_AERO, 120);
    tree->setColumnWidth(C_DESCRIPTION, 200);
    tree->setColumnWidth(C_ZIP_SIZE, 60);
    tree->setColumnWidth(C_ZIP_MD5, 160);
    //tree->setColumnWidth(C_STATUS, 80);
   // tree->setColumnWidth(C_ID, 50);

   // tree->setColumnHidden(C_ID, true);
       // tree->setColumnHidden(C_XML_SET, true);


   // tree->setColumnHidden(C_ZIP_MD5, true);
   // tree->setColumnHidden(C_ZIP_FILE, false);
   // tree->setColumnHidden(C_SUB_DIR, true);


    //====================================================
    //== Downloaded panel



    //==========================================
    //== Status Bar
    /*
    statusBar = new XStatusBar(this);
    mainLayout->addWidget(statusBar);
    connect(statusBar, SIGNAL(refresh_server()), this, SLOT(fetch_server()));
    */

    downManWidget = new DownloadManagerWidget();
    mainLayout->addWidget(downManWidget);
    connect(downManWidget, SIGNAL(status_update(QString,QString)),
                            this, SLOT(on_status_update(QString, QString)));

    //========================================
    //= Server Call
    //server = new ServerCall(this);
    //server->attach_statusbar(statusBar);
   // connect(server, SIGNAL(data(QScriptValue)),
    //        this, SLOT(on_server_data(QScriptValue)));



    //=================================
    //== Buttons groups
    buttGroupInstall = new QButtonGroup(this);
    connect(    buttGroupInstall, SIGNAL(buttonClicked(QAbstractButton*)),
                this, SLOT(on_install_button_clicked(QAbstractButton*))
    );

    buttGroupInfo = new QButtonGroup(this);
    connect(    buttGroupInfo, SIGNAL(buttonClicked(QAbstractButton*)),
                this, SLOT(on_info_button_clicked(QAbstractButton*))
    );

    QTimer::singleShot(500, this, SLOT(fetch_server()));
}


//=================================================================
//== Fetch Server
void AircraftInstallWidget::fetch_server()
{
    QUrl url("http://127.0.0.1/~fg/flightgear-aircraft/index.json");
    //QUrl url("http://downloads.freeflightsim.org/flightgear-aircraft/index.json");
    QNetworkRequest req(url);
    netMan->get(req);
    qDebug() << "requeeted" << url.toString();
}

//=================================================================
//== Load Server Data
void AircraftInstallWidget::on_server_data(QScriptValue data)
{

    if ( data.property("aircraft").isArray() )
    {
        qDebug() << "on_server_data";
        QScriptValueIterator it(data.property("aircraft"));
        int c = 0;
        while (it.hasNext()) {
            it.next();
            if (it.flags() & QScriptValue::SkipInEnumeration){
                continue;
            }
            QList<QStandardItem*> items = this->create_model_row();


            QString dir = it.value().property("dir").toString();
             qDebug() << "YES" << dir;
            /*
            item->setIcon(C_AERO, QIcon(":/icon/rel"));
            QFont f = item->font(C_AERO);
            f.setBold(true);
            item->setFont(C_AERO, f);
            item->setText(C_AERO, it.value().property("aero").toString());
            */
            items.at(C_SUB_DIR)->setText( dir.append(".zip") );
            items.at(C_AERO)->setText( it.value().property("aero").toString() );
            items.at(C_LAST_UPDATED)->setText(it.value().property("last_updated").toString());

            items.at(C_AUTHOR)->setText( it.value().property("author").toString());
            items.at(C_DESCRIPTION)->setText( it.value().property("description").toString());
            items.at(C_ZIP_MD5)->setText( it.value().property("md5").toString());
            items.at(C_ZIP_SIZE)->setText( it.value().property("zip_size").toString() );

            items.at(C_ACTION_LBL)->setText( "Available for download");


            //= only after adding item to tree can we add the buttons..
            model->appendRow(items);

            /*
            QToolButton *buttInfo = new QToolButton();
            buttInfo->setText(it.value().property("aero").toString());
            buttInfo->setProperty("id", it.value().property("id").toString());
            buttInfo->setProperty("zip_file", it.value().property("zip_file").toString());
            buttInfo->setIcon(QIcon(":/icon/rel"));
            buttInfo->setAutoRaise(true);
            buttInfo->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            //buttInfo->setProperty("state", "none");
            buttInfo->setStyleSheet("font-weight: bold; text-align: left;");
            //tree->setItemWidget(item, C_AERO, buttInfo);
            tree->setIndexWidget(items.at(C_ACTION)->index(), buttInfo);
            buttGroupInfo->addButton(buttInfo);
            */
            QToolButton *buttInstall = new QToolButton();
            buttInstall->setAutoRaise(true);
            buttInstall->setText("Download");
            buttInstall->setProperty("id", it.value().property("id").toString());
            buttInstall->setProperty("zip_file", it.value().property("zip_file").toString());
            buttInstall->setProperty("state", "available");
            //tree->setItemWidget(item, C_ACTION, buttInstall);
            tree->setIndexWidget(items.at(C_ACTION)->index(), buttInstall);
            buttGroupInstall->addButton(buttInstall);

            c++;
        }
    }
}


//======================================================
// Info Button
void AircraftInstallWidget::on_info_button_clicked(QAbstractButton *butt)
{
    emit open_aero(butt->property("id").toInt());
    qDebug() << "emit" << butt->property("id").toInt();

}


//======================================================
// Install Button
void AircraftInstallWidget::on_install_button_clicked(QAbstractButton *butt)
{

    //#if ( downloadQueue.contains(""))
   // qDebug() << "qidx=" << downloadQueue.indexOf(butt->property("id").toString());
    QString state = butt->property("state").toString();

    //qDebug() << "on_install id/state = " << butt->property("id") << state;
    //d//ownManWidget->add_download(butt->property("zip_file").toString(),
     //                           XSettings::getInstance().server_url("/download/aircraft/zip/"),
    //                            XSettings::temp(QString("/aero/")));

   // return;

    QString zip_file = butt->property("zip_file").toString();

    if(  state == "available" ){

        /*
        QList<QAbstractButton *> butts = buttGroupInstall->buttons();
        for(int i = 0; i < butts.count(); i++){
           QAbstractButton *b = butts.at(i);
           if( zip_file == b->property("zip_file").toString()){
               b->setText("Remove");
               b->setProperty("state", "queued");
               b->setStyleSheet("color: #000099; font-weight: bold;");
           }
        }
        QList<QTreeWidgetItem *> items = tree->findItems(zip_file, Qt::MatchExactly, C_ZIP_FILE);
        for(int i = 0; i < items.count(); i++){
            QTreeWidgetItem *item = items.at(i);
            item->setText(C_ACTION_LBL, "Queued for download");
        }
        */

        //downManWidget->add_download(zip_file,
        //                           XSettings::getInstance().server_url("/download/aircraft/zip/"),
        //                            XSettings::temp(QString("/aero/")));



    }else if(state == "queued"){
        //downloadQueue.remove_file(zip_file);
        //downManWidget->remove_download(zip_file,
        //                           XSettings::getInstance().server_url("/download/aircraft/zip/"),
        //                            XSettings::temp(QString("/aero/")));

    }
    //progressBar->setVisible(true);
}


void AircraftInstallWidget::on_status_update(QString zip_file, QString status)
{
    QString butt_label = "na";
    QString status_label = "na";
    QString style = "";
    bool disabled = false;

    if(status == "available"){
        butt_label = "Download";
        status_label = "Available for download";

    }else if(status == "queued"){
        butt_label = "Queued";
        status_label = "Queued for download";
        style = "font-weight: bold; color: #009900;";
        disabled = true;

    }


    /*
    QList<QTreeWidgetItem *> items = tree->findItems(zip_file, Qt::MatchExactly, C_ZIP_FILE);
    for(int i = 0; i < items.count(); i++){
        QTreeWidgetItem *item = items.at(i);
        item->setText(C_ACTION_LBL, status_label);
    }

    QList<QAbstractButton *> butts = buttGroupInstall->buttons();
    for(int i = 0; i < butts.count(); i++){
       QAbstractButton *b = butts.at(i);
       if( zip_file == b->property("zip_file").toString()){
           b->setText(butt_label);
           b->setProperty("state", status);
           b->setStyleSheet(style);
           b->setDisabled(disabled);
       }
    }*/
}

void AircraftInstallWidget::on_request_finished(QNetworkReply *reply){
    qDebug() << "on_request_finished" << reply->request().url().toString();
    QScriptEngine engine;
    QScriptValue json = engine.evaluate( "(" + reply->readAll() + ")");
    this->on_server_data(json);
}


QList<QStandardItem*> AircraftInstallWidget::create_model_row(){
    QList<QStandardItem*> lst;
    for(int i = 0; i < this->model->columnCount(); i++){
        lst.append( new QStandardItem() );
    }
    //model->appendRow(lst);
    return lst;
}
