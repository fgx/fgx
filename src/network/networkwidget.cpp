

#include <QtCore/QString>
#include <QtCore/QChar>
#include <QtCore/QMap>
#include <QtCore/QMapIterator>
#include <QtCore/QUrl>

#include <QtCore/QProcess>

#include <QtNetwork/QHostInfo>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkInterface>

#include <QtGui/QDesktopServices>
#include <QtGui/QBrush>
#include <QtGui/QColor>

#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>

#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QLabel>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QHeaderView>
#include <QtGui/QToolButton>
#include <QtGui/QPushButton>

#include "network/networkwidget.h"
#include "network/mptelnet.h"
#include "utilities/helpers.h"



NetworkWidget::NetworkWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mOb;

    //* Main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setSpacing(0);
	int m = 10;
	mainLayout->setContentsMargins(m,m,m,m);


	//** MiddleLayout
	QHBoxLayout *middleLayout = new QHBoxLayout();
	mainLayout->addLayout(middleLayout);
	middleLayout->setSpacing(10);
	int mm = 10;
	middleLayout->setContentsMargins(mm,mm,mm,mm);

	//====================================================
	//== Mp IN
	QVBoxLayout *leftLayout = new QVBoxLayout();
	middleLayout->addLayout(leftLayout, 1);


	grpMpServerIn = new XGroupHBox("MultiPlayer In");
	grpMpServerIn->setCheckable(true);
	leftLayout->addWidget(grpMpServerIn);
	connect(grpMpServerIn, SIGNAL(toggled(bool)), this, SLOT(set_mp_server()));


	comboLocalIpAddress = new QComboBox();
	grpMpServerIn->addWidget(comboLocalIpAddress);
	connect(comboLocalIpAddress, SIGNAL(currentIndexChanged(int)), this, SLOT(set_mp_server()));

	grpMpServerIn->addWidget(new QLabel("port"));

	comboLocalPort = new QComboBox();
	comboLocalPort->addItem("5000", "5000");
	comboLocalPort->addItem("5001", "5001");
	comboLocalPort->setEditable(true);
	comboLocalPort->setValidator( new QIntValidator(1000, 30000, this) );
	grpMpServerIn->addWidget(comboLocalPort);
	connect(comboLocalPort, SIGNAL(currentIndexChanged(int)), this, SLOT(set_mp_server()));

	grpMpServerIn->addWidget(new QLabel("at"));
	comboHzIn = new QComboBox();
	grpMpServerIn->addWidget(comboHzIn);
	populate_combo_hz(comboHzIn);
	connect(comboHzIn, SIGNAL(currentIndexChanged(int)), this, SLOT(set_mp_server()));
	grpMpServerIn->addWidget(new QLabel("Hz"));

	grpMpServerIn->xLayout->addStretch(20);


	//===============================================
	//== Mp Out
	grpMpServerOut = new XGroupVBox("MultiPlayer Out");
	grpMpServerOut->setCheckable(true);
	leftLayout->addWidget(grpMpServerOut);
	connect(grpMpServerOut, SIGNAL(toggled(bool)), this, SLOT(set_mp_server()));

	QHBoxLayout *layMpOutControl = new QHBoxLayout();
	grpMpServerOut->addLayout(layMpOutControl);

	comboRemoteAddress = new QComboBox();
	comboRemoteAddress->addItem(tr("Use domain"), "domain");
	comboRemoteAddress->addItem(tr("Use IP"), "ip");
	comboRemoteAddress->setCurrentIndex(0);
	layMpOutControl->addWidget(comboRemoteAddress);
	connect(comboRemoteAddress, SIGNAL(currentIndexChanged(int)), this, SLOT(set_mp_server()));

	layMpOutControl->addWidget(new QLabel("port"));

	comboRemotePort = new QComboBox();
	comboRemotePort->addItem("5000", "5000");
	comboRemotePort->addItem("5001", "5001");
	comboRemotePort->setEditable(true);
	comboRemotePort->setValidator( new QIntValidator(1000, 30000, this) );
	layMpOutControl->addWidget(comboRemotePort);
	connect(comboRemotePort, SIGNAL(currentIndexChanged(int)), this, SLOT(set_mp_server()));

	layMpOutControl->addWidget(new QLabel("at"));
	comboHzOut = new QComboBox();
	layMpOutControl->addWidget(comboHzOut);
	populate_combo_hz(comboHzOut);
	connect(comboHzOut, SIGNAL(currentIndexChanged(int)), this, SLOT(set_mp_server()));

	layMpOutControl->addWidget(new QLabel("Hz"));

	layMpOutControl->addStretch(20);

	QVBoxLayout *mpTreeLayout = new QVBoxLayout();
	grpMpServerOut->addLayout(mpTreeLayout);

	//=======================================
	//* TreeWidget
    treeWidget = new QTreeWidget();
	mpTreeLayout->addWidget(treeWidget, 100);
    treeWidget->setAlternatingRowColors(true);
    treeWidget->setRootIsDecorated(false);
	treeWidget->setSortingEnabled(true);
	treeWidget->sortByColumn(C_SERVER_NAME, Qt::AscendingOrder);
	//treeWidget->setMinimumWidth();

    QTreeWidgetItem * headerItem = treeWidget->headerItem();
    headerItem->setText(C_SERVER_NAME, tr("Name"));
    headerItem->setText(C_DOMAIN, tr("Domain"));
    headerItem->setText(C_IP_ADDRESS, tr("IP Address"));
    headerItem->setText(C_PILOTS_COUNT, tr("Pilots"));
	headerItem->setText(C_FLAG, "-");
    treeWidget->header()->setStretchLastSection(true);
	treeWidget->setColumnWidth(C_SERVER_NAME, 100);
    treeWidget->setColumnWidth(C_DOMAIN, 100);
	treeWidget->setColumnWidth(C_PILOTS_COUNT, 50);

	treeWidget->setColumnHidden(C_DOMAIN, true);
	treeWidget->setColumnHidden(C_FLAG, true);
	//treeWidget->setColumnHidden(C_PILOTS_COUNT, true);

	connect(treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(set_mp_server()));

	//************* Bottom Layout
	QHBoxLayout *layoutBottomTreeBar = new QHBoxLayout();
	mpTreeLayout->addLayout(layoutBottomTreeBar);
	layoutBottomTreeBar->addStretch(30);

	//* refresh MP servers
	QToolButton *refreshButton = new QToolButton(this);
	layoutBottomTreeBar->addWidget(refreshButton);
	refreshButton->setText("Refresh Server List");
	refreshButton->setAutoRaise(true);
	refreshButton->setIcon(QIcon(":/icon/load"));
	refreshButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(refreshButton, SIGNAL(clicked()), this, SLOT(dns_lookup()) );



	//=================================================================================================
	// RIGHT side >>>>
	//=================================================================================================
	QVBoxLayout *rightLayout = new QVBoxLayout();
	rightLayout->setSpacing(20);
	middleLayout->addLayout(rightLayout, 1);

	//========================================================================
	// FgCom Box
	grpFgCom = new XGroupGBox(tr("FGCom - Voice Communications"));
	rightLayout->addWidget(grpFgCom, 2);
	grpFgCom->setCheckable(true);
	grpFgCom->setChecked(false);
	connect(grpFgCom, SIGNAL(clicked(bool)), this, SLOT(set_fgcom()));

	QString style("font-size: 8pt; color: #666666;");

	// fgCom NO
	int row = 0;
	grpFgCom->addWidget(new QLabel("Server"), row, 0, 1, 1, Qt::AlignRight);
	txtFgComNo = new QLineEdit();
	grpFgCom->addWidget(txtFgComNo, row, 1, 1, 1);
	connect(txtFgComNo, SIGNAL(textChanged(QString)), this, SLOT(set_fgcom()));

	row++;
	QLabel *lblHelp1 = new QLabel("eg: fgcom.flightgear.org.uk");
	lblHelp1->setStyleSheet(style);
	grpFgCom->addWidget(lblHelp1, row, 1, 1, 2);

	//* fgCom Port
	row++;
	grpFgCom->addWidget(new QLabel("Port"), row, 0, 1, 1, Qt::AlignRight);
	txtFgComPort = new QLineEdit();
	txtFgComPort->setMaximumWidth(100);
	grpFgCom->addWidget(txtFgComPort, row, 1, 1, 1);
	connect(txtFgComPort, SIGNAL(textChanged(QString)), this, SLOT(set_fgcom()));

	row++;
	QLabel *lblHelp2 = new QLabel("eg: 16661");
	lblHelp2->setStyleSheet(style);
	grpFgCom->addWidget(lblHelp2, row, 1, 1, 2);



	//===========================================================
	//** Telnet
	grpTelnet = new XGroupHBox(tr("Telnet Properties Server"));
	grpTelnet->setCheckable(true);
	grpTelnet->setChecked(false);
	rightLayout->addWidget(grpTelnet);
	connect(grpTelnet, SIGNAL(clicked()), this, SLOT(on_telnet()));

	grpTelnet->xLayout->setSpacing(10);
	grpTelnet->xLayout->setContentsMargins(m,m,m,m);

	QLabel *lblTelnet = new QLabel();
	lblTelnet->setText(tr("Set Port No:"));
	grpTelnet->addWidget( lblTelnet);

	txtTelnet = new QLineEdit("5500");
	txtTelnet->setValidator(new QIntValidator(80, 32000, this));
	grpTelnet->addWidget(txtTelnet);
	connect(txtTelnet, SIGNAL(textChanged(QString)), this, SLOT(on_telnet()));

	QToolButton *buttTelnet = new QToolButton();
	grpTelnet->addWidget(buttTelnet);
	buttTelnet->setIcon(QIcon(":/icon/terminal"));
	buttTelnet->setToolButtonStyle(Qt::ToolButtonIconOnly);
	connect(buttTelnet, SIGNAL(clicked()), this, SLOT(on_open_telnet()));
	buttTelnet->hide();

	//===========================================================
	//** Screenshot
	grpScreenShot = new XGroupHBox(tr("Screen Shot Server"));
	grpScreenShot->setCheckable(true);
	grpScreenShot->setChecked(false);
	rightLayout->addWidget(grpScreenShot);
	connect(grpScreenShot, SIGNAL(clicked()), this, SLOT(on_screenshot()) );

	grpScreenShot->xLayout->setSpacing(10);
	grpScreenShot->xLayout->setContentsMargins(m,m,m,m);
	grpScreenShot->addWidget( new QLabel(tr("Port No:")));

	txtScreenShot = new QLineEdit("8088");
	txtScreenShot->setValidator(new QIntValidator(80, 32000, this));
	grpScreenShot->addWidget(txtScreenShot);
	connect(txtScreenShot, SIGNAL(textChanged(QString)), this, SLOT(on_screenshot()));

	QToolButton *buttScreenshot = new QToolButton();
	grpScreenShot->addWidget(buttScreenshot);
	buttScreenshot->setIcon(QIcon(":/icon/browse"));
	buttScreenshot->setToolButtonStyle(Qt::ToolButtonIconOnly);
	connect(buttScreenshot, SIGNAL(clicked()), this, SLOT(on_browse_screenshot()));

	//==========================================================
	//** HTTP
	grpHttp = new XGroupHBox(tr("HTTP Web Server"));
	grpHttp->setCheckable(true);
	grpHttp->setChecked(false);
	rightLayout->addWidget(grpHttp);
	connect(grpHttp, SIGNAL(clicked()), this, SLOT(on_http()));


	grpHttp->addWidget( new QLabel(tr("Port No:")) );

	txtHttp = new QLineEdit("8080");
	txtHttp->setValidator(new QIntValidator(80, 32000, this));
	grpHttp->addWidget(txtHttp);
	connect(txtHttp, SIGNAL(textChanged(QString)), this, SLOT(on_http()));

	QToolButton *butHttp = new QToolButton();
	grpHttp->addWidget(butHttp);
	butHttp->setIcon(QIcon(":/icon/browse"));
	butHttp->setToolButtonStyle(Qt::ToolButtonIconOnly);
	connect(butHttp, SIGNAL(clicked()), this, SLOT(on_browse_http()));


	rightLayout->addStretch(20);


	//** Setup network stuff
	dns_lookup();
	load_local_addresses();
	//set_mp_server();

	//== Connect Settings
	connect(this, SIGNAL(setx(QString,bool,QString)), mainObject->X, SLOT(set_option(QString,bool,QString)) );
	connect(mainObject->X, SIGNAL(upx(QString,bool,QString)), this, SLOT(on_upx(QString,bool,QString)));


}
/* end constructor */


