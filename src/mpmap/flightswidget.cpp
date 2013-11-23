// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // pilotswidget.cpp
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#include <QtCore/QTimer>

#include <QListIterator>

#include <QDomNodeList>
#include <QDomNamedNodeMap>


#include <QNetworkRequest>
#include <QNetworkReply>


#include <QVBoxLayout>
#include <QToolBar>
#include <QVBoxLayout>
#include <QTreeWidgetItem>
#include <QHeaderView>

//#include "utilities/utilities.h"
#include "xwidgets/xtreewidgetitem.h"
#include "flightswidget.h"




FlightsWidget::FlightsWidget(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mob;

	setMinimumWidth(300);


	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->setContentsMargins(0,0,0,0);
	mainLayout->setSpacing(0);
	setLayout(mainLayout);



	//========================================
	//= Toolbar
	QToolBar *toolbar = new QToolBar();
	mainLayout->addWidget(toolbar);
	toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);


	checkBoxFollowSelected = new QCheckBox();
	checkBoxFollowSelected->setText("Follow Selected");
	toolbar->addWidget(checkBoxFollowSelected);


	//= CheckBox AutoRefresh
	checkBoxAutoRefresh = new QCheckBox("Auto");
	toolbar->addWidget(checkBoxAutoRefresh);
	checkBoxAutoRefresh->setChecked(mainObject->settings->value("mpxmap_autorefresh_enabled").toBool());
	connect(checkBoxAutoRefresh, SIGNAL(stateChanged(int)), this, SLOT(on_check_autorefresh(int)));

	//= ComboBox HZ
	comboBoxHz = new QComboBox();
	toolbar->addWidget(comboBoxHz);
	for(int sex=1; sex < 10; sex++){
		comboBoxHz->addItem(QString("%1 sec").arg(QString::number(sex)), QString::number(sex));
	}
	int cidx = comboBoxHz->findData(mainObject->settings->value("mpxmap_autorefresh_hz").toString());
	comboBoxHz->setCurrentIndex(cidx == -1 ? 0 : cidx);
	connect(comboBoxHz, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combo_changed(int)));

	//= Refresh
	QAction *actionRefresh = new QAction(this);
	//actionRefresh->setText("Refresh");
	actionRefresh->setIcon(QIcon(":/icon/refresh"));
	toolbar->addAction(actionRefresh);
	connect(actionRefresh, SIGNAL(triggered()), this, SLOT(fetch_pilots()));



	//===================================================
	// Show cols actiona re created here but added below to status bar
	//=============================================
	// Cols Selector
	QToolButton *buttShowColumns = new QToolButton(this);
	buttShowColumns->setText("Show");
	buttShowColumns->setIcon(QIcon(":/icon/select_cols"));
	buttShowColumns->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	buttShowColumns->setPopupMode(QToolButton::InstantPopup);


	QMenu *menuCols = new QMenu();
	buttShowColumns->setMenu(menuCols);

	//= Cols Widget
	QWidget *widgetColsSelecta = new QWidget();
	QVBoxLayout *layCols = new QVBoxLayout();
	widgetColsSelecta->setLayout(layCols);

	buttonGroupCols = new QButtonGroup(this);
	buttonGroupCols->setExclusive(false);
	connect(buttonGroupCols, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(on_show_cols(QAbstractButton*)));

	mainObject->settings->beginGroup("pilots_widget_cols");


	QCheckBox *chkShowModel = new QCheckBox();
	chkShowModel->setText("Aircraft Type");
	layCols->addWidget(chkShowModel);
    buttonGroupCols->addButton(chkShowModel, FlightsModel::C_AIRCRAFT);
    chkShowModel->setChecked(mainObject->settings->value(QString::number(FlightsModel::C_AIRCRAFT), "1").toBool());

	QCheckBox *chkShowLatLon = new QCheckBox();
	chkShowLatLon->setText("Lat/Lon");
	layCols->addWidget(chkShowLatLon);
    buttonGroupCols->addButton(chkShowLatLon, FlightsModel::C_LAT);
    chkShowLatLon->setChecked(mainObject->settings->value(QString::number(FlightsModel::C_LAT), "1").toBool());

	QCheckBox *chkShowAlt = new QCheckBox();
	chkShowAlt->setText("Altitude");
	layCols->addWidget(chkShowAlt);
    buttonGroupCols->addButton(chkShowAlt, FlightsModel::C_ALTITUDE);
    chkShowAlt->setChecked(mainObject->settings->value(QString::number(FlightsModel::C_ALTITUDE), "1").toBool());

	QCheckBox *chkShowHdg = new QCheckBox();
	chkShowHdg->setText("Heading");
	layCols->addWidget(chkShowHdg);
    buttonGroupCols->addButton(chkShowHdg, FlightsModel::C_HEADING);
    chkShowHdg->setChecked(mainObject->settings->value(QString::number(FlightsModel::C_HEADING), "1").toBool());

	mainObject->settings->endGroup();

	QWidgetAction *colsWidgetAction = new QWidgetAction(this);
	colsWidgetAction->setDefaultWidget(widgetColsSelecta);
	menuCols->addAction(colsWidgetAction);

    //=========================================================
    // Models
    //this->proxyModel = new QSortFilterProxyModel(this);
    //this->proxyModel->setSourceModel(this->mainObject->flightsModel);

	//=========================================================
	//== Tree
    tree = new QTreeView();
	mainLayout->addWidget(tree);

    this->tree->setModel(this->mainObject->flightsModel);

	tree->setRootIsDecorated(false);
	tree->setUniformRowHeights(true);
	tree->setAlternatingRowColors(true);


	tree->header()->setStretchLastSection(true);
	tree->header()->setResizeMode(QHeaderView::Stretch);
    /*
	tree->headerItem()->setText(C_CALLSIGN, "Callsign");
	tree->headerItem()->setText(C_AIRCRAFT, "Aircraft");
	tree->headerItem()->setText(C_ALTITUDE, "Alt");
	tree->headerItem()->setText(C_HEADING, "Hdg");
	tree->headerItem()->setText(C_PITCH, "Pitch");
	tree->headerItem()->setText(C_LAT, "Lat");
	tree->headerItem()->setText(C_LON, "Lon");
	tree->headerItem()->setText(C_FLAG, "Flag");
	tree->headerItem()->setText(C_COUNT, "Count");

	tree->headerItem()->setTextAlignment(C_ALTITUDE, Qt::AlignRight);
	tree->headerItem()->setTextAlignment(C_HEADING, Qt::AlignRight);
	tree->headerItem()->setTextAlignment(C_LAT, Qt::AlignRight);
	tree->headerItem()->setTextAlignment(C_LON, Qt::AlignRight);
    */

    //tree->setColumnHidden(C_PITCH, true);
    tree->setColumnHidden(FlightsModel::C_FLAG, true);
    tree->setColumnHidden(FlightsModel::C_COUNT, true);


    tree->setColumnHidden(FlightsModel::C_AIRCRAFT, !chkShowModel->isChecked());
    tree->setColumnHidden(FlightsModel::C_HEADING, !chkShowHdg->isChecked());
    tree->setColumnHidden(FlightsModel::C_ALTITUDE, !chkShowAlt->isChecked());
    //tree->setColumnHidden(FlightsModel::C_LAT, !chkShowLatLon->isChecked());
    //tree->setColumnHidden(FlightsModel::C_LON, !chkShowLatLon->isChecked());
    tree->setColumnHidden(FlightsModel::C_LAT, true);
    tree->setColumnHidden(FlightsModel::C_LON, true);

    tree->setColumnWidth(FlightsModel::C_CALLSIGN, 100);
    tree->setColumnWidth(FlightsModel::C_AIRCRAFT, 100);
    tree->setColumnWidth(FlightsModel::C_ALTITUDE, 50);
    tree->setColumnWidth(FlightsModel::C_HEADING, 50);

	tree->setSortingEnabled(true);
    tree->sortByColumn(FlightsModel::C_CALLSIGN, Qt::AscendingOrder);

    //connect(tree,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
    //		this, SLOT(on_item_doubled_clicked(QTreeWidgetItem*,int)));

	//===========================================================================
	//= Status Bar
	statusBar = new QStatusBar();
	mainLayout->addWidget(statusBar);
	statusBar->showMessage("Click refresh to load");

	statusBar->addPermanentWidget(buttShowColumns);



}
/*
void FlightsWidget::fetch_pilots()
{

	server_string = "";
	QUrl url("http://mpmap01.flightgear.org/fg_server_xml.cgi?mpserver01.flightgear.org:5001");
	QNetworkRequest request;
	request.setUrl(url );

	reply = netMan->get(request);
	connect(reply, SIGNAL( error(QNetworkReply::NetworkError)),
			this, SLOT(on_server_error(QNetworkReply::NetworkError))
	);
	connect(reply, SIGNAL( readyRead()),
			this, SLOT(on_server_ready_read())
	);
	connect(reply, SIGNAL( finished()),
			this, SLOT(on_server_read_finished())
	);
	statusBar->showMessage("Request");

}
*/





