#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QMessageBox>

class MessageBox : public QMessageBox
{
Q_OBJECT
public:
    explicit MessageBox(QWidget *parent = 0);

    void showWindowMessage(QString alertMessage);


signals:

public slots:

};

#endif // MESSAGEBOX_H
