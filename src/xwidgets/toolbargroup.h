#ifndef TOOLBARGROUP_H
#define TOOLBARGROUP_H

#include <QWidget>
#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>

class ToolBarGroup : public QWidget
{
    Q_OBJECT
public:
    explicit ToolBarGroup(QWidget *parent = 0);

    QVBoxLayout *mainLayout;
    QToolBar *toolbar;
    QLabel *lblTitle;

    void addWidget(QWidget *wid);

signals:

public slots:

};

#endif // TOOLBARGROUP_H
