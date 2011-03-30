

#include <QtCore/QString>
#include <QtCore/QChar>
#include <QtCore/QMap>
#include <QtCore/QMapIterator>




#include <QtNetwork/QHostInfo>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkInterface>

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

#include "network/networkwidget.h"
#include "network/mptelnet.h"

/* list the Mp Servers which are gotten from a DNS lookup

*/


NetworkWidget::NetworkWidget(QWidget *parent) :
    QWidget(parent)
{

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

	//========================================================================
	// Mp Servers Box
	grpMpServer  = new QGroupBox("Enable Multiplayer");
	middleLayout->addWidget(grpMpServer, 3);
	grpMpServer->setCheckable(true);
	connect(grpMpServer, SIGNAL(clicked(bool)), this, SLOT(on_mp_server_checked(bool)));

	QVBoxLayout *layoutMpServer = new QVBoxLayout();
	grpMpServer->setLayout(layoutMpServer);


	//**  Grid =========================================
	QGridLayout *gridMP = new QGridLayout();
	layoutMpServer->addLayout(gridMP);

	int row = 0;
	//** Callsign
	gridMP->addWidget(new QLabel("Callsign:"), row, 0, 1, 1, Qt::AlignRight);
	txtCallSign = new QLineEdit();
	txtCallSign->setText("");
	txtCallSign->setMaximumWidth(100);
	txtCallSign->setMaxLength(7);
	gridMP->addWidget(txtCallSign, row, 1, 1, 1);
	connect(txtCallSign, SIGNAL(textChanged(QString)), this, SLOT(on_callsign_changed(QString)) );




	//** Mop IN
	row++;
	checkBoxIn = new QCheckBox("In from");
	gridMP->addWidget(checkBoxIn, row, 0);
	connect(checkBoxIn, SIGNAL(clicked()), this, SLOT(on_checkbox_in()));

	comboLocalIpAddress = new QComboBox();
	gridMP->addWidget(comboLocalIpAddress, row, 1);
	connect(comboLocalIpAddress, SIGNAL(currentIndexChanged(int)), this, SLOT(set_mp_server()));

	gridMP->addWidget(new QLabel("port"), row, 2);

	comboLocalPort = new QComboBox();
	comboLocalPort->addItem("5000");
	comboLocalPort->addItem("5001");
	comboLocalPort->setEditable(true);
	comboLocalPort->setValidator( new QIntValidator(1000, 30000, this) );
	gridMP->addWidget(comboLocalPort, row, 3);
	connect(comboLocalPort, SIGNAL(currentIndexChanged(int)), this, SLOT(set_mp_server()));

	gridMP->addWidget(new QLabel("at"), row, 4);
	comboHzIn = new QComboBox();
	gridMP->addWidget(comboHzIn, row, 5);
	populate_combo_hz(comboHzIn);
	connect(comboHzIn, SIGNAL(currentIndexChanged(int)), this, SLOT(set_mp_server(bool)));
	gridMP->addWidget(new QLabel("Hz"), row, 6);


	//** Mp Out
	row++;
	checkBoxOut = new QCheckBox("Out to");
	gridMP->addWidget(checkBoxOut, row, 0);
	connect(checkBoxOut, SIGNAL(clicked()), this, SLOT(on_checkbox_out()));

	comboRemoteAddress = new QComboBox();
	comboRemoteAddress->addItem(tr("Use domain"), "domain");
	comboRemoteAddress->addItem(tr("Use IP"), "ip");
	comboRemoteAddress->setCurrentIndex(0);
	gridMP->addWidget(comboRemoteAddress, row, 1);
	connect(comboRemoteAddress, SIGNAL(currentIndexChanged(int)), this, SLOT(set_mp_server()));

	gridMP->addWidget(new QLabel("port"), row, 2);

	comboRemotePort = new QComboBox();
	comboRemotePort->addItem("5000");
	comboRemotePort->addItem("5001");
	comboRemotePort->setEditable(true);
	comboRemotePort->setValidator( new QIntValidator(1000, 30000, this) );
	gridMP->addWidget(comboRemotePort, row, 3);
	connect(comboRemotePort, SIGNAL(currentIndexChanged(int)), this, SLOT(set_mp_server()));

	gridMP->addWidget(new QLabel("at"), row, 4);
	comboHzOut = new QComboBox();
	gridMP->addWidget(comboHzOut, row, 5);
	populate_combo_hz(comboHzOut);
	connect(comboHzOut, SIGNAL(currentIndexChanged(int)), this, SLOT(set_mp_server()));
	gridMP->addWidget(new QLabel("Hz"), row, 6);


	gridMP->setColumnStretch(6,5);


	//=======================================
	//* TreeWidget
    treeWidget = new QTreeWidget();
	layoutMpServer->addWidget(treeWidget, 100);
    treeWidget->setAlternatingRowColors(true);
    treeWidget->setRootIsDecorated(false);


    QTreeWidgetItem * headerItem = treeWidget->headerItem();
    headerItem->setText(C_SERVER_NO, tr("No"));
    headerItem->setText(C_SERVER_NAME, tr("Name"));
    headerItem->setText(C_DOMAIN, tr("Domain"));
    headerItem->setText(C_IP_ADDRESS, tr("IP Address"));
    headerItem->setText(C_PILOTS_COUNT, tr("Pilots"));
	headerItem->setText(C_FLAG, "-");
    treeWidget->header()->setStretchLastSection(true);
    treeWidget->setColumnWidth(C_SERVER_NO, 40);
	treeWidget->setColumnWidth(C_SERVER_NAME, 80);
    treeWidget->setColumnWidth(C_DOMAIN, 100);
	treeWidget->setColumnWidth(C_PILOTS_COUNT, 50);
	treeWidget->setColumnWidth(C_FLAG, 10);

	treeWidget->setColumnHidden(C_DOMAIN, true);
	treeWidget->setColumnHidden(C_SERVER_NO, true);
	treeWidget->setColumnHidden(C_FLAG, true);
	treeWidget->setColumnHidden(C_PILOTS_COUNT, true);

	connect(treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(set_mp_server()));

	//************* Bottom Layout
	QHBoxLayout *layoutBottomTreeBar = new QHBoxLayout();
	layoutMpServer->addLayout(layoutBottomTreeBar);
	layoutBottomTreeBar->addStretch(30);

	//* refresh MP servers
	QToolButton *refreshButton = new QToolButton(this);
	layoutBottomTreeBar->addWidget(refreshButton);
	refreshButton->setIcon(QIcon(":/icon/refresh"));
	refreshButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	connect(refreshButton, SIGNAL(clicked()), this, SLOT(dns_lookup_all()) );



	//****************** RIGHT ********************************
	QVBoxLayout *rightLayout = new QVBoxLayout();
	rightLayout->setSpacing(20);
	middleLayout->addLayout(rightLayout, 2);

	//========================================================================
	// FgCom Box
	grpFgCom = new QGroupBox(tr("fgCom - Voice Communications"));
	rightLayout->addWidget(grpFgCom, 2);
	grpFgCom->setCheckable(true);
	connect(grpFgCom, SIGNAL(clicked(bool)), this, SLOT(set_fgcom()));

	QVBoxLayout *layoutFgCom = new QVBoxLayout();
	grpFgCom->setLayout(layoutFgCom);

	QString style("font-size: 8pt; color: #666666;");

	// fgCom NO
	txtFgComNo = new QLineEdit();
	layoutFgCom->addWidget(txtFgComNo);
	connect(txtFgComNo, SIGNAL(textChanged(QString)), this, SLOT(set_fgcom()));

	QLabel *lblHelp1 = new QLabel("Call default FlightGear fgCom server");
	lblHelp1->setStyleSheet(style);
	layoutFgCom->addWidget(lblHelp1);


	layoutFgCom->addSpacing(10);


	//* fgCom Port
	txtFgComPort = new QLineEdit();
	txtFgComPort->setMaximumWidth(100);
	layoutFgCom->addWidget(txtFgComPort);
	connect(txtFgComPort, SIGNAL(textChanged(QString)), this, SLOT(set_fgcom()));

	QLabel *lblHelp2 = new QLabel("Default fgCom UDP port");
	lblHelp2->setStyleSheet(style);
	layoutFgCom->addWidget(lblHelp2);


	//===========================================================
	//** Telnet
	grpTelnet = new QGroupBox();
	grpTelnet->setTitle(tr("Telnet Properties Server"));
	grpTelnet->setCheckable(true);
	grpTelnet->setChecked(false);
	rightLayout->addWidget(grpTelnet);
	connect(grpTelnet, SIGNAL(clicked()), this, SLOT(set_telnet()));

	QHBoxLayout *layoutNetTelnet = new QHBoxLayout();
	grpTelnet->setLayout(layoutNetTelnet);
	layoutNetTelnet->setSpacing(10);
	//int m = 5;
	layoutNetTelnet->setContentsMargins(m,m,m,m);

	QLabel *lblTelnet = new QLabel();
	lblTelnet->setText(tr("Set Port No:"));
	layoutNetTelnet->addWidget( lblTelnet);

	txtTelnet = new QLineEdit("5500");
	txtTelnet->setValidator(new QIntValidator(80, 32000, this));
	layoutNetTelnet->addWidget(txtTelnet);
	connect(txtTelnet, SIGNAL(textChanged(QString)), this, SLOT(set_telnet()));


	//===========================================================
	//** Telnet
	grpScreenShot = new QGroupBox();
	grpScreenShot->setTitle(tr("Screen Shot Server"));
	grpScreenShot->setCheckable(true);
	grpScreenShot->setChecked(false);
	rightLayout->addWidget(grpScreenShot);
	connect(grpScreenShot, SIGNAL(clicked()), this, SLOT(set_screenshot()));

	QHBoxLayout *layoutScreenShot = new QHBoxLayout();
	grpScreenShot->setLayout(layoutScreenShot);
	layoutScreenShot->setSpacing(10);
	//int m = 5;
	layoutScreenShot->setContentsMargins(m,m,m,m);

	QLabel *lblScreenshot = new QLabel();
	lblScreenshot->setText(tr("Set Port No:"));
	layoutScreenShot->addWidget( lblScreenshot);

	txtScreenShot = new QLineEdit("7777");
	txtScreenShot->setValidator(new QIntValidator(80, 32000, this));
	layoutScreenShot->addWidget(txtScreenShot);
	connect(txtScreenShot, SIGNAL(textChanged(QString)), this, SLOT(set_screenshot()));

	//==========================================================
	//** HTTP
	grpHttp = new QGroupBox();
	grpHttp->setTitle(tr("HTTP Web Server"));
	grpHttp->setCheckable(true);
	grpHttp->setChecked(false);
	rightLayout->addWidget(grpHttp);
	connect(grpHttp, SIGNAL(clicked(bool)), this, SLOT(set_http()));

	QHBoxLayout *layoutNetHttp = new QHBoxLayout();
	grpHttp->setLayout(layoutNetHttp);
	layoutNetHttp->setSpacing(10);
	//int m = 5;
	//layoutNetHttp->setContentsMargins(m,m,m,m);

	QLabel *lblHttp = new QLabel();
	lblHttp->setText(tr("Set Port No:"));
	layoutNetHttp->addWidget( lblHttp);

	txtHttp = new QLineEdit("8080");
	txtHttp->setValidator(new QIntValidator(80, 32000, this));
	layoutNetHttp->addWidget(txtHttp);
	connect(txtHttp, SIGNAL(textChanged(QString)), this, SLOT(set_http()));

	QToolButton *butHttp = new QToolButton();
	layoutNetHttp->addWidget(butHttp);
	butHttp->setIcon(QIcon(":/icons/dns_lookup"));
	butHttp->setToolButtonStyle(Qt::ToolButtonIconOnly);
	butHttp->setPopupMode(QToolButton::InstantPopup);

	QMenu *menuNetHttp = new QMenu();
	butHttp->setMenu(menuNetHttp);

	QAction *actHttpBrowse = new QAction(menuNetHttp);
	menuNetHttp->addAction(actHttpBrowse);
	actHttpBrowse->setText(tr("Open in external browser"));
	//connect(actExePath, SIGNAL(triggered()), this, SLOT(on_exe_path()));


	rightLayout->addStretch(20);


	//** Setup network stuff
	dns_lookup_all();
	load_addresses();
	on_checkbox_in();
	on_checkbox_out();


}
/* end constructor */


