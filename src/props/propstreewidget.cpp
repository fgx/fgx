

#include <QtCore/QList>

#include <QtGui/QVBoxLayout>
#include <QtGui/QSplitter>
#include <QtGui/QToolBar>
#include <QtGui/QAction>
#include <QtGui/QAbstractItemView>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QIcon>
#include <QtGui/QFont>


#include "propstreewidget.h"
#include "propeditwidget.h"

/*



  --TODO--
  Move tree columns numbers to constants

  */

PropsTreeWidget::PropsTreeWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{

    mainObject = mOb;

	telnet = new TelnetSlave(this);
	telnet->telnet_connect("127.0.0.1", 7777);


	connect(telnet, SIGNAL(props_path(QString, QString)),
             this, SLOT(on_props_path(QString, QString))
    );
	connect(telnet, SIGNAL(props_node(QString, QString, QString, QString)),
            this, SLOT(on_props_node(QString, QString, QString, QString))
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

    //*****************************
    //** Toolbar
    QToolBar *treeToolbar = new QToolBar();
    treeLayout->addWidget(treeToolbar);
    treeToolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    //** Refresh Button
    QAction *actionRefreshTree = new QAction(this);
    treeToolbar->addAction(actionRefreshTree);
    actionRefreshTree->setText("Refresh");
	actionRefreshTree->setIcon(QIcon(":/icon/refresh"));
    connect(actionRefreshTree, SIGNAL(triggered()), this, SLOT(load_nodes()) );

    treeToolbar->addSeparator();

    //*****************************************
    //** Autorefresh
    //****************************************
    timer = new QTimer(this);
    timer->setInterval(1000); //TODO default more

    //** Check Autorefresh enabled
    chkAutoRefresh = new QCheckBox();
    treeToolbar->addWidget(chkAutoRefresh);
    chkAutoRefresh->setText("Autorefresh Enabled");
    chkAutoRefresh->setCheckState(Qt::Unchecked);
    connect(chkAutoRefresh, SIGNAL(toggled(bool)),
            this, SLOT(on_auto_refresh_enabled())
    );

    //** ComboBox sets refresh rate
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

	//** Edit Property Action
	actionEditProperty = new QAction(this);
	actionEditProperty->setText("Edit");
	actionEditProperty->setDisabled(true);
	actionEditProperty->setIcon(QIcon(":/icons/node_val"));
	treeToolbar->addAction(actionEditProperty);
	connect(actionEditProperty, SIGNAL(triggered()), this, SLOT(on_edit_property()) );

	QAction *actionTest = new QAction(this);
	actionTest->setText("Test");
	treeToolbar->addAction(actionTest);
	connect(actionTest, SIGNAL(triggered()), this, SLOT(on_test_()) );

    //******************************************************
    //** Tree Widgets
    treeWidget = new QTreeWidget(this);
    treeLayout->addWidget(treeWidget);
    treeWidget->setItemsExpandable(true);
    treeWidget->setAlternatingRowColors(true);
    treeWidget->setRootIsDecorated(true);
    treeWidget->setSortingEnabled(true);
    treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    treeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    treeWidget->setSortingEnabled(true);
    treeWidget->sortByColumn(0, Qt::AscendingOrder);

    QTreeWidgetItem *headerItem = treeWidget->headerItem();
	headerItem->setText(C_NODE, tr("Property"));
	headerItem->setText(C_VALUE, tr("Value"));
	headerItem->setText(C_TYPE, tr("Type"));
	headerItem->setText(C_PATH, tr("Full Path"));
    treeWidget->setColumnWidth(0, 200);

    connect(treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)),
             this, SLOT(on_item_expanded(QTreeWidgetItem*))
    );
	connect(treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
			this, SLOT(on_item_double_clicked(QTreeWidgetItem*,int))
    );
	connect(treeWidget, SIGNAL(itemSelectionChanged()),
			this, SLOT(on_on_aiport_selection_changedselection_changed())
	);


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


