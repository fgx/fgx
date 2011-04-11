#ifndef EXECONTROLS_H
#define EXECONTROLS_H

#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

class ExeControls : public QGroupBox
{
Q_OBJECT
public:
	explicit ExeControls(QWidget *parent = 0);

	QLabel *labelPid;
	QPushButton *buttonStart;
	QPushButton *buttonStop;

signals:

public slots:
	void on_stop_clicked();
	void on_start_clicked();

	void update_pid();
	void kill_pid();


};

#endif // EXECONTROLS_H
