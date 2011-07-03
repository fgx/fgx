
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

	netMan = new QNetworkAccessManager(this);

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

	QAction *actionRefresh = new QAction(this);
	actionRefresh->setText("Refresh");
	actionRefresh->setIcon(QIcon(":/icon/refresh"));
	toolbar->addAction(actionRefresh);
	connect(actionRefresh, SIGNAL(triggered()), this, SLOT(fetch_pilots()));

	checkBoxAutoRefresh = new QCheckBox("Auto Refresh");
	toolbar->addWidget(checkBoxAutoRefresh);
	checkBoxAutoRefresh->setChecked(true);
	connect(checkBoxAutoRefresh, SIGNAL(stateChanged(int)), this, SLOT(on_check_autorefresh(int)));

	comboBoxHz = new QComboBox();
	toolbar->addWidget(comboBoxHz);
	for(int sex=1; sex < 10; sex++){
		comboBoxHz->addItem("5 seconds", QString::number(sex));
	}
	comboBoxHz->setCurrentIndex(3);
	connect(comboBoxHz, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combo_changed(int)));


	//=========================================================
	//== Tree ( Coeden )
	tree = new QTreeWidget();
	mainLayout->addWidget(tree);

	tree->setRootIsDecorated(false);
	tree->setUniformRowHeights(true);

	tree->headerItem()->setText(C_CALLSIGN, "Callsign");
	tree->headerItem()->setText(C_AIRCRAFT, "Aircraft");
	tree->headerItem()->setText(C_ALTITUDE, "Altitude");
	tree->headerItem()->setText(C_HEADING, "Heading");
	tree->headerItem()->setText(C_PITCH, "Pitch");
	tree->headerItem()->setText(C_LAT, "Lat");
	tree->headerItem()->setText(C_LNG, "Lng");
	tree->headerItem()->setText(C_FLAG, "Flag");



	statusBar = new QStatusBar();
	mainLayout->addWidget(statusBar);
	statusBar->showMessage("Click refresh to load");

}

void PilotsWidget::fetch_pilots()
{
	server_string = "";
	QUrl url("http://mpmap01.flightgear.org/fg_server_xml.cgi?mpserver01.flightgear.org:5001");
	QNetworkRequest request;
	request.setUrl(url );
	//qDebug() << url.toString();

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

void PilotsWidget::on_server_read_finished(){
	//qDebug() << "done"; // << server_string;

	// first mark the existing as flagged


	//= Create Dom Document
	QDomDocument dom;
	dom.setContent(server_string);

	//= get the <fg_server> node
	QDomNodeList nodes = dom.elementsByTagName("marker");
	QStringList list;
	qDebug() << nodes.length();
	if (nodes.count() > 0){
		for(int idxd =0; idxd < nodes.count(); idxd++){

			QTreeWidgetItem *item = new QTreeWidgetItem();
			tree->addTopLevelItem(item);

			QDomNode node = nodes.at(idxd);
			list << node.firstChildElement("marker").text();
			QDomNamedNodeMap attribs =  node.attributes();
			item->setText(C_CALLSIGN, attribs.namedItem("callsign").nodeValue());
			item->setText(C_AIRCRAFT, attribs.namedItem("model").nodeValue());
			item->setText(C_ALTITUDE, attribs.namedItem("alt").nodeValue());
			item->setText(C_HEADING, attribs.namedItem("heading").nodeValue());
			item->setText(C_PITCH, attribs.namedItem("pitch").nodeValue());
			item->setText(C_LAT, attribs.namedItem("lat").nodeValue());
			item->setText(C_LNG, attribs.namedItem("lng").nodeValue());

		}
	}

}


void PilotsWidget::on_check_autorefresh(int x){

}

void PilotsWidget::on_combo_changed(int idx){

}




