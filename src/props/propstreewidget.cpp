

#include <QHostAddress>
#include <QNetworkInterface>

#include <QtCore/QList>

#include <QtGui/QVBoxLayout>
#include <QtGui/QSplitter>
#include <QtGui/QToolBar>
#include <QtGui/QAction>
#include <QtGui/QAbstractItemView>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QIcon>
#include <QtGui/QFont>


#include "props/propstreewidget.h"
#include "props/propeditwidget.h"

/*



  --TODO--
  Move tree columns numbers to constants

  */

PropsTreeWidget::PropsTreeWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{

    mainObject = mOb;

	telnet = new TelnetSlave(this);

	connect(telnet, SIGNAL(props_folder(QString, QString)),
			 this, SLOT(on_props_folder(QString, QString))
    );
	connect(telnet, SIGNAL(props_value(QString, QString, QString, QString)),
			this, SLOT(on_props_value(QString, QString, QString, QString))
    );
	connect(telnet, SIGNAL(telnet_connected(bool)),
			this, SLOT(on_telnet_connected(bool))
	);

	setWindowTitle("Property Tree Browser");
	setWindowIcon(QIcon(":/icon/props"));


	setProperty("settings_namespace", QVariant("properties_window"));
	mainObject->settings->restoreWindow(this);


    //* Main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);

    QSplitter *splitter = new QSplitter(this);
    mainLayout->addWidget(splitter);

    //***************************************************
    //** Left
    QWidget *leftWidget = new QWidget();
    splitter->addWidget(leftWidget);
    QVBoxLayout *treeLayout = new QVBoxLayout();
    leftWidget->setLayout(treeLayout);
    treeLayout->setContentsMargins(0,0,0,0);
    treeLayout->setSpacing(0);


	//== Toolbar
    QToolBar *treeToolbar = new QToolBar();
    treeLayout->addWidget(treeToolbar);
    treeToolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	//===========================================================
	// Telnet
	//===========================================================

	//= Host
	comboHost = new QComboBox();
	comboHost->setMaximumWidth(100);
	//comboHost->setText("localhost");
	treeToolbar->addWidget(comboHost);
	QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
	for (int i = 0; i < addresses.size(); ++i) {
		if(addresses.at(i).protocol() == QAbstractSocket::IPv4Protocol){
			comboHost->addItem( addresses.at(i).toString(), addresses.at(i).toString() );
		}
	}
	comboHost->setCurrentIndex(0);

	//= Port
	txtPort = new QLineEdit();
	txtPort->setMaximumWidth(50);
	//txtPort->setText(mainObject->settings->value("telnet_port").toString());
	txtPort->setText("6666");
	treeToolbar->addWidget(txtPort);

	//= Connect
	actionConnect = new QAction(this);
	actionConnect->setText("Connect");
	actionConnect->setIcon(QIcon(":/icon/connect"));
	actionConnect->setDisabled(false);
	treeToolbar->addAction(actionConnect);
	connect(actionConnect, SIGNAL(triggered()), this, SLOT(telnet_connect()) );

	//= Disconnect
	actionDisconnect = new QAction(this);
	actionDisconnect->setText("DisConnect");
	actionDisconnect->setIcon(QIcon(":/icon/disconnect"));
	actionDisconnect->setDisabled(true);
	treeToolbar->addAction(actionDisconnect);
	connect(actionDisconnect, SIGNAL(triggered()), this, SLOT(telnet_disconnect()) );


	treeToolbar->addSeparator();

	//== Refresh Button
    QAction *actionRefreshTree = new QAction(this);
    treeToolbar->addAction(actionRefreshTree);
    actionRefreshTree->setText("Refresh");
	actionRefreshTree->setIcon(QIcon(":/icon/refresh"));
    connect(actionRefreshTree, SIGNAL(triggered()), this, SLOT(load_nodes()) );


    treeToolbar->addSeparator();

	//========================
	//== Autorefresh
    timer = new QTimer(this);
    timer->setInterval(1000); //TODO default more

	//== Check Autorefresh enabled
    chkAutoRefresh = new QCheckBox();
    treeToolbar->addWidget(chkAutoRefresh);
    chkAutoRefresh->setText("Autorefresh Enabled");
    chkAutoRefresh->setCheckState(Qt::Unchecked);
    connect(chkAutoRefresh, SIGNAL(toggled(bool)),
            this, SLOT(on_auto_refresh_enabled())
    );

	//== ComboBox sets refresh rate
    comboAutoRefreshRate = new QComboBox();
    treeToolbar->addWidget(comboAutoRefreshRate);
    comboAutoRefreshRate->addItem("1");
    comboAutoRefreshRate->addItem("2");
    comboAutoRefreshRate->addItem("3");
    comboAutoRefreshRate->addItem("4");
    comboAutoRefreshRate->addItem("5");
    connect(comboAutoRefreshRate, SIGNAL(activated(int)),
            this, SLOT(on_set_timer_rate())
    );

	treeToolbar->addSeparator();

	//== Edit Property Action
	actionEditProperty = new QAction(this);
	actionEditProperty->setText("Edit");
	actionEditProperty->setDisabled(true);
	actionEditProperty->setIcon(QIcon(":/icons/node_val"));
	treeToolbar->addAction(actionEditProperty);
	connect(actionEditProperty, SIGNAL(triggered()), this, SLOT(on_edit_property()) );
	actionEditProperty->setVisible(false);

	QAction *actionTest = new QAction(this);
	actionTest->setText("Test");
	treeToolbar->addAction(actionTest);
	connect(actionTest, SIGNAL(triggered()), this, SLOT(on_test_()) );
	actionTest->setVisible(false);


	//======================================================
	//= Models
	model = new QStandardItemModel(this);
	model->setColumnCount(4);
	QStringList headers;
	headers << "Node" << "Value" << "Type" << "Path";
	model->setHorizontalHeaderLabels(headers);


	proxyModel = new QSortFilterProxyModel(this);
	proxyModel->setSourceModel(model);


	//======================================================
	//=== Tree View
	tree = new QTreeView(this);
	treeLayout->addWidget(tree);
	tree->setModel(model);

	tree->setItemsExpandable(true);
	tree->setUniformRowHeights(true);
	tree->setAlternatingRowColors(true);
	tree->setRootIsDecorated(true);
	tree->setSelectionMode(QAbstractItemView::SingleSelection);
	tree->setSelectionBehavior(QAbstractItemView::SelectRows);
	tree->sortByColumn(C_NODE, Qt::AscendingOrder);
	tree->setSortingEnabled(true);

	/*
	QTreeWidgetItem *headerItem = tree->headerItem();
	headerItem->setText(C_NODE, tr("Property"));
	headerItem->setText(C_VALUE, tr("Value"));
	headerItem->setText(C_TYPE, tr("Type"));
	headerItem->setText(C_PATH, tr("Full Path"));
	*/
	tree->setColumnWidth(C_NODE, 200);
	tree->setColumnWidth(C_VALUE, 200);

	/*
	connect(tree, SIGNAL(itemExpanded(QTreeWidgetItem*)),
             this, SLOT(on_item_expanded(QTreeWidgetItem*))
    );
	connect(tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
			this, SLOT(on_item_double_clicked(QTreeWidgetItem*,int))
	); */
	//connect(treeWidget, SIGNAL(itemSelectionChanged()),
	//		this, SLOT(on_on_aiport_selection_changedselection_changed())
	//);


    statusBarTree = new QStatusBar();
    treeLayout->addWidget(statusBarTree);
    statusBarTree->showMessage("Idle");
}