//=============================================================
//** Network Addresses
void NetworkWidget::load_addresses(){
	QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
	for (int i = 0; i < addresses.size(); ++i) {
		if(addresses.at(i).protocol() == QAbstractSocket::IPv4Protocol){
			comboLocalIpAddress->addItem( addresses.at(i).toString() );
		}
	}
}



//=============================================================
//** Dns Lookup All
void NetworkWidget::dns_lookup_all(){
	for(int i=1; i < 25; i++){
        dns_lookup(i);
    }
}

//** Dns Lookup (server_no)
void NetworkWidget::dns_lookup(int server_int){

    //* make server info
    QString domain_name = QString("mpserver%1.flightgear.org").arg(server_int, 2, 10, QChar('0'));
    QString server_name = QString("mpserver%1").arg(server_int, 2, 10, QChar('0'));
    QString server_no = QString("%1").arg(server_int, 2, 10, QChar('0'));


    //* find domain_name in tree and add if not there
    QList<QTreeWidgetItem*> items = treeWidget->findItems(domain_name, Qt::MatchExactly, C_DOMAIN);
    if(items.count() == 0){
        QTreeWidgetItem *newItem = new QTreeWidgetItem();
        newItem->setText(C_SERVER_NO, server_no);
        newItem->setText(C_SERVER_NAME, server_name);
        newItem->setText(C_DOMAIN, domain_name);

		newItem->setText(C_IP_ADDRESS, tr("Looking up"));
		QBrush b = newItem->foreground(NetworkWidget::C_IP_ADDRESS);
        b.setColor(QColor(100, 100, 100));
        newItem->setForeground(C_IP_ADDRESS, b);

        int newIdx = treeWidget->invisibleRootItem()->childCount();
        treeWidget->insertTopLevelItem(newIdx, newItem);
    }
    //* execute lookup
    QHostInfo::lookupHost(domain_name, this, SLOT(on_dns_lookup_host(QHostInfo)));

}

