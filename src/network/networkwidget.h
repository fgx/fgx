#ifndef NETWORKWIDGET_H
#define NETWORKWIDGET_H


#include <QtNetwork/QHostInfo>


#include <QWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>

#include "xobjects/mainobject.h"
#include "xwidgets/xgroupboxes.h"

class MainObject;

class NetworkWidget : public QWidget
{
Q_OBJECT
public:

    enum COLS{
        C_SERVER_NAME,
        C_DOMAIN,
        C_PILOTS_COUNT,
		C_IP_ADDRESS,
		C_FLAG
    };

	explicit NetworkWidget(MainObject *mOb, QWidget *parent = 0);

	qint64 pid_fgcom;

	MainObject *mainObject;

	XGroupHBox *grpMpServerIn;
	XGroupVBox *grpMpServerOut;


	QComboBox *comboLocalIpAddress;
	QComboBox *comboRemoteAddress;
	QComboBox *comboRemotePort;
	QComboBox *comboLocalPort;
	QComboBox *comboHzOut;
	QComboBox *comboHzIn;
	QTreeWidget *treeWidget;


	XGroupGBox *grpFgCom;
	QLineEdit *txtFgComNo;
	QLineEdit *txtFgComPort;

	XGroupHBox *grpTelnet;
	QLineEdit *txtTelnet;

	XGroupHBox *grpHttp;
	QLineEdit *txtHttp;

	XGroupHBox *grpScreenShot;
	QLineEdit *txtScreenShot;


	void load_local_addresses();
	void populate_combo_hz(QComboBox *combo);

	QString validate();


signals:
	void setx(QString option, bool enabled, QString value);

public slots:
	void dns_lookup();
	void on_dns_lookup_callback(const QHostInfo &hostInfo);
	void on_telnet_data(QString, QString);

	void set_mp_server();
	void set_fgcom();

	void on_http();
	void on_telnet();
	void on_screenshot();

	void on_browse_http();
	void on_browse_screenshot();
	void on_open_telnet();
	
	void on_upx(QString option, bool enabled, QString value);

};

#endif // NETWORKWIDGET_H
