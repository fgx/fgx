#ifndef XSTATUSBAR_H
#define XSTATUSBAR_H


#include <QWidget>
#include <QHBoxLayout>
#include <QStatusBar>
#include <QProgressBar>
#include <QToolButton>


class XStatusBar : public QWidget
{
    Q_OBJECT
public:
    explicit XStatusBar(QWidget *parent = 0);

    QHBoxLayout *mainLayout;
    QStatusBar *statusBar;
    QProgressBar *progressBar;
    QToolButton *buttRefresh;

    //void addWidget(QWidget *);
    //void removeWidget(QWidget *);

signals:
    void refresh();

public slots:

    void set_busy(bool state, QString mess);

};

#endif // XSTATUSBAR_H