// DNS Callback
void NetworkWidget::on_dns_lookup_host(const QHostInfo &hostInfo){

    //* Find row matching by domain
    QList<QTreeWidgetItem*> items = treeWidget->findItems(hostInfo.hostName(), Qt::MatchExactly, C_DOMAIN);

    //** Make the colors change if address found
    QString lbl;
    QColor color;
    bool has_address = hostInfo.addresses().count() > 0;
    if( has_address ){
         lbl = hostInfo.addresses().first().toString();
         color = QColor(0, 150, 0);
     }else{
		lbl = tr("No Entry");
		color = QColor(150, 150, 150);
     }
     QBrush brush = items[0]->foreground(C_IP_ADDRESS);
     brush.setColor(color);
     items[0]->setForeground(C_IP_ADDRESS, brush);
     items[0]->setText(C_IP_ADDRESS, lbl);

     if(has_address){
		items[0]->setText(C_FLAG, "1");
		QString ip_or_domain( comboRemoteAddress->itemData(comboRemoteAddress->currentIndex(), Qt::UserRole).toString());
		QList<QTreeWidgetItem*> selItems = treeWidget->findItems(settings.value("mpserver").toString(),
																	Qt::MatchExactly,
																	ip_or_domain == "domain" ? C_DOMAIN : C_IP_ADDRESS);
		if(selItems.length() > 0){
			treeWidget->setCurrentItem(selItems[0]);
		}

		/* TODO
        MpTelnet *telnet = new MpTelnet(this );
        telnet->get_info(hostInfo.addresses().first().toString());
        connect(telnet, SIGNAL(telnet_data(QString, QString)),
                this, SLOT(on_telnet_data(QString, QString))
        );
		*/
        //TODO catch no return ? error ?
     }
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
    QMap<QString, int> mServerCount;

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
            QStringList parts = line.split(" ");
            QString callsign = parts.at(0).split("@").at(0);
            QString mp_server = parts.at(0).split("@").at(1);
            QString mp_server_ip = QString("");
            mp_server = mp_server.replace(QString(":"), QString("")); //* get rid of trailing ":" at end eg a.b.c.192:

            //** Map to an IP address
            if(mp_server == "LOCAL"){
                //* Its ip_address from callback
                mp_server_ip = QString(ip_address);

            }else if(mp_server.startsWith("mpserver")){
                //* its mpserver__ so find in tree if avaiable
                // TODO insert unfound ip
                QList<QTreeWidgetItem*> items = treeWidget->findItems(mp_server, Qt::MatchExactly, C_SERVER_NAME);
                /*
                if(items.count() == 0){
                    mp_server_ip = "NOT_FOUND";
                    QTreeWidgetItem *newItem = new QTreeWidgetItem();
                    newItem->setText(C_IP_ADDRESS,ip_address);
                    newItem->setText(C_DOMAIN, "?");
                    treeWidget->addTopLevelItem(newItem);
                }else{
                    mp_server_ip = items[0]->text(C_IP_ADDRESS);
                }*/
                //qDebug() << "Found: " << mp_server << " = " << items.count();
            }else{
                //* Otherwise we got returned an ip address
                mp_server_ip = mp_server;
            }
            //** Increment pilot count
            if(mServerCount.contains(mp_server_ip)){
                mServerCount[mp_server_ip] = mServerCount[mp_server_ip] + 1;
            }else{
                mServerCount[mp_server_ip] = 1;
            }


            pilots_count++;

        }
    } /* for lines() */
    QMapIterator<QString, int> i(mServerCount);
    while (i.hasNext()) {
        i.next();
        QList<QTreeWidgetItem*> items = treeWidget->findItems(i.key(), Qt::MatchExactly, C_IP_ADDRESS);
        if(items.count() == 1){
		   // qDebug() << "FoundRow" << i.key() ;
        }else{
			//qDebug() << "Row no found" << i.key();
        }
        //qDebug() << i.key() << ": " << i.value() << endl;
    }
    //** Update the Pilots Count
    QList<QTreeWidgetItem*> items = treeWidget->findItems(ip_address, Qt::MatchExactly, C_IP_ADDRESS);
    //treeWidget->removeItemWidget(items[0], C_PILOTS_COUNT);
    items[0]->setText(C_PILOTS_COUNT, QString::number(pilots_count));
    //qDebug() >> "update=" << items.count();
}




