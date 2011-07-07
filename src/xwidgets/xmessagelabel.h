#ifndef XMESSAGELABEL_H
#define XMESSAGELABEL_H

#include <QLabel>
#include <QTimer>

class XMessageLabel : public QLabel
{
Q_OBJECT
public:
    explicit XMessageLabel(QWidget *parent = 0);

	QTimer *timer;

signals:

public slots:


	void showMessage(QString message, int timeout);
	void showMessage(QString message);
	

	void start_fade();

};

#endif // XMESSAGELABEL_H