//=============================================================
//** Network Addresses
void NetworkWidget::load_local_addresses(){
	QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
	for (int i = 0; i < addresses.size(); ++i) {
		if(addresses.at(i).protocol() == QAbstractSocket::IPv4Protocol){
			comboLocalIpAddress->addItem( addresses.at(i).toString(), addresses.at(i).toString() );
		}
	}
}



//=============================================================
//** Dns Lookup All
void NetworkWidget::dns_lookup(){
	treeWidget->model()->removeRows(0, treeWidget->model()->rowCount());
	for(int i=1; i < 25; i++){
		QString domain_name = QString("mpserver%1.flightgear.org").arg(i, 2, 10, QChar('0'));
		QHostInfo::lookupHost(domain_name, this, SLOT(on_dns_lookup_callback(QHostInfo)));
    }
}


// DNS Callback
void NetworkWidget::on_dns_lookup_callback(const QHostInfo &hostInfo){

	if(hostInfo.addresses().count() == 0){
		return;
	}
	QTreeWidgetItem *newItem = new QTreeWidgetItem();
	newItem->setText(C_SERVER_NAME, hostInfo.hostName().split(".")[0]);
	newItem->setText(C_DOMAIN, hostInfo.hostName());
	newItem->setText(C_IP_ADDRESS, hostInfo.addresses().first().toString());
	newItem->setText(C_PILOTS_COUNT, "-");
	treeWidget->addTopLevelItem(newItem);
	QString serv = mainObject->X->getx("--multiplay=out");
	if(serv.indexOf(",") > 0){
		if(serv.split(",",QString::SkipEmptyParts).at(1) == hostInfo.hostName()){
			treeWidget->setCurrentItem(newItem);
		}
	}
	return;

	// TODO Connect to telnet and get players list
	MpTelnet *telnet = new MpTelnet(this );
	telnet->get_info(hostInfo.addresses().first().toString());
	connect(telnet, SIGNAL(telnet_data(QString, QString)),
			this, SLOT(on_telnet_data(QString, QString))
	);
}