//=============================================
void PropsTreeWidget::load_nodes(){
    //mainObject->telnet->fg_connect();
	telnet->get_node("/");
	qDebug() << "get_node /";
}


//================================
// make_row - returns a list of standard items
QList<QStandardItem *> PropsTreeWidget::make_row(QString node_name, QString node_value, QString node_type, QString end_path, bool is_folder)
{
	QList<QStandardItem *> items;

	QStandardItem *name = new QStandardItem(node_name);
	name->setIcon(QIcon(is_folder ? ":/icon/folder" : ":/icon/prop_node"));
	QStandardItem *value = new QStandardItem(node_value);
	QStandardItem *type = new QStandardItem(node_type);
	QStandardItem *path = new QStandardItem(end_path);

	items << name << value << type << path;
	return items;
}


//=========================================================================
//== On Props Folder
//=========================================================================
void PropsTreeWidget::on_props_folder(QString parent_path, QString path){

	//= Find if actual path end node is already there
	QString end_path = QString(parent_path).append(path);
	QList<QStandardItem *> items = model->findItems(end_path, Qt::MatchExactly | Qt::MatchRecursive, C_PATH);
	if( items.size() > 0){
	   return; //= node exists so no need to add
	}
	//== Find the parent and add child
	if(parent_path == "/"){
		//= add top level item
		model->invisibleRootItem()->appendRow(make_row(path, "","",path, true));
		//qDebug() << path;
		//telnet->get_node(path);
	}else{
		//= Find parent and append child node
		QList<QStandardItem *> fitems = model->findItems(path, Qt::MatchExactly | Qt::MatchRecursive, C_PATH);
		fitems.at(0)->appendRow( make_row(path, "","",path, true) );
	}

	//telnet->get_node(path);
}

