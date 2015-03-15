

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
#include <QDockWidget>
#include <QFormLayout>



#include "xwidgets/toolbargroup.h"

#include "installer/aircraftinstallwidget.h"

AircraftInstallWidget::AircraftInstallWidget(MainObject *mOb, QMainWindow *parent) :
    QMainWindow(parent)
{

    mainObject = mOb;
    //==================================
    //= Setup Models
    model = new QStandardItemModel(this);
    QStringList hLabels;
    hLabels << "Dir" << "Aero" << "Description" <<  "Author" <<  "Size" << "Hash" << "Updated" << "Status" << "Filter";
    model->setHorizontalHeaderLabels(hLabels);

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(C_FILTER);


    //==================================
    //= NetworkManager
    netMan = new QNetworkAccessManager(this);
    connect(this->netMan, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(on_request_finished(QNetworkReply*))
            );


    //==================================
    //= Centrail Main Widget and layout
    QWidget *mainWidget = new QWidget();
    this->setCentralWidget(mainWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    mainWidget->setLayout(mainLayout);


    //=========================================
    //= Top toolbar
    QHBoxLayout *topBar = new QHBoxLayout();
    topBar->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(topBar);

    //---------------------------------------
    //= Filter Toolbar
    ToolBarGroup *grpFilter = new ToolBarGroup();
    topBar->addWidget(grpFilter);
    grpFilter->setTitle("Filter");

    //= Clear filter button
    QToolButton * buttClearFilter = new QToolButton();
    buttClearFilter->setText("CKR >");
    buttClearFilter->setFixedWidth(20);
    buttClearFilter->setIcon(QIcon(":/icon/clear_filter"));
    buttClearFilter->setAutoRaise(true);
    buttClearFilter->setToolButtonStyle(Qt::ToolButtonIconOnly);
    grpFilter->addWidget(buttClearFilter);
    connect(buttClearFilter, SIGNAL(clicked()), this, SLOT(on_clear_filter()) );

    //= Filter Text
    txtFilter = new QLineEdit();
    txtFilter->setFixedWidth(100);
    grpFilter->addWidget(txtFilter);
    connect(txtFilter, SIGNAL(textChanged(const QString)), this, SLOT(on_filter_text_changed(const QString)));

    topBar->addStretch(10);


    //=========================================
    //= Tree
    QString tree_style("QTreeView::item:hover {background-color: #dddddd;}");

    this->tree = new QTreeView();
    mainLayout->addWidget(this->tree);

    this->tree->setModel(this->proxyModel);
    this->tree->setUniformRowHeights(true);
    this->tree->setAlternatingRowColors(true);
    this->tree->setRootIsDecorated(false);
    this->tree->setStyleSheet(tree_style);


    this->tree->header()->setStretchLastSection(true);

    this->tree->setColumnWidth(C_SUB_DIR, 150);
    this->tree->setColumnWidth(C_AERO, 120);
    this->tree->setColumnWidth(C_DESCRIPTION, 200);
    this->tree->setColumnWidth(C_ZIP_SIZE, 60);
    this->tree->setColumnWidth(C_ZIP_MD5, 160);


    this->tree->setColumnHidden(C_ZIP_MD5, true);
    this->tree->setColumnHidden(C_ZIP_SIZE, true);
    this->tree->setColumnHidden(C_FILTER, true);
    connect(this->tree->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(on_tree_selection_changed(QItemSelection,QItemSelection))
            );


    //====================================================
    //== Download Side panel
    QDockWidget *dockAero = new QDockWidget();
    dockAero->setFeatures(QDockWidget::NoDockWidgetFeatures);
    dockAero->setWindowTitle("Details");
    dockAero->setMinimumWidth(200);
    this->addDockWidget(Qt::RightDockWidgetArea, dockAero);

    QWidget *widgetAero = new QWidget();
    dockAero->setWidget(widgetAero);

    QFormLayout *layForm = new QFormLayout();
    widgetAero->setLayout(layForm);

    this->lblAero = new QLabel();
    this->lblAero->setStyleSheet("background-color: white; font-weight: bold;");
    layForm->addRow("Aero", this->lblAero);

    buttDownload = new QPushButton();
    buttDownload->setText("Download");
    buttDownload->setDisabled(true);
    layForm->addRow("", this->buttDownload);
    connect(buttDownload, SIGNAL(clicked()),
            this, SLOT(on_butt_download_clicked())
            );

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





    QTimer::singleShot(500, this, SLOT(fetch_server()));
}

void AircraftInstallWidget::on_tree_selection_changed(QItemSelection sel, QItemSelection desel){

    if(this->tree->selectionModel()->hasSelection() == false){
        this->buttDownload->setDisabled(true);
        this->lblAero->setText("");
        return;
    }

    this->buttDownload->setDisabled(false);
    QString aero = this->model->itemFromIndex( this->proxyModel->mapToSource(sel.indexes().at(C_SUB_DIR)) )->text();
    this->lblAero->setText(aero);
}

//=================================================================
//== Fetch Server
void AircraftInstallWidget::fetch_server()
{
    QUrl url( this->mainObject->settings->aircraft_downloads_url("index.json") );
    QNetworkRequest req(url);
    netMan->get(req);
    qDebug() << "requeeted" << url.toString();
}

//=================================================================
//== Load Server Data
void AircraftInstallWidget::load_data(QScriptValue data)
{

    if ( data.property("aircraft").isArray() )
    {
        qDebug() << "on_server_data";
        QScriptValueIterator it(data.property("aircraft"));

        while (it.hasNext()) {
            it.next();
            if (it.flags() & QScriptValue::SkipInEnumeration){
                continue;
            }
            QList<QStandardItem*> items = this->create_model_row();


            QString dir = it.value().property("dir").toString();
            QString description = it.value().property("description").toString();
            // qDebug() << "YES" << dir;

            items.at(C_SUB_DIR)->setText( dir.append(".zip") );
            items.at(C_SUB_DIR)->setIcon( QIcon(":/icon/zip"));
            QFont font = items.at(C_SUB_DIR)->font();
            font.setBold(true);
            items.at(C_SUB_DIR)->setFont(font);



            items.at(C_AERO)->setText( it.value().property("aero").toString() );
            items.at(C_LAST_UPDATED)->setText(it.value().property("last_updated").toString());

            items.at(C_AUTHOR)->setText( it.value().property("author").toString());
            items.at(C_DESCRIPTION)->setText( description);
            items.at(C_ZIP_MD5)->setText( it.value().property("md5").toString());
            items.at(C_ZIP_SIZE)->setText( it.value().property("zip_size").toString() );

            items.at(C_FILTER)->setText( description.append(dir) );

            items.at(C_STATUS)->setText( "Available for download");


            //= only after adding item to tree can we add the buttons..
            model->appendRow(items);

        }
    }
}



//======================================================


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
    this->load_data(json);
}


QList<QStandardItem*> AircraftInstallWidget::create_model_row(){
    QList<QStandardItem*> lst;
    for(int i = 0; i < this->model->columnCount(); i++){
        lst.append( new QStandardItem() );
    }
    //model->appendRow(lst);
    return lst;
}

void AircraftInstallWidget::on_clear_filter(){
    txtFilter->setText("");
    txtFilter->setFocus();
}

void AircraftInstallWidget::on_filter_text_changed(const QString s){
    proxyModel->setFilterFixedString(s);
}


void AircraftInstallWidget::on_butt_download_clicked(){


    QString state = "available";

    QString zip_file = lblAero->text();
    QString sub_dir = lblAero->text();
    sub_dir.chop(4);

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

        downManWidget->add_download(zip_file,
                                   this->mainObject->settings->aircraft_downloads_url(sub_dir, zip_file),
                                   QString("/home/fgx/_TEMP_/")
                                    );



    }else if(state == "queued"){
        //downloadQueue.remove_file(zip_file);
        //downManWidget->remove_download(zip_file,
        //                           XSettings::getInstance().server_url("/download/aircraft/zip/"),
        //                            XSettings::temp(QString("/aero/")));

    }
    //progressBar->setVisible(true);_
}
