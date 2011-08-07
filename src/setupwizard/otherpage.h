#ifndef OTHERPAGE_H
#define OTHERPAGE_H

#include <QWizardPage>
#include <QCheckBox>
#include <QLineEdit>
#include <QToolButton>
#include <QLabel>


#include "xobjects/mainobject.h"

class OtherPage : public QWizardPage
{
	Q_OBJECT
	public:
		explicit OtherPage(MainObject *mob, QWidget *parent = 0);
		
		MainObject *mainObject;
		QCheckBox *checkBoxUseFGCom;
		QCheckBox *checkBoxUseJSDemo;
		QToolButton *buttSelectFGComExePath;
		QToolButton *buttSelectJSDemoExePath;
		QLabel *FGComPathLabel;
		QLabel *JSDemoPathLabel;
		QLabel *FGComExePathLabel;
		QLabel *JSDemoExePathLabel;
		QLineEdit *txtFGComExePath;
		QLineEdit *txtJSDemoExePath;
		QLabel *lblHelpFGComExe;
		QLabel *lblHelpJSDemoExe;
		
		
	signals:
		
		public slots:
		void on_checkbox_fgcom_clicked();
		void on_select_fgcom_exe_path();
		void check_fgcom_exe_paths();
		void on_checkbox_jsdemo_clicked();
		void on_select_jsdemo_exe_path();
		void check_jsdemo_exe_paths();
		
		void initializePage();
		bool validatePage();
};
	
#endif // OTHERPAGE_H