#ifndef XSTATUSBAR_H
#define XSTATUSBAR_H

#include <QStatusBar>

class XStatusBar : public QStatusBar
{
Q_OBJECT
public:
    explicit XStatusBar(QWidget *parent = 0);

    void showMessage(QString message, int timeout);
    void showMessage(QString message);

    void showError(QString message, int timeout);
    void showError(QString message);


signals:

public slots:

};

#endif // XSTATUSBAR_H
