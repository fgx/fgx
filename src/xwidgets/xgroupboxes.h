#ifndef XGROUPBOXES_H
#define XGROUPBOXES_H

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBoxLayout>
#include <QGridLayout>

/** \brief A QGroupBox with a QVBoxLyout layout */
class XGroupVBox : public QGroupBox
{
Q_OBJECT
public:
	explicit XGroupVBox(QString title, QWidget *parent = 0);


	QVBoxLayout *xLayout;
	void addWidget(QWidget *w);
	void addWidget(QWidget *w, int stretch);
	void addLayout(QBoxLayout *lay);
	void addLayout(QGridLayout *gridlayout);

signals:

public slots:

};

/** \brief A QGroupBox with a QHBoxLyout layout */
class XGroupHBox : public QGroupBox
{
Q_OBJECT
public:
	explicit XGroupHBox(QString title, QWidget *parent = 0);

	QHBoxLayout *xLayout;
	void addWidget(QWidget *w);
	void addWidget(QWidget *w, int stretch);
	void addLayout(QBoxLayout *lay);
	void addLayout(QGridLayout *gridlayout);

signals:

public slots:

};


/** \brief A QGroupBox with a QGridLayout layout */
class XGroupGBox : public QGroupBox
{
Q_OBJECT
public:
	explicit XGroupGBox(QString title, QWidget *parent = 0);

	QGridLayout *gridLayout;
	void addWidget(QWidget *w, int row, int col, int row_span, int col_span);
	void addWidget(QWidget *w, int row, int col, int row_span, int col_span, Qt::AlignmentFlag align);

signals:

public slots:

};

#endif // XGROUPBOX_H
