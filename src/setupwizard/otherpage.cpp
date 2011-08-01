


#include <QGridLayout>
#include <QFileDialog>

#include "setupwizard/otherpage.h"
#include "xobjects/xopt.h"
#include "xwidgets/xgroupboxes.h"
#include "utilities/utilities.h"

OtherPage::OtherPage(MainObject *mob, QWidget *parent) :
	QWizardPage(parent)
{

	mainObject = mob;

	setTitle("Utility Paths");
    setSubTitle("Setup paths to other executables");

	QVBoxLayout *mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	//==================================================
	// FgCom path
	XGroupGBox *grpFgcom = new XGroupGBox("FgCom executable");
	mainLayout->addWidget(grpFgcom);

	int row = 0;
	txtFgCom = new QLineEdit("");
	grpFgcom->addWidget(txtFgCom, row, 0, 1, 1);
    //connect(txtFgCom, SIGNAL(textChanged(QString)), this, SLOT(check_paths()) );
    connect(txtFgCom, SIGNAL(textEdited(QString)), this, SLOT(on_fgcom()));
	//= Dropdown button for path
	QToolButton *buttFgCom = new QToolButton();
	grpFgcom->addWidget(buttFgCom, row, 1, 1, 1);
	buttFgCom->setIcon(QIcon(":/icon/black"));
	buttFgCom->setPopupMode(QToolButton::InstantPopup);

	QMenu *menuFgCom = new QMenu();
	buttFgCom->setMenu(menuFgCom);

	QAction *actionFgComSelectPath = new QAction(menuFgCom);
	menuFgCom->addAction(actionFgComSelectPath);
	actionFgComSelectPath->setText(tr("Select path ..."));
	connect(actionFgComSelectPath, SIGNAL(triggered()), this, SLOT(on_select_fgcom_path()));

	// does not work for win/osx
    // then why in these boxes have code that does NOTHING?
#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
    if(mainObject->runningOs() == XSettings::LINUX){
		QAction *actionFgComAutoSelect = new QAction(menuFgCom);
		menuFgCom->addAction(actionFgComAutoSelect);
		actionFgComAutoSelect->setText(tr("Autodetect"));
		connect(actionFgComAutoSelect, SIGNAL(triggered()), this, SLOT(on_fgcom_autodetect()));
		//actionFgfsAutoSelect->setVisible( mainObject->settings->runningOs() != XSettings::WINDOWS );
	}
#endif

	row++;
    lblFgCom = new QLabel("FGCom untested");
	grpFgcom->addWidget(lblFgCom, row, 0, 1, 2);

	//==================================================
	// Joystick Demo path
	XGroupGBox *grpJoystick = new XGroupGBox("Joystick executable");
	mainLayout->addWidget(grpJoystick);

	row = 0;
	txtJoystickDemo = new QLineEdit("");
	grpJoystick->addWidget(txtJoystickDemo, row, 0, 1, 1);
    //connect(txtJoystickDemo, SIGNAL(txtJoystickDemo(QString)), this, SLOT(check_paths()) );

	//= Dropdown button for path
	QToolButton *buttJoystick = new QToolButton();
	grpJoystick->addWidget(buttJoystick, row, 1, 1, 1);
	buttJoystick->setIcon(QIcon(":/icon/black"));
	buttJoystick->setPopupMode(QToolButton::InstantPopup);

	QMenu *menuJoystick = new QMenu();
	buttJoystick->setMenu(menuJoystick);

	QAction *actionJoystickSelectPath = new QAction(menuFgCom);
	menuJoystick->addAction(actionJoystickSelectPath);
	actionJoystickSelectPath->setText(tr("Select path ..."));
	connect(actionJoystickSelectPath, SIGNAL(triggered()), this, SLOT(on_select_joystick_path()));

	// does not work for win/osx
    // then why in these boxes have code that does NOTHING?
#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
	if(mainObject->runningOs() == XSettings::LINUX){
		QAction *actionJoystickAutoSelect = new QAction(menuJoystick);
		menuJoystick->addAction(actionJoystickAutoSelect);
		actionJoystickAutoSelect->setText(tr("Autodetect"));
		connect(actionJoystickAutoSelect, SIGNAL(triggered()), this, SLOT(on_joystick_autodetect()));
		//actionFgfsAutoSelect->setVisible( mainObject->settings->runningOs() != XSettings::WINDOWS );
	}
#endif

	row++;
    lblJoystickDemo = new QLabel("js_demo untested");
	grpJoystick->addWidget(lblJoystickDemo, row, 0, 1, 2);


    //============================================================================
    //== Main Settings connection
    connect(this, SIGNAL(setx(QString,bool,QString)), mainObject->X, SLOT(set_option(QString,bool,QString)) );

    registerField("fgcom_exe_path", txtFgCom);
    registerField("jsdemo_exe_path", txtJoystickDemo);

}