//=====================================
// Mp Server Enabled
void NetworkWidget::on_mp_server_checked(bool state){
	if (state){
		txtCallSign->setFocus();
	}
}


//=====================================
// Callsign Changed
void NetworkWidget::on_callsign_changed(QString txt){
	QString callsign = txt.trimmed();
	if(callsign.length() == 0){
		emit set_arg("remove", "--callsign=","");


	}else{
		emit set_arg("set", "--callsign=",callsign);
	}
}

//=====================================
// Mp Server Selected
void NetworkWidget::set_mp_server(){

	QTreeWidgetItem *item = treeWidget->currentItem();

	if(!item or item->text(C_FLAG).length() == 0){
		//* No Muliplayer server selected to no multiplay
		emit set_arg("remove", "--multiplay=out", "");
		emit set_arg("remove", "--multiplay=in", "");
		return;
	}

	QString out(",");
	out.append(comboHzOut->currentText());
	out.append(",");
	out.append( comboRemoteAddress->itemData(comboRemoteAddress->currentIndex(),Qt::UserRole) == "domain"
				? item->text(C_DOMAIN)
				: item->text(C_IP_ADDRESS));
	out.append(",").append("5000");
	emit set_arg("set", "--multiplay=out", out);

	QString in(",");
	in.append(comboHzIn->currentText());
	in.append(",");
	in.append(comboLocalIpAddress->currentText());
	in.append(",").append("5000");
	emit set_arg("set", "--multiplay=in", in);
}