void FlightsWidget::on_check_autorefresh(int checked){
	mainObject->settings->setValue("mpxmap_autorefresh_enabled", checked);
	if(checked){
        //fetch_pilots();
		//timer->start();
	}else{
		//timer->stop();
	}
}

void FlightsWidget::on_combo_changed(int idx){
	Q_UNUSED(idx);
	mainObject->settings->setValue("mpxmap_autorefresh_hz", comboBoxHz->itemData(comboBoxHz->currentIndex()).toString());
	//timer->setInterval(comboBoxHz->itemData(comboBoxHz->currentIndex()).toInt());
}

//void FlightsWidget::on_item_doubled_clicked(QTreeWidgetItem *item, int colidx){
//	Q_UNUSED(colidx);
	//emit aircraft_selected(item->text(C_CALLSIGN));
    //XAero aero(item->text(C_CALLSIGN));
    //aero.lat = item->text(C_LAT);
    //aero.lon = item->text(C_LON);
    //emit aircraft_selected(aero);
//}



void FlightsWidget::on_show_cols(QAbstractButton *button)
{
	int col_idx = buttonGroupCols->id(button);
	tree->setColumnHidden(col_idx, !button->isChecked() );
    if(col_idx == FlightsModel::C_LAT){
        tree->setColumnHidden(FlightsModel::C_LON, !button->isChecked() );
	}
	mainObject->settings->beginGroup("pilots_widget_cols");
	mainObject->settings->setValue(QString::number(col_idx), button->isChecked());
	mainObject->settings->endGroup();
	mainObject->settings->sync();
}


