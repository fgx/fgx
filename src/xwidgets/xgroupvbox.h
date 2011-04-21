#ifndef XGROUPVBOX_H
#define XGROUPVBOX_H

#include <QGroupBox>
#include <QVBoxLayout>

class XGroupVBox : public QGroupBox
{
Q_OBJECT
public:
	explicit XGroupVBox(QString title, QWidget *parent = 0);

	QVBoxLayout *xLayout;


	void addWidget(QWidget *w);
signals:

public slots:

};

#endif // XGROUPVBOX_H