/* Example Output >>
Trying 85.214.37.14...
Connected to mpserver01.flightgear.org.
Escape character is '^]'.
# This is mpserver01
# FlightGear Multiplayer Server v0.10.3 using protocol version v1.1 (LazyRelay enabled)
# This server is tracked: 62.112.194.20
# 38 pilots online
airdomi@LOCAL: 4107968.717705 3824275.355909 3041539.588760 28.615373 42.951713 34195.591451 -3.696031 1.916703 0.573010 Aircraft/777-200/Models/777-200ER.xml
minaya@mpserver10: 4008833.892465 -10727.752073 4944410.518456 51.153549 -0.153324 562.061917 -2.642317 2.096125 1.046300 Aircraft/777-200/Models/777-200ER.xml
*/
void NetworkWidget::on_telnet_data(QString ip_address, QString telnet_reply){

    //** Split the reply into lines and parse each line
    QStringList lines = telnet_reply.split("\n");
    QString line;
    int pilots_count = 0;

    for(int i = 0; i < lines.size(); ++i){

        line = lines.at(i).trimmed();
        if(i == 0){
            // skip first line
        }else if(line.startsWith("#")){
            // skip lines starting with #
        }else if(line.length() == 0){
            // skip blank lines
        }else{

            //*** Process the line
			QString mp_server = line.split(" ").at(0).split("@").at(1);
            mp_server = mp_server.replace(QString(":"), QString("")); //* get rid of trailing ":" at end eg a.b.c.192:
            //** Map to an IP address
            if(mp_server == "LOCAL"){
                //* Its ip_address from callback
				pilots_count++;
			}
        }
    } /* for lines() */

    //** Update the Pilots Count
    QList<QTreeWidgetItem*> items = treeWidget->findItems(ip_address, Qt::MatchExactly, C_IP_ADDRESS);
    items[0]->setText(C_PILOTS_COUNT, QString::number(pilots_count));
	items[0]->setText(C_FLAG, "1");
}