bool OtherPage::check_fgcom(QString filePath)
{
    bool result = false;
    if(filePath.length() > 0){
        testExe * pe = new testExe(filePath);
        pe->finds << "a communication radio based on VoIP";
        pe->args << "--help";
        if (pe->runTest()) {
            txtFgCom->setText(filePath);
            lblFgCom->setText("ok");
            lblFgCom->setStyleSheet("color:#009900;");
            result = true;
        } else {
            lblFgCom->setText(pe->stgResult);
            lblFgCom->setStyleSheet("color:#990000;");
        }
        outLog("TEST:fgcom: "+pe->stgResult);
        delete pe;
    }
    return result;
}

void OtherPage::on_select_fgcom_path()
{
    QString def = txtFgCom->text();
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select FGCom binary"),
                                                         def);
    if(filePath.length() > 0){
        if (check_fgcom(filePath))
            on_fgcom();
    }
}

bool OtherPage::check_jsdemo(QString filePath)
{
    bool result = false;
    if(filePath.length() > 0){
        testExe * pe = new testExe(filePath);
        pe->finds << "Joystick test program";
        pe->timeOut = 1;
        if (pe->runTest()) {
            result = true;
            txtJoystickDemo->setText(filePath);
            lblJoystickDemo->setText("ok");
            lblJoystickDemo->setStyleSheet("color:#009900;");
            result = true;
        } else {
            lblJoystickDemo->setText(pe->stgResult);
            lblJoystickDemo->setStyleSheet("color:#990000;");
        }
        outLog("TEST:js_demo: "+pe->stgResult);
        delete pe;
    }
    return result;
}


void OtherPage::on_select_joystick_path()
{
    QString def = txtJoystickDemo->text();
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select JS demo binary"),
                                                         def);
    if(filePath.length() > 0){
        if (check_jsdemo(filePath))
            on_jsdemo();
    }
}

void OtherPage::on_fgcom_autodetect(){

}

void OtherPage::on_joystick_autodetect(){

}

void OtherPage::check_paths()
{
    QString comdef = txtFgCom->text();
    check_fgcom(comdef);
    QString jsdef = txtJoystickDemo->text();
    check_jsdemo(jsdef);
}

void OtherPage::on_fgcom()
{
    bool enab = false;
    QString fgcom = txtFgCom->text();
    fgcom = fgcom.trimmed();
    if (fgcom.length() > 0) {
        enab = true;
    } else {
        enab = false;
    }
    //outLog("Set FGCOM EXE: "+fgcom);
    emit setx("fgcom_exe_path", enab, fgcom);
}

void OtherPage::on_jsdemo()
{
    bool enable = false;
    QString jsdemo_exe = txtJoystickDemo->text().trimmed();
    if (jsdemo_exe.length())
        enable = true;
    emit setx("jsdemo_exe_path", enable, jsdemo_exe);
}

//===================================================
//= InitializePage
void OtherPage::initializePage()
{
    XOpt optFgCom = mainObject->X->get_opt("fgcom_exe_path");
    txtFgCom->setText(optFgCom.value);
    XOpt optJsDemo = mainObject->X->get_opt("jsdemo_exe_path");
    txtJoystickDemo->setText(optJsDemo.value);
    check_paths();
}

//====================================================
//= ValidatePage
bool OtherPage::validatePage()
{
	//check_data_paths();
	//check_exe_paths();

	//if(checkBoxUseTerrasync->isChecked()){
	//	if(QFile::exists(txtTerraSyncPath->text())){
	//		// TODO - check its writable
	//		return true;
	//	}else{
	//		txtTerraSyncPath->setFocus();
	//		return false;
	//	}
	//}
	return true;
}
