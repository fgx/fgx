

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

/* list the Mp Servers which are gotten from a DNS lookup

*/


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

	//========================================================================
	// Mp Servers Box
	grpMpServer  = new QGroupBox("Enable Multiplayer");
	middleLayout->addWidget(grpMpServer, 3);
	grpMpServer->setCheckable(true);
	grpMpServer->setChecked(false);
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
	checkBoxIn = new QCheckBox("In to");
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
	connect(comboHzIn, SIGNAL(currentIndexChanged(int)), this, SLOT(set_mp_server()));
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
	treeWidget->setSortingEnabled(true);
	treeWidget->sortByColumn(C_SERVER_NAME, Qt::AscendingOrder);

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
	layoutMpServer->addLayout(layoutBottomTreeBar);
	layoutBottomTreeBar->addStretch(30);

	//* refresh MP servers
	QToolButton *refreshButton = new QToolButton(this);
	layoutBottomTreeBar->addWidget(refreshButton);
	refreshButton->setIcon(QIcon(":/icon/refresh"));
	refreshButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	connect(refreshButton, SIGNAL(clicked()), this, SLOT(dns_lookup()) );



	//****************** RIGHT ********************************
	QVBoxLayout *rightLayout = new QVBoxLayout();
	rightLayout->setSpacing(20);
	middleLayout->addLayout(rightLayout, 2);

	//========================================================================
	// FgCom Box
	grpFgCom = new QGroupBox(tr("fgCom - Voice Communications"));
	rightLayout->addWidget(grpFgCom, 2);
	grpFgCom->setCheckable(true);
	grpFgCom->setChecked(false);
	connect(grpFgCom, SIGNAL(clicked(bool)), this, SLOT(set_fgcom()));

	QGridLayout *layoutFgCom = new QGridLayout();
	grpFgCom->setLayout(layoutFgCom);

	QString style("font-size: 8pt; color: #666666;");

	// fgCom NO
	row = 0;
	layoutFgCom->addWidget(new QLabel("Options"), row, 0, 1, 1, Qt::AlignRight);
	txtFgComNo = new QLineEdit();
	layoutFgCom->addWidget(txtFgComNo, row, 1);
	connect(txtFgComNo, SIGNAL(textChanged(QString)), this, SLOT(set_fgcom()));

	row++;
	QLabel *lblHelp1 = new QLabel("eg: -Sfgcom.flightgear.org.uk");
	lblHelp1->setStyleSheet(style);
	layoutFgCom->addWidget(lblHelp1, row, 1, 1, 2);


	//layoutFgCom->addSpacing(10);

	//* fgCom Port
	row++;
	layoutFgCom->addWidget(new QLabel("Port"), row, 0, 1, 1, Qt::AlignRight);
	txtFgComPort = new QLineEdit();
	txtFgComPort->setMaximumWidth(100);
	layoutFgCom->addWidget(txtFgComPort);
	connect(txtFgComPort, SIGNAL(textChanged(QString)), this, SLOT(set_fgcom()));

	row++;
	QLabel *lblHelp2 = new QLabel("eg: 16661");
	lblHelp2->setStyleSheet(style);
	layoutFgCom->addWidget(lblHelp2, row, 1, 1, 2);

	row++;
	QHBoxLayout *hboxfgCom = new QHBoxLayout();
	layoutFgCom->addLayout(hboxfgCom, row,0,1,2);
	hboxfgCom->addStretch(20);


	//===========================================================
	//** Telnet
	grpTelnet = new QGroupBox();
	grpTelnet->setTitle(tr("Telnet Properties Server"));
	grpTelnet->setCheckable(true);
	grpTelnet->setChecked(false);
	rightLayout->addWidget(grpTelnet);

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

	QToolButton *buttTelnet = new QToolButton();
	layoutNetTelnet->addWidget(buttTelnet);
	buttTelnet->setDisabled(true); // TODO
	buttTelnet->setIcon(QIcon(":/icon/terminal"));
	buttTelnet->setToolButtonStyle(Qt::ToolButtonIconOnly);
	connect(buttTelnet, SIGNAL(clicked()), this, SLOT(on_open_telnet()));

	//===========================================================
	//** Screenshot
	grpScreenShot = new QGroupBox();
	grpScreenShot->setTitle(tr("Screen Shot Server"));
	grpScreenShot->setCheckable(true);
	grpScreenShot->setChecked(false);
	rightLayout->addWidget(grpScreenShot);

	QHBoxLayout *layoutScreenShot = new QHBoxLayout();
	grpScreenShot->setLayout(layoutScreenShot);
	layoutScreenShot->setSpacing(10);
	//int m = 5;
	layoutScreenShot->setContentsMargins(m,m,m,m);

	layoutScreenShot->addWidget( new QLabel(tr("Port No:")));

	txtScreenShot = new QLineEdit("8088");
	txtScreenShot->setValidator(new QIntValidator(80, 32000, this));
	layoutScreenShot->addWidget(txtScreenShot);

	QToolButton *buttScreenshot = new QToolButton();
	layoutScreenShot->addWidget(buttScreenshot);
	buttScreenshot->setIcon(QIcon(":/icon/browse"));
	buttScreenshot->setToolButtonStyle(Qt::ToolButtonIconOnly);
	connect(buttScreenshot, SIGNAL(clicked()), this, SLOT(on_browse_screenshot()));

	//==========================================================
	//** HTTP
	grpHttp = new QGroupBox();
	grpHttp->setTitle(tr("HTTP Web Server"));
	grpHttp->setCheckable(true);
	grpHttp->setChecked(false);
	rightLayout->addWidget(grpHttp);

	QHBoxLayout *layoutNetHttp = new QHBoxLayout();
	grpHttp->setLayout(layoutNetHttp);
	layoutNetHttp->setSpacing(10);

	layoutNetHttp->addWidget( new QLabel(tr("Port No:")) );

	txtHttp = new QLineEdit("8080");
	txtHttp->setValidator(new QIntValidator(80, 32000, this));
	layoutNetHttp->addWidget(txtHttp);

	QToolButton *butHttp = new QToolButton();
	layoutNetHttp->addWidget(butHttp);
	butHttp->setIcon(QIcon(":/icon/browse"));
	butHttp->setToolButtonStyle(Qt::ToolButtonIconOnly);
	connect(butHttp, SIGNAL(clicked()), this, SLOT(on_browse_http()));


	rightLayout->addStretch(20);


	//** Setup network stuff
	dns_lookup();
	load_local_addresses();
	on_checkbox_in();
	on_checkbox_out();


}
/* end constructor */


