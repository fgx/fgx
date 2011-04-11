#ifndef EXECONTROLS_H
#define EXECONTROLS_H

#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

class ExeControls : public QGroupBox
{
Q_OBJECT
public:
	explicit ExeControls(QString title, QString exeNam, QWidget *parent = 0);

	QString exe_name;
	QLabel *labelPid;
	QPushButton *buttonStart;
	QPushButton *buttonStop;
	QPushButton *buttonRefresh;

	int get_pid();
	void update_pid();
	void kill_pid();

signals:

public slots:
	void on_stop_clicked();
	void on_refresh_clicked();






};

#endif // EXECONTROLS_H
