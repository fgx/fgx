
#include <QtDebug>


#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNamedNodeMap>


#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>


#include <QVBoxLayout>
#include <QToolBar>
#include <QVBoxLayout>
#include <QTreeWidgetItem>


#include "pilotswidget.h"

PilotsWidget::PilotsWidget(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mob;

	setMinimumWidth(400);


	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->setContentsMargins(0,0,0,0);
	mainLayout->setSpacing(0);
	setLayout(mainLayout);



	//========================================
	//= Toolbar
	QToolBar *toolbar = new QToolBar();
	mainLayout->addWidget(toolbar);
	toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	//* Refresh
	QAction *actionRefresh = new QAction(this);
	actionRefresh->setText("Refresh");
	actionRefresh->setIcon(QIcon(":/icon/refresh"));
	toolbar->addAction(actionRefresh);
	connect(actionRefresh, SIGNAL(triggered()), this, SLOT(fetch_pilots()));

	//* CheckBox AutoRefresh
	checkBoxAutoRefresh = new QCheckBox("Auto Refresh");
	toolbar->addWidget(checkBoxAutoRefresh);
	checkBoxAutoRefresh->setChecked(mainObject->settings->value("mpxmap_autorefresh_enabled").toBool());
	connect(checkBoxAutoRefresh, SIGNAL(stateChanged(int)), this, SLOT(on_check_autorefresh(int)));

	//* ComboBox HZ
	comboBoxHz = new QComboBox();
	toolbar->addWidget(comboBoxHz);
	for(int sex=1; sex < 10; sex++){
		comboBoxHz->addItem(QString("%1 sec").arg(QString::number(sex)), QString::number(sex * 1000));
	}
	int cidx = comboBoxHz->findData(mainObject->settings->value("mpxmap_autorefresh_hz").toString());
	comboBoxHz->setCurrentIndex(cidx == -1 ? 0 : cidx);
	connect(comboBoxHz, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combo_changed(int)));


	//=========================================================
	//== Tree ( Coeden )
	tree = new QTreeWidget();
	mainLayout->addWidget(tree);

	tree->setRootIsDecorated(false);
	tree->setUniformRowHeights(true);
	tree->setAlternatingRowColors(true);

	tree->headerItem()->setText(C_CALLSIGN, "Callsign");
	tree->headerItem()->setText(C_AIRCRAFT, "Aircraft");
	tree->headerItem()->setText(C_ALTITUDE, "Alt");
	tree->headerItem()->setText(C_HEADING, "Hdg");
	tree->headerItem()->setText(C_PITCH, "Pitch");
	tree->headerItem()->setText(C_LAT, "Lat");
	tree->headerItem()->setText(C_LNG, "Lng");
	tree->headerItem()->setText(C_FLAG, "Flag");

	tree->headerItem()->setTextAlignment(C_ALTITUDE, Qt::AlignRight);
	tree->headerItem()->setTextAlignment(C_HEADING, Qt::AlignRight);
	tree->headerItem()->setTextAlignment(C_LAT, Qt::AlignRight);
	tree->headerItem()->setTextAlignment(C_LNG, Qt::AlignRight);

	tree->setColumnHidden(C_PITCH, true);
	tree->setColumnHidden(C_FLAG, true);

	tree->setColumnWidth(C_CALLSIGN, 80);
	tree->setColumnWidth(C_AIRCRAFT, 80);
	tree->setColumnWidth(C_ALTITUDE, 50);
	tree->setColumnWidth(C_HEADING, 50);

	tree->setSortingEnabled(true);
	tree->sortByColumn(C_CALLSIGN, Qt::AscendingOrder);

	//= Status Bar
	statusBar = new QStatusBar();
	mainLayout->addWidget(statusBar);
	statusBar->showMessage("Click refresh to load");


	//==========================================================
	//= Initialize Objects
	netMan = new QNetworkAccessManager(this);

	timer = new QTimer(this);
	timer->setInterval(comboBoxHz->itemData(comboBoxHz->currentIndex()).toInt());
	connect(timer, SIGNAL(timeout()), this, SLOT(fetch_pilots()));
	fetch_pilots();
	if(checkBoxAutoRefresh->isChecked()){
		timer->start();
	}
}