//=====================================
// Setup fgCom
void NetworkWidget::set_fgcom(){
	if(grpFgCom->isChecked()){
		if(txtFgComNo->text().trimmed().length() == 0){
			txtFgComNo->setText(settings.default_fgcom_no());
		}
		if(txtFgComPort->text().trimmed().length() == 0){
			txtFgComPort->setText(settings.default_fgcom_port());
		}
		emit set_arg("set", "--fgcom=", txtFgComNo->text().append(":").append( txtFgComPort->text() ) );
	}else{
		emit set_arg("remove", "--fgcom=","");
	}
}

//=====================================
// Setup Combo Hz
void NetworkWidget::populate_combo_hz(QComboBox *combo){
	for(int i=1; i < 31; i++){
		combo->addItem(QString("%1").arg(i));
	}
	combo->setCurrentIndex(4);
}




//=================================================
// Set Http
void NetworkWidget::set_http(){
	if( grpHttp->isChecked() ){
		emit set_arg("set", "--http=", txtHttp->text());
	}else{
		emit set_arg("remove", "--http=", "");
	}
}


//=================================================
// Set Telnet
void NetworkWidget::set_telnet(){
	if( grpTelnet->isChecked() ){
		emit set_arg("set", "--telnet=", txtTelnet->text());
	}else{
		emit set_arg("remove", "--telnet=", "");
	}
}


