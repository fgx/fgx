
#include <QVBoxLayout>
#include <QToolBar>
#include <QVBoxLayout>

#include "pilotswidget.h"

PilotsWidget::PilotsWidget(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mob;

	setMinimumWidth(200);


	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->setContentsMargins(0,0,0,0);
	mainLayout->setSpacing(0);
	setLayout(mainLayout);


	QToolBar *toolbar = new QToolBar();
	mainLayout->addWidget(toolbar);

	QAction *actionRefresh = new QAction(this);
	actionRefresh->setText("Refresh");
	actionRefresh->setIcon(QIcon(":/icon/refresh"));
	toolbar->addAction(actionRefresh);

	tree = new QTreeWidget();
	mainLayout->addWidget(tree);

	tree->headerItem()->setText(C_CALLSIGN, "Callsign");
	tree->headerItem()->setText(C_AERO, "Aircraft");
	tree->headerItem()->setText(C_ALTITUDE, "Altitude");



	statusBar = new QStatusBar();
	mainLayout->addWidget(statusBar);
	statusBar->showMessage("Click refresh to load");

}