//========================================================================
//== On props Value
//========================================================================
void PropsTreeWidget::on_props_value(QString parent_path, QString node_name,
                                    QString node_value, QString node_type){

	//qDebug() << parent_path << node_name << node_value;
	//= Find if actual path end node is already there
    QString end_path = QString(parent_path).append(node_name);
	QList<QStandardItem *> existing_items = model->findItems(end_path,
                                                           Qt::MatchExactly | Qt::MatchRecursive,
															C_PATH);
    if( existing_items.size() > 0){
		//= Node exists so update value
		existing_items[0]->setText( node_value);
		return;
    }

	//= Find the parent and add child
    if(parent_path == "/"){
		model->invisibleRootItem()->appendRow( make_row(node_name, node_value, node_type, end_path, false) );

    }else{
		QList<QStandardItem *> fitems = model->findItems(parent_path, Qt::MatchExactly | Qt::MatchRecursive, C_PATH);
		fitems.at(0)->appendRow( make_row(node_name, node_value, node_type, end_path, false) );
    }
}





void PropsTreeWidget::on_item_expanded(QTreeWidgetItem *item){
	Q_UNUSED(item);
   // qDebug() << "ON Expand=" << item->text(3);
   // qDebug() << item->text(3);
	//item->setIcon(0, QIcon(":/icon/folder")); // folder_open
	/*
	item->setText(1, tr("Refresh"));
    QFont font = item->font(1);
    font.setPointSize(7);
    item->setFont(1, font);
    QColor color(0, 0, 170);
    item->setForeground(1, color);
    item->setCheckState(1, Qt::Unchecked);
	*/
	//telnet->get_node(item->text(3));
}

//*** TODO ???
void PropsTreeWidget::on_item_double_clicked(QTreeWidgetItem *item, int col){
	Q_UNUSED(col);
	if(item->text(C_TYPE).length() == 0){
		return;
	}
	actionEditProperty->trigger();
}


void PropsTreeWidget::on_item_selection_changed(){
	//QTreeWidgetItem *item = treeWidget->currentItem();
	//if(!item){
	//	return;
	//}
	//** Using the Type column to determine if its a value as there is always a type
	//actionEditProperty->setDisabled(item->text(C_TYPE).length() == 0);
}




//*************************************************************
//* checkbox to start/stop timer in toolbar check button
void PropsTreeWidget::on_auto_refresh_enabled(){
    if( chkAutoRefresh->checkState() ){
        timer->start();
    }else{
        timer->stop();
    }
}
//** Combobox event to set timer rate
void PropsTreeWidget::on_set_timer_rate(){
    int rate = comboAutoRefreshRate->itemText( comboAutoRefreshRate->currentIndex() ).toInt();
    timer->setInterval(rate * 1000);
}






void PropsTreeWidget::on_edit_property(){
	return;
//	PropEditWidget *propEditWidget = new PropEditWidget(mainObject);
//	propEditWidget->set_from_item(treeWidget->currentItem());
//	propEditWidget->exec();
}

void PropsTreeWidget::on_test_(){
	telnet->set_property("/autopilot/settings/target-speed-kt", "250");
}


//= window close
void PropsTreeWidget::closeEvent(QCloseEvent *event){
	mainObject->settings->saveWindow(this);
	event->accept();
}


void PropsTreeWidget::telnet_connect(){
	telnet->telnet_connect(comboHost->currentText(), txtPort->text().toInt());
}
void PropsTreeWidget::telnet_disconnect(){
	telnet->telnet_disconnect();
}

void PropsTreeWidget::on_telnet_connected(bool connected){
	qDebug()<< "on_connect" << connected;
	actionConnect->setEnabled(!connected);
	actionDisconnect->setEnabled(connected);
	if(connected){
		telnet->get_node("/");
	}
}
