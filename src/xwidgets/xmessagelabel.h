#ifndef XMESSAGELABEL_H
#define XMESSAGELABEL_H

#include <QLabel>
#include <QTimer>

class XMessageLabel : public QLabel
{
Q_OBJECT
public:
    explicit XMessageLabel(QWidget *parent = 0);

	QTimer *fadeTimer;

	QWidget *popWidget;
	QLabel *popLabel;


signals:

public slots:


	void showMessage(QString message, int timeout);
	void showMessage(QString message);
	

	void start_fade();
	void on_fade_timer();
};

#endif // XMESSAGELABEL_H
