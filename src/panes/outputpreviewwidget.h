#ifndef OUTPUTPREVIEWWIDGET_H
#define OUTPUTPREVIEWWIDGET_H

#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

class OutputPreviewWidget : public QWidget
{
Q_OBJECT
public:
    explicit OutputPreviewWidget(QWidget *parent = 0);

	QPlainTextEdit *txtPreviewOutput;
	QPushButton *buttonCommandPreview;
	QPushButton *buttonCommandHelp;
signals:

public slots:

};

#endif // OUTPUTPREVIEWWIDGET_H
