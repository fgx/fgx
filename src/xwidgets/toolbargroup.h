#ifndef TOOLBARGROUP_H
#define TOOLBARGROUP_H


#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class ToolBarGroup : public QWidget
{
    Q_OBJECT
public:
    explicit ToolBarGroup(QWidget *parent = 0);

    QLabel *lblTitle;
    QHBoxLayout *bottomLayout;

    void setTitle(QString);
    void addWidget(QWidget *);
    void removeWidget(QWidget *);

signals:

public slots:

};

#endif // TOOLBARGROUP_H
