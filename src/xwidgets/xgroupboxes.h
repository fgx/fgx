#ifndef XGROUPBOXES_H
#define XGROUPBOXES_H

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBoxLayout>

//=====================================================
//** Vertical Box
class XGroupVBox : public QGroupBox
{
Q_OBJECT
public:
	explicit XGroupVBox(QString title, QWidget *parent = 0);

	QVBoxLayout *xLayout;
	void addWidget(QWidget *w);
	void addWidget(QWidget *w, int stretch);
	void addLayout(QBoxLayout *lay);
signals:

public slots:

};

//=====================================================
//** Horizontal Box
class XGroupHBox : public QGroupBox
{
Q_OBJECT
public:
	explicit XGroupHBox(QString title, QWidget *parent = 0);

	QHBoxLayout *xLayout;
	void addWidget(QWidget *w);
	void addWidget(QWidget *w, int stretch);
	void addLayout(QBoxLayout *lay);
signals:

public slots:

};


#endif // XGROUPBOX_H