//=============================================================
//** Network Addresses
void NetworkWidget::load_local_addresses(){
	QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
	for (int i = 0; i < addresses.size(); ++i) {
		if(addresses.at(i).protocol() == QAbstractSocket::IPv4Protocol){
			comboLocalIpAddress->addItem( addresses.at(i).toString() );
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
	if(mainObject->settings->value("mpserver").toString() == hostInfo.hostName()){
		treeWidget->setCurrentItem(newItem);
	}
	return; //######################
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
// Mp Server Enabled
void NetworkWidget::on_mp_server_checked(bool state){
	if (state){
		txtCallSign->setFocus();
	}
}


//=====================================
// Callsign Changed
void NetworkWidget::on_callsign_changed(QString txt){
	mainObject->actionCallsign->setText(txt.trimmed());
}

//=====================================
// Mp Server Selected
void NetworkWidget::set_mp_server(){

	QTreeWidgetItem *item = treeWidget->currentItem();

	if(!item or item->text(C_FLAG).length() == 0){
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
			txtFgComNo->setText(mainObject->settings->default_fgcom_no());
		}
		if(txtFgComPort->text().trimmed().length() == 0){
			txtFgComPort->setText(mainObject->settings->default_fgcom_port());
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
// Get Args
QStringList NetworkWidget::get_args(){

	validate();
	QStringList args;

	//* Enable Multiplay
	if(grpMpServer->isChecked()){
		if(checkBoxIn->isChecked()){
			args << QString("--multiplay=in,%1,%2,%3").arg(
									comboHzIn->currentText()).arg(
									comboLocalIpAddress->currentText()).arg(
									comboLocalPort->currentText()
							);
		}
		if(checkBoxOut->isChecked()){
			if(treeWidget->currentItem()){
				QString remote_server(	comboRemoteAddress->itemData(comboRemoteAddress->currentIndex(), Qt::UserRole).toString() == "domain"
								? treeWidget->currentItem()->text(C_DOMAIN)
								: treeWidget->currentItem()->text(C_IP_ADDRESS));
				args << QString("--multiplay=out,%1,%2,%3").arg(
								comboHzOut->currentText()).arg(
								remote_server).arg(
								comboRemotePort->currentText());
			}
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
	mainObject->settings->setValue("enable_mp", grpMpServer->isChecked());
	mainObject->settings->setValue("callsign", txtCallSign->text());

	mainObject->settings->setValue("in", checkBoxIn->isChecked());
	mainObject->settings->setValue("out", checkBoxOut->isChecked());

	mainObject->settings->setValue("in_address", comboLocalIpAddress->currentText());

	QString ip_or_domain( comboRemoteAddress->itemData(comboRemoteAddress->currentIndex(), Qt::UserRole).toString());
	mainObject->settings->setValue("out_with", ip_or_domain);
	if(treeWidget->currentItem()){
		mainObject->settings->setValue("mpserver", treeWidget->currentItem()->text(C_DOMAIN));
	}

	mainObject->settings->setValue("in_port", comboLocalPort->currentText());
	mainObject->settings->setValue("out_port", comboRemotePort->currentText());

	mainObject->settings->setValue("in_hz", comboHzIn->currentText());
	mainObject->settings->setValue("out_hz", comboHzOut->currentText());

	mainObject->settings->setValue("fgcom", grpFgCom->isChecked());
	mainObject->settings->setValue("fgcom_no", txtFgComNo->text());
	mainObject->settings->setValue("fgcom_port", txtFgComPort->text());


	mainObject->settings->setValue("telnet", grpTelnet->isChecked());
	mainObject->settings->setValue("telnet_port", txtTelnet->text());

	mainObject->settings->setValue("http", grpHttp->isChecked());
	mainObject->settings->setValue("http_port", txtHttp->text());

	mainObject->settings->setValue("screenshot", grpScreenShot->isChecked());
	mainObject->settings->setValue("screenshot_port", txtScreenShot->text());

	mainObject->settings->sync();
}


//=============================================================
// Load Settings
void NetworkWidget::load_settings(){
	int idx;

	grpMpServer->setChecked( mainObject->settings->value("enable_mp").toBool() );
	txtCallSign->setText( mainObject->settings->value("callsign").toString() );

	checkBoxIn->setChecked( mainObject->settings->value("in").toBool() );
	checkBoxOut->setChecked( mainObject->settings->value("out").toBool() );

	idx = comboLocalIpAddress->findText(mainObject->settings->value("in_address").toString(), Qt::MatchExactly);
	comboLocalIpAddress->setCurrentIndex( idx == -1 ? 0 : idx);

	idx = comboRemoteAddress->findData(mainObject->settings->value("out_with").toString(), Qt::UserRole, Qt::MatchExactly);
	comboRemoteAddress->setCurrentIndex( idx == -1 ? 0 : idx);

	idx = comboHzIn->findText(mainObject->settings->value("in_hz").toString(), Qt::MatchExactly);
	comboHzIn->setCurrentIndex( idx == -1 ? 0 : idx );
	idx = comboHzOut->findText(mainObject->settings->value("out_hz").toString(), Qt::MatchExactly);
	comboHzOut->setCurrentIndex( idx == -1 ? 0 : idx );

	idx = comboLocalPort->findText(mainObject->settings->value("in_port").toString(), Qt::MatchExactly);
	comboLocalPort->setCurrentIndex( idx == -1 ? 0 : idx );
	idx = comboRemotePort->findText(mainObject->settings->value("out_port").toString(), Qt::MatchExactly);
	comboRemotePort->setCurrentIndex( idx == -1 ? 0 : idx );


	grpFgCom->setChecked( mainObject->settings->value("fgcom").toBool() );
	txtFgComNo->setText( mainObject->settings->value("fgcom_no", mainObject->settings->default_fgcom_no()).toString());
	txtFgComPort->setText( mainObject->settings->value("fgcom_port", mainObject->settings->default_fgcom_port()).toString());


	grpHttp->setChecked( mainObject->settings->value("http").toBool() );
	grpTelnet->setChecked( mainObject->settings->value("telnet").toBool() );
	grpScreenShot->setChecked( mainObject->settings->value("screenshot").toBool() );

	txtHttp->setText(mainObject->settings->value("http_port", "8080").toString());
	txtTelnet->setText(mainObject->settings->value("telnet_port", "5555").toString());
	txtScreenShot->setText(mainObject->settings->value("screenshot_port", "8081").toString());

	on_checkbox_in();
	on_checkbox_out();
}

//=============================================================
// Validate
QString NetworkWidget::validate(){
	if(grpMpServer->isChecked()){
		if(txtCallSign->text().trimmed().length() == 0){
			txtCallSign->setFocus();
			return QString("Callsign required");
		}
		if(checkBoxOut->isChecked()){
			if(!treeWidget->currentItem()){
				treeWidget->setFocus();
				return QString("No multiplayer out server selected");;
			}
		}
	}
	return QString();
}




