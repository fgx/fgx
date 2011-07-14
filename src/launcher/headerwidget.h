#ifndef XHEADERWIDGET_H
#define XHEADERWIDGET_H

#include <QLabel>
#include <QTimer>

class HeaderWidget : public QLabel
{
Q_OBJECT
public:
	explicit HeaderWidget(QWidget *parent = 0);

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

#endif // XHEADERWIDGET_H
