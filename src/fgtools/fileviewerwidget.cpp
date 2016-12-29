
#include <QtDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>


#include <QFileInfo>
#include <QFile>

#include "fileviewerwidget.h"

FileViewerWidget::FileViewerWidget(QWidget *parent) :
    QWidget(parent)
{

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);

    //= Top layout has label adn close
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->setContentsMargins(0,0,0,0);
    topLayout->setSpacing(0);
    mainLayout->addLayout(topLayout);

    lblFileName = new QLabel();
    lblFileName->setStyleSheet("background-color: black; color: #dddddd; font-family: monospace; font-size: 14pt;");
    topLayout->addWidget(lblFileName);

    QToolButton *buttClose = new QToolButton();
    buttClose->setText(tr("Close"));
    buttClose->setIcon(QIcon(":/icon/close"));
    buttClose->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    topLayout->addWidget(buttClose);
    connect(buttClose, SIGNAL(clicked()),
            this, SLOT(close())
    );

    txtEdit = new QPlainTextEdit();
    txtEdit->setWordWrapMode(QTextOption::NoWrap);
    mainLayout->addWidget(txtEdit);


}

/*! @brief Loads file and sets labels
 *
 * @todo no error trapping, autodetect required
 */
void FileViewerWidget::setFile(QString file_path){

    QFileInfo finfo(file_path);

    this->setWindowTitle(file_path);

    this->lblFileName->setText(finfo.fileName());

    QFile file( file_path );
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            //qDebug() << "TODO Open error cachce file=";
            return;
    }
    QString contents = QString(file.readAll()).toUtf8();
    //qDebug() << contents;
    this->txtEdit->setPlainText(contents);

}

void FileViewerWidget::moveEvent(QMoveEvent *ev){
    Q_UNUSED(ev);
    //TODO save window size
    //this->mainObject->settings->saveWindow(this);
}
