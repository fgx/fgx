#ifndef XGROUPBOXES_H
#define XGROUPBOXES_H

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBoxLayout>
#include <QGridLayout>

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


//=====================================================
//** Grid Box
class XGroupGBox : public QGroupBox
{
Q_OBJECT
public:
	explicit XGroupGBox(QString title, QWidget *parent = 0);

	QGridLayout *gridLayout;
	void addWidget(QWidget *w, int row, int col, int row_span, int col_span);
signals:

public slots:

};

#endif // XGROUPBOX_H
