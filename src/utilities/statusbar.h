#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>

class StatusBar : public QStatusBar
{
Q_OBJECT
public:
    explicit StatusBar(QWidget *parent = 0);

    void showMessage(QString message, int timeout);
    void showMessage(QString message);

    void showError(QString message, int timeout);
    void showError(QString message);


signals:

public slots:

};

#endif // STATUSBAR_H