//=====================================
// Mp Server Selected
void NetworkWidget::set_mp_server(){
	QString in(",");
	in.append(comboHzIn->currentText());
	in.append(",");
	in.append(comboLocalIpAddress->currentText());
	in.append(",").append(comboLocalPort->currentText());

	emit setx("--multiplay=in", grpMpServerIn->isChecked(), in);


	//if(!treeWidget->currentIndex()){
		//treeWidget->setCu
	//}
	QTreeWidgetItem *item = treeWidget->currentItem();
	//bool mp_out_valid = !item; //or item->text(C_FLAG).length() == 0;
	if(!item){ // or item->text(C_FLAG).length() == 0){
		//emit setx("--multiplay=in", grpMpServerIn->isChecked(), in);
		return;
	}

	QString out(",");
	out.append(comboHzOut->currentText());
	out.append(",");
	out.append(  item->text(C_IP_ADDRESS) );
	out.append(",").append(comboRemotePort->currentText());

	//=out,10,server.ip.address,5000
	emit setx("--multiplay=out", grpMpServerOut->isChecked(), out);


}


//=====================================
// Setup fgCom
void NetworkWidget::set_fgcom(){
	//emit setx( "--fgcom=", grpFgCom->isChecked(), txtFgComNo->text().append(":").append( txtFgComPort->text() ) );
	emit setx( "fgcom_generic_socket",
				grpFgCom->isChecked(),
				QString("--generic=socket,out,10,localhost,%1,udp,fgcom").arg(txtFgComPort->text())
			);
}
		
		
//=====================================
// Setup Combo Hz
void NetworkWidget::populate_combo_hz(QComboBox *combo){
	for(int i=5; i < 15; i++){
		combo->addItem(QString("%1").arg(i), QString("%1").arg(i));
	}
	combo->setCurrentIndex(4);
}