//*************************************************************************
//** On props Node
//*************************************************************************
void PropsTreeWidget::on_props_node(QString parent_path, QString node_name,
                                    QString node_value, QString node_type){
    //qDebug() << "YES " << parent_path << "=" << node_value;

    //** Find if actual path end node is already there
    QString end_path = QString(parent_path).append(node_name);
    QList<QTreeWidgetItem *> existing_items = treeWidget->findItems(end_path,
                                                           Qt::MatchExactly | Qt::MatchRecursive,
                                                            3);
    if( existing_items.size() > 0){
        //qDebug() << "NODE EXISTS";
        existing_items[0]->setText(1, node_value); //* UPdate existing node
       return; //* node exists so no need to add
    }
   //QTreeWidgetItem *parent_item;

	//*** TODO refactor this lot to avoif duplicate code, need sfferent parent for node
    //** Otherwise find the parent and add child
    if(parent_path == "/"){
        QTreeWidgetItem *newTopItem = new QTreeWidgetItem();
		newTopItem->setText(C_NODE, node_name);
		newTopItem->setText(C_VALUE, node_value);
		newTopItem->setText(C_TYPE, node_type);
		newTopItem->setText(C_PATH, end_path );
		newTopItem->setIcon(0, QIcon(":/icon/prop_node"));
       treeWidget->addTopLevelItem(newTopItem);
    }else{
        QList<QTreeWidgetItem *> items = treeWidget->findItems(parent_path,
                                                           Qt::MatchExactly | Qt::MatchRecursive,
                                                            3);
        QTreeWidgetItem *newNodeItem = new QTreeWidgetItem(items[0]);

		newNodeItem->setText(C_NODE, node_name);
		newNodeItem->setText(C_VALUE, node_value);
		newNodeItem->setText(C_TYPE, node_type);
		newNodeItem->setText(C_PATH, end_path );
		newNodeItem->setIcon(0, QIcon(":/icon/prop_node"));
    }
}
//*************************************************************************
//** On props Path
//*************************************************************************
void PropsTreeWidget::on_props_path(QString parent_path, QString path){

    //** Find if actual path end node is already there
    QString end_path = QString(parent_path).append(path);
    QList<QTreeWidgetItem *> end_items = treeWidget->findItems(end_path,
                                                           Qt::MatchExactly | Qt::MatchRecursive,
                                                            3);
    if( end_items.size() > 0){
       return; //* node exists so no need to add
    }


    //** Otherwise find the parent and add child
    if(parent_path == "/"){
        QTreeWidgetItem *newTopItem = new QTreeWidgetItem();
		newTopItem->setText(C_NODE, path.left(path.length() - 1));
        newTopItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
		newTopItem->setIcon(C_NODE, QIcon(":/icon/folder")); // folder_closed
		newTopItem->setText(C_PATH, end_path );
        treeWidget->addTopLevelItem(newTopItem);
    }else{
        QList<QTreeWidgetItem *> items = treeWidget->findItems(parent_path,
                                                           Qt::MatchExactly | Qt::MatchRecursive,
                                                            3);
        QTreeWidgetItem *newNodeItem = new QTreeWidgetItem(items[0]);
		newNodeItem->setText(C_NODE, path.left(path.length() - 1));
        newNodeItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
		newNodeItem->setIcon(C_NODE, QIcon(":/icon/folder")); // folder_closed
		newNodeItem->setText(C_PATH, end_path );
    }
}

void PropsTreeWidget::on_item_expanded(QTreeWidgetItem *item){
    qDebug() << "ON Expand=" << item->text(3);
   // qDebug() << item->text(3);
	item->setIcon(0, QIcon(":/icon/folder")); // folder_open
    item->setText(1, tr("Refresh"));
    QFont font = item->font(1);
    font.setPointSize(7);
    item->setFont(1, font);
    QColor color(0, 0, 170);
    item->setForeground(1, color);
    item->setCheckState(1, Qt::Unchecked);

	telnet->get_node(item->text(3));
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
	QTreeWidgetItem *item = treeWidget->currentItem();
	if(!item){
		return;
	}
	//** Using the Type column to determine if its a value as there is always a type
	actionEditProperty->setDisabled(item->text(C_TYPE).length() == 0);
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
	PropEditWidget *propEditWidget = new PropEditWidget(mainObject);
	propEditWidget->set_from_item(treeWidget->currentItem());
	propEditWidget->exec();
}

void PropsTreeWidget::on_test_(){
	telnet->set_property("/autopilot/settings/target-speed-kt", "250");
}


//= window close
void PropsTreeWidget::closeEvent(QCloseEvent *event){
	mainObject->settings->saveWindow(this);
	event->accept();
}