//=================================================
// Set ScreenShot
void NetworkWidget::set_screenshot(){
	if( grpScreenShot->isChecked() ){
		emit set_arg("set", "--jpg-httpd=", txtScreenShot->text());
	}else{
		emit set_arg("remove", "--jpg-httpd=", "");
	}
}

//=================================================
// MP IN / Out Events
void NetworkWidget::on_checkbox_in( ){
	bool state = checkBoxIn->isChecked();
	comboLocalIpAddress->setEnabled(state);
	comboLocalPort->setEnabled(state);
	comboHzIn->setEnabled(state);
}
void NetworkWidget::on_checkbox_out(){
	bool state = checkBoxOut->isChecked();
	comboRemoteAddress->setEnabled(state);
	comboRemotePort->setEnabled(state);
	comboHzOut->setEnabled(state);
	treeWidget->setEnabled(state);

}



//=============================================================
// Get Args
QStringList NetworkWidget::get_args(){

	QStringList args;

	//* Enable Multiplay
	if(grpMpServer->isChecked()){
		args << "--enable-ai-models";
		if(checkBoxIn->isChecked()){
			args << QString("--multiplay=in,%1,%2,%3").arg(
									comboHzIn->currentText()).arg(
									comboLocalIpAddress->currentText()).arg(
									comboLocalPort->currentText()
							);
		}
		if(checkBoxOut->isChecked()){
			QString remote_server(	comboRemoteAddress->itemData(comboRemoteAddress->currentIndex(), Qt::UserRole).toString() == "domain"
							? treeWidget->currentItem()->text(C_DOMAIN)
							: treeWidget->currentItem()->text(C_IP_ADDRESS)
			);
			args << QString("--multiplay=out,%1,%2,%3").arg(
									comboHzOut->currentText()).arg(
									remote_server).arg(
									comboRemotePort->currentText()
							);
		}
	}

	//* FgCom
	if(grpFgCom->isChecked()){
		args << QString("--generic=socket,out,10,localhost,%1,udp,fgcom").arg( txtFgComPort->text() );
	}

	//* Http
	if(grpHttp->isChecked()){
		args << QString("--http=%1").arg( txtHttp->text() );
	}

	//* Telnet
	if(grpTelnet->isChecked()){
		args << QString("--telnet=%1").arg( txtTelnet->text() );
	}

	//* ScreenShot
	if(grpScreenShot->isChecked()){
		args << QString("--jpg-httpd=%1").arg( txtScreenShot->text() );
	}

	return args;
}