void PilotsWidget::fetch_pilots()
{
	server_string = "";
	QUrl url("http://mpmap01.flightgear.org/fg_server_xml.cgi?mpserver01.flightgear.org:5001");
	QNetworkRequest request;
	request.setUrl(url );

	//TODO we need to check if already in a request.. ?
	//if(reply && reply->isRunning()){
	//	qDebug() << "request already running???";
	//	return;
	//}
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

}


//==========================================================
//= Server Events
//==========================================================
void PilotsWidget::on_server_error(QNetworkReply::NetworkError error){
	qDebug() << "error" << error;
	// TODO
}

void PilotsWidget::on_server_ready_read(){
	QString s(reply->readAll());
	server_string.append(s);
}

//=============================================
// Server call finished.. so parse to tree
//=============================================
/* we do not want to clear the tree and reload as user would loose focus..
   Also the last "communicate tiem means a pilot does not clear of the list"
   A net conection might drop and reappear later = crash or terrain terrain etc..
   SO current plan is to make a time stamp when a new pilot is gr
*/

void PilotsWidget::on_server_read_finished(){
	//qDebug() << "done"; // << server_string;


	//== Loop all ndes and set flag to 1 - item remaining will b enuked
	for(int idx=0; idx << tree->invisibleRootItem()->childCount(); idx++){
		tree->invisibleRootItem()->child(idx)->setText(C_FLAG, "1");
	}


	//= Create Dom Document
	QDomDocument dom;
	dom.setContent(server_string);

	//= get the <fg_server> node
	QDomNodeList nodes = dom.elementsByTagName("marker");
	QStringList list;

	QTreeWidgetItem *item;

	if (nodes.count() > 0){
		for(int idxd =0; idxd < nodes.count(); idxd++){

			QDomNode node = nodes.at(idxd);
			QDomNamedNodeMap attribs =  node.attributes();

			// = check if pilot in list or update
			QList<QTreeWidgetItem *> fitems = tree->findItems(attribs.namedItem("callsign").nodeValue(), Qt::MatchExactly, C_CALLSIGN);
			if(fitems.size() == 0){
				item = new QTreeWidgetItem();
				item->setText(C_CALLSIGN, attribs.namedItem("callsign").nodeValue());
				tree->addTopLevelItem(item);
			}else{
				item = fitems.at(0);
			}

			item->setText(C_AIRCRAFT, attribs.namedItem("model").nodeValue());
			item->setText(C_ALTITUDE, QString::number(attribs.namedItem("alt").nodeValue().toFloat(), 'f', 0));
			item->setTextAlignment(C_ALTITUDE, Qt::AlignRight);

			item->setText(C_HEADING, QString::number(attribs.namedItem("heading").nodeValue().toFloat(), 'f', 0));
			item->setTextAlignment(C_HEADING, Qt::AlignRight);

			item->setText(C_LAT, attribs.namedItem("lat").nodeValue());
			item->setTextAlignment(C_LAT, Qt::AlignRight);
			item->setText(C_LNG, attribs.namedItem("lng").nodeValue());
			item->setTextAlignment(C_LNG, Qt::AlignRight);

			item->setText(C_PITCH, attribs.namedItem("pitch").nodeValue());
			item->setText(C_FLAG, "");

		}
	}

	//= remove the flagged items
	QList<QTreeWidgetItem *> items = tree->findItems("1", Qt::MatchExactly, C_FLAG);
	for(int idxr=0; idxr << items.count(); idxr++){
		tree->invisibleRootItem()->removeChild(items.at(idxr));
	}

	//= Resize columns the first time (python does not have infunction statics like this ;-( ))
	/*
	static bool first_time_resize = false;
	if (first_time_resize == false){
		for(int cidx =0; cidx < tree->columnCount(); cidx++){
			tree->resizeColumnToContents(cidx);
		}
		first_time_resize = true;
	}
	*/
}


void PilotsWidget::on_check_autorefresh(int checked){
	mainObject->settings->setValue("mpxmap_autorefresh_enabled", checked);
	if(checked){
		fetch_pilots();
		timer->start();
	}else{
		timer->stop();
	}
}

void PilotsWidget::on_combo_changed(int idx){
	Q_UNUSED(idx);
	mainObject->settings->setValue("mpxmap_autorefresh_hz", comboBoxHz->itemData(comboBoxHz->currentIndex()).toString());
	timer->setInterval(comboBoxHz->itemData(comboBoxHz->currentIndex()).toInt());
}