//=================================================
// Open Browsers
void NetworkWidget::on_browse_http(){
	QString url = QString("http://localhost:%1").arg(txtHttp ->text());
	QDesktopServices::openUrl(QUrl(url));
}

void NetworkWidget::on_browse_screenshot(){
	QString url = QString("http://localhost:%1").arg(txtScreenShot ->text());
	QDesktopServices::openUrl(QUrl(url));
}

//=================================================
// Open Telnet
void NetworkWidget::on_open_telnet(){
	// TODO
}




//=============================================================
// Validate
QString NetworkWidget::validate(){
	if(grpMpServerOut->isChecked()){
		if(!treeWidget->currentItem()){
			treeWidget->setFocus();
			return QString("No multiplayer out server selected");;
		}
	}
	return QString("");
}



//================================================================

void NetworkWidget::on_telnet()
{
	emit setx("--telnet=", grpTelnet->isChecked(), txtTelnet->text());
}

void NetworkWidget::on_http()
{
	emit setx("--httpd=", grpHttp->isChecked(), txtHttp->text());
}

void NetworkWidget::on_screenshot()
{
	emit setx("--jpg-httpd=", grpScreenShot->isChecked(), txtScreenShot->text());
}



//=============================================================
// Update Widgets
void NetworkWidget::on_upx(QString option, bool enabled, QString value)
{


	if(option == "--multiplay=in" || option == "--multiplay=out"){

		// --multiplay=out,10,server.ip.address,5000
		if(value.contains(",")){
			QStringList parts;
			parts = value.split(",",QString::SkipEmptyParts);

			if(option == "--multiplay=in"){
				grpMpServerIn->setChecked(enabled);
				Helpers::select_combo(comboLocalIpAddress, parts.at(1));
				Helpers::select_combo(comboHzIn, parts.at(0));
				Helpers::select_combo(comboLocalPort, parts.at(2));

			}else{
				grpMpServerOut->setChecked(enabled);
				Helpers::select_combo(comboHzOut, parts.at(0));
				Helpers::select_combo(comboRemotePort, parts.at(2));
				QList<QTreeWidgetItem *> items = treeWidget->findItems(parts.at(1), Qt::MatchExactly, C_IP_ADDRESS);
				if(items.size() > 0){
					treeWidget->setCurrentItem(items.at(0));
				}
			}
		}


	}else if(option == "--fgcom="){
		grpFgCom->setChecked(enabled);
		if(value.contains(":")){
			txtFgComNo->setText( value.split(":").at(0));
			txtFgComPort->setText( value.split(":").at(1));
		}

	}else if(option == "--telnet="){
		grpTelnet->setChecked(enabled);
		txtTelnet->setText(value);


	}else if(option == "--httpd="){
		grpHttp->setChecked(enabled);
		txtHttp->setText(value);


	}else if(option == "--jpg-httpd="){
		grpScreenShot->setChecked(enabled);
		txtScreenShot->setText(value);

	}
}
