// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // headerwidget.cpp
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information


#include "app_config.h"
#include <QtDebug>

#include <QVBoxLayout>

#include "launcher/headerwidget.h"
#include "utilities/utilities.h"


HeaderWidget::HeaderWidget(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

    mainObject = mob;


    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->setSpacing(2);
    setLayout(mainLayout);

    //= Fgx Logo is a QLabel with background image
    QLabel *iconLabel = new QLabel("");
    iconLabel->setFixedHeight(40);
    iconLabel->setFixedWidth(40);
    iconLabel->setStyleSheet("QLabel { margin: 0px; background-image: url(:artwork/fgx-logo-flyer);  background-repeat: none;}");
    mainLayout->addWidget(iconLabel, 0, 0, 2, 1);

    //= Main header
    headerLabel = new QLabel("");
    headerLabel->setStyleSheet("color: #333333; font-size: 18pt; font-family: monospace; padding: 2px; border-radius: 8px;");
    mainLayout->addWidget(headerLabel, 0, 1, 1, 1);

    //= Callsing Full words
    callsignLabel = new QLabel("");
    callsignLabel->setStyleSheet("color: #333333; font-size: 8pt; font-family: monospace; margin-left: 10px;");
    mainLayout->addWidget(callsignLabel, 1, 1, 1, 1);


    //= Message Label
    messageLabel = new QLabel();
    messageLabel->setText("FGx booted ;-)");
    mainLayout->addWidget(messageLabel, 0, 2,2,1);

}

//========================================================================
//= Show Message
void HeaderWidget::showMessage(QString message){
    showMessage(message, DEF_TO_MS);
}

void HeaderWidget::showMessage(QString message, int timeout){

    messageLabel->setStyleSheet("QLabel { color: #666666; font-size: 16px; padding-left: 5px; background-color: yellow; }");
    messageLabel->setText(message);

    QTimer::singleShot(timeout, this, SLOT(cancel_message()) );
}

//== Cancel message
void HeaderWidget::cancel_message()
{
    messageLabel->setStyleSheet("");
    messageLabel->setText("");
}


//========================================================================
//= Set Header
void HeaderWidget::setHeader(QString text)
{
    headerLabel->setText(text);

}

//= Set Callsign Sentence
void HeaderWidget::setCallsign(QString text)
{
    text = text.toUpper();
    QStringList words;
    for (int i = 0; i < text.size(); ++i) {
        words << abc_to_telephony(text.at(i).toLatin1());
    }
    callsignLabel->setText(words.join(" ").toUpper());

}
