#ifndef NETWORKWIDGET_H
#define NETWORKWIDGET_H


#include <QtNetwork/QHostInfo>


#include <QWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>

#include "xobjects/xsettings.h"

class NetworkWidget : public QWidget
{
Q_OBJECT
public:

    enum COLS{
        C_SERVER_NO,
        C_SERVER_NAME,
        C_DOMAIN,
        C_PILOTS_COUNT,
		C_IP_ADDRESS,
		C_FLAG
    };

	explicit NetworkWidget(QWidget *parent = 0);

	//MainObject *mainObject;
	XSettings settings;

	QGroupBox *grpMpServer;
	QLineEdit *txtCallSign;

	QCheckBox *checkBoxIn;
	QCheckBox *checkBoxOut;
	QComboBox *comboLocalIpAddress;
	QComboBox *comboRemoteAddress;
	QComboBox *comboRemotePort;
	QComboBox *comboLocalPort;
	QComboBox *comboHzOut;
	QComboBox *comboHzIn;
	QTreeWidget *treeWidget;


	QGroupBox *grpFgCom;
	QLineEdit *txtFgComNo;
	QLineEdit *txtFgComPort;

	QGroupBox *grpTelnet;
	QLineEdit *txtTelnet;

	QGroupBox *grpHttp;
	QLineEdit *txtHttp;

	QGroupBox *grpScreenShot;
	QLineEdit *txtScreenShot;

    void dns_lookup(int server_no);
	void load_addresses();
	void populate_combo_hz(QComboBox *combo);
	void save_settings();
	void load_settings();
	QString validate();
	QStringList get_args();

signals:
	void set_arg(QString action, QString arg, QString value);


public slots:
     void dns_lookup_all();
     void on_dns_lookup_host(const QHostInfo &hostInfo);
     void on_telnet_data(QString, QString);

	 void on_mp_server_checked(bool);
	 void on_callsign_changed(QString);
	 void set_mp_server();

	 void set_fgcom();
	 void set_http();
	 void set_telnet();
	 void set_screenshot();

	 void on_checkbox_in();
	 void on_checkbox_out();
};

#endif // NETWORKWIDGET_H