//=============================================================
// Save Settings
void NetworkWidget::save_settings(){
	settings.setValue("enable_mp", grpMpServer->isChecked());
	settings.setValue("callsign", txtCallSign->text());

	settings.setValue("in", checkBoxIn->isChecked());
	settings.setValue("out", checkBoxOut->isChecked());

	settings.setValue("in_address", comboLocalIpAddress->currentText());

	QString ip_or_domain( comboRemoteAddress->itemData(comboRemoteAddress->currentIndex(), Qt::UserRole).toString());
	settings.setValue("out_with", ip_or_domain);
	if(treeWidget->currentItem()){
		settings.setValue("mpserver", treeWidget->currentItem()->text(ip_or_domain == "domain" ? C_DOMAIN : C_IP_ADDRESS));
	}

	settings.setValue("in_port", comboLocalPort->currentText());
	settings.setValue("out_port", comboRemotePort->currentText());

	settings.setValue("in_hz", comboHzIn->currentText());
	settings.setValue("out_hz", comboHzOut->currentText());

	settings.setValue("fgcom", grpFgCom->isChecked());
	settings.setValue("fgcom_no", txtFgComNo->text());
	settings.setValue("fgcom_port", txtFgComPort->text());


	settings.setValue("telnet", grpTelnet->isChecked());
	settings.setValue("telnet_port", txtTelnet->text());

	settings.setValue("http", grpHttp->isChecked());
	settings.setValue("http_port", txtHttp->text());

	settings.setValue("screenshot", grpScreenShot->isChecked());
	settings.setValue("screenshot_port", txtScreenShot->text());



}


//=============================================================
// Load Settings
void NetworkWidget::load_settings(){
	int idx;

	grpMpServer->setChecked( settings.value("enable_mp").toBool() );
	txtCallSign->setText( settings.value("callsign").toString() );

	checkBoxIn->setChecked( settings.value("in").toBool() );
	checkBoxOut->setChecked( settings.value("out").toBool() );

	idx = comboLocalIpAddress->findText(settings.value("in_address").toString(), Qt::MatchExactly);
	comboLocalIpAddress->setCurrentIndex( idx == -1 ? 0 : idx);

	idx = comboRemoteAddress->findData(settings.value("out_with").toString(), Qt::UserRole, Qt::MatchExactly);
	comboRemoteAddress->setCurrentIndex( idx == -1 ? 0 : idx);

	idx = comboHzIn->findText(settings.value("in_hz").toString(), Qt::MatchExactly);
	comboHzIn->setCurrentIndex( idx == -1 ? 0 : idx );
	idx = comboHzOut->findText(settings.value("out_hz").toString(), Qt::MatchExactly);
	comboHzOut->setCurrentIndex( idx == -1 ? 0 : idx );

	idx = comboLocalPort->findText(settings.value("in_port").toString(), Qt::MatchExactly);
	comboLocalPort->setCurrentIndex( idx == -1 ? 0 : idx );
	idx = comboRemotePort->findText(settings.value("out_port").toString(), Qt::MatchExactly);
	comboRemotePort->setCurrentIndex( idx == -1 ? 0 : idx );


	grpFgCom->setChecked( settings.value("fgcom").toBool() );
	txtFgComNo->setText( settings.value("fgcom_no", settings.default_fgcom_no()).toString());
	txtFgComPort->setText( settings.value("fgcom_port", settings.default_fgcom_port()).toString());


	grpHttp->setChecked( settings.value("http").toBool() );
	grpTelnet->setChecked( settings.value("telnet").toBool() );
	grpScreenShot->setChecked( settings.value("screenshot").toBool() );

	txtHttp->setText(settings.value("http_port", "8080").toString());
	txtTelnet->setText(settings.value("telnet_port", "5555").toString());
	txtScreenShot->setText(settings.value("screenshot_port", "8081").toString());

	on_checkbox_in();
	on_checkbox_out();
}

//=============================================================
// Validate
bool NetworkWidget::validate(){
	if(grpMpServer->isChecked()){
		if(txtCallSign->text().trimmed().length() == 0){
			txtCallSign->setFocus();
			return false;
		}
		if(!treeWidget->currentItem() or treeWidget->currentItem()->text(C_FLAG) != "1"){
			treeWidget->setFocus();
			return false;
		}
	}
	return true;
}
