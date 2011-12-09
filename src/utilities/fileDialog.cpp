/* ********************************************************
   *  fileDialog.cpp
   *
   *  Created by Geoff R. Mclane, Paris
   *  (C) Dec 2011 GPL2 (or later)
   *
   *  A rather crude getFileName capability
   *
   ******************************************************** */
#include "fileDialog.h"
#include "utilities.h"

fileDialog::fileDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Select File");
    setMinimumSize(550,450);
    got_cancel = false;
    filt_count = 0;
    optionFlag = flg_Default;

    int m = 10;
    int row = 0;
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setContentsMargins(m,m,m,m);
    mainLayout->setSpacing(10);
    setLayout(mainLayout);

    QLabel *info1 = new QLabel("Enter file Name: or Path: or navigate via the tree list below.",this);
    mainLayout->addWidget(info1,row,0,1,4);
    row++;

    // ***TBD*** Need to understand how to force the Name: and Path:
    // labels to their smallest. Maybe use the 'friend' idea, or
    // set maximum size, or something...
    // file name row
    nameLab = new QLabel("Name:",this);
    nameLab->setAlignment(Qt::AlignRight);
    nameEd = new QLineEdit(this);
    // add the Name:
    mainLayout->addWidget(nameLab,row,0,1,1);
    mainLayout->addWidget(nameEd,row,1,1,3);
    // add change handler
    connect(nameEd,SIGNAL(textChanged(QString)),this,SLOT(on_name_change(QString)));
    row++;  // bump ROW

    // path row
    pathLab = new QLabel("Path:",this);
    pathLab->setAlignment(Qt::AlignRight);
    pathEd = new QLineEdit(this);
    // add the Path:
    mainLayout->addWidget(pathLab,row,0,1,1);
    mainLayout->addWidget(pathEd,row,1,1,3);
    // add change handler
    connect(pathEd,SIGNAL(textChanged(QString)),this,SLOT(on_path_change(QString)));
    row++;  // bump ROW

    QLabel *info2 = new QLabel("Green if exists. Red if new.",this);
    mainLayout->addWidget(info2,row,0,1,4);
    row++;

    filtLabel1 = new QLabel("Filter:",this);
    filtLabel2 = new QLabel("*",this);
    mainLayout->addWidget(filtLabel1,row,0,1,1);
    mainLayout->addWidget(filtLabel2,row,1,1,3);
    row++;


    //= Directory Tree
    treeWidget = new QTreeWidget(this);
    treeWidget->setAlternatingRowColors(true);
    treeWidget->setRootIsDecorated(true);
    treeWidget->setUniformRowHeights(true);
    treeWidget->setSortingEnabled(false);
    treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    treeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    // ***TBD*** How to make the 'Type' column smaller
    // ***TBD*** Maybe make the 'Type' column hidden, and add a visible ICON
    // Need ICONS - Directory, File (and Drives for windows)
    treeWidget->headerItem()->setText(C_TYPE, "Type");
    treeWidget->headerItem()->setText(C_NAME, "Name");
    connect( treeWidget,
             SIGNAL( itemSelectionChanged() ),
             this,
             SLOT( on_tree_selection_changed() )
    );
    mainLayout->addWidget(treeWidget,row,0,10,4);
    row += 10;

    QPushButton *cancel = new QPushButton(this);
    cancel->setText("Cancel");
    mainLayout->addWidget(cancel,row,0,1,2);
    connect(cancel, SIGNAL(clicked()), this, SLOT(on_cancel()));

    QPushButton *ok = new QPushButton(this);
    ok->setText("Ok");
    ok->setAutoDefault(true);
    mainLayout->addWidget(ok,row,2,1,2);
    connect(ok, SIGNAL(clicked()), this, SLOT(on_ok()));

    row++;

    // NO, no status bar required here
    //QStatusBar *statusBar = new QStatusBar(this);
    //mainLayout->addWidget(statusBar,row,0,1,4);

}

// ***TBD*** Need to separate into directory + name
// here done very manually, but maybe there are Qt services to
// do this path parsing
void fileDialog::init(QString title, QString prev, QStringList filt)
{
    int ind, siz;
    QString tmp = prev;
    QString fil = prev;
    if (title.length())
        setWindowTitle(title);
    if (filt.count()) {
        tmp = "";
        for (ind = 0; ind < filt.count(); ind++) {
            if (tmp.length())
                tmp.append(";");
            tmp.append(filt.at(ind));
            filt_count++;
        }
        filtLabel2->setText(tmp);
    }
    tmp = prev;
    ind = tmp.indexOf(QChar('/'));
    siz = 0;
    // march to last '/' in path
    while (ind >= 0) {
        tmp = tmp.mid(ind + 1);
        siz = tmp.size();
        ind = tmp.indexOf(QChar('/'));
    }
    if (siz > 0) {
        fil = tmp;
        tmp = prev;
        tmp.chop(siz); // This is the DIRECTORY
        tmp = QDir::cleanPath(tmp);
    } else {
        tmp = util_getCurrentWorkDirectory();
    }
    nameEd->setText(fil);
    pathEd->setText(tmp);
    fill_tree();
    on_name_change(fil);
    on_path_change(tmp);
}


// this ONLY applies to WINDOWS, but the service apparently exists in
// all platforms, but is ignored, since there are no logical drives
int fileDialog::getDriveCount()
{
    int i;
    int cnt = 0;
    QFileInfoList drvList = QDir::drives();
    for (i = 0; i < drvList.count(); i++) {
        QFileInfo fi = drvList.at(i);
        QDir d(fi.filePath());
        if (!d.exists() || !d.isReadable())
            continue;   // do NOT add NON existant DRIVES
        cnt++;
    }
    return cnt;
}

void fileDialog::fill_tree()
{
    int i;
    QString tmp(pathEd->text());    // get the PATH
    // fill the tree
#ifdef Q_OS_WIN
    if (tmp == ST_DRIVES) {
        QFileInfoList drvList = QDir::drives();
        // int cnt = getDriveCount();
        treeWidget->setUpdatesEnabled(false);
        treeWidget->model()->removeRows(0, treeWidget->model()->rowCount());
        QTreeWidgetItem *parentItem; // = new QTreeWidgetItem();
        parentItem = treeWidget->invisibleRootItem();
        for (i = 0; i < drvList.count(); i++) {
            QFileInfo fi = drvList.at(i);
            QDir d(fi.filePath());
            if (!d.exists() || !d.isReadable())
                continue;   // do NOT add NON existant DRIVES
            QTreeWidgetItem *fileItem = new QTreeWidgetItem(parentItem);
            fileItem->setText(C_TYPE,"Dr");
            fileItem->setText(C_NAME,fi.filePath());
        }
        treeWidget->setUpdatesEnabled(true);
        return;
    }
#endif
    QString filt(filtLabel2->text());
    QStringList filter(filt.split(QChar(';'),QString::SkipEmptyParts));
    if (tmp == ST_ROOT)
        tmp = "/";
    QStringList fList = findFiles(tmp,filter,false);  // get file list
    QStringList dList = findDirs(tmp,false);        // get directory list

    treeWidget->setUpdatesEnabled(false);
    treeWidget->model()->removeRows(0, treeWidget->model()->rowCount());
    QTreeWidgetItem *parentItem; // = new QTreeWidgetItem();
    parentItem = treeWidget->invisibleRootItem();
    QTreeWidgetItem *fileItem = new QTreeWidgetItem(parentItem);
    fileItem->setText(C_TYPE,"D");
    fileItem->setText(C_NAME,"..");
    int ind;
    QString fil;
    for (i = 0; i < dList.size(); ++i) {
        fil = dList.at(i);
        ind = fil.indexOf(QChar('/'));
        // march to last '/' in path
        while (ind >= 0) {
            fil = fil.mid(ind + 1);
            ind = fil.indexOf(QChar('/'));
        }
        QTreeWidgetItem *fi = new QTreeWidgetItem(parentItem);
        fi->setText(C_TYPE,"D");
        fi->setText(C_NAME,fil);
    }
    for (i = 0; i < fList.size(); ++i) {
        fil = fList.at(i);
        ind = fil.indexOf(QChar('/'));
        // march to last '/' in path
        while (ind >= 0) {
            fil = fil.mid(ind + 1);
            ind = fil.indexOf(QChar('/'));
        }
        QTreeWidgetItem *fi = new QTreeWidgetItem(parentItem);
        fi->setText(C_TYPE,"F");
        fi->setText(C_NAME,fil);
    }
    treeWidget->setUpdatesEnabled(true);
}

void fileDialog::on_tree_selection_changed()
{
    QTreeWidgetItem *item = treeWidget->currentItem();
    if (!item) {
        //outLog("on_tree_selection_changed: no selected item");
        //nameEd->setText("");
        return;
    }
    QString type = item->text(C_TYPE);
    QString name = item->text(C_NAME);
    //= Check there is item and it has a name
    if ((type.length() == 0)||(name.length() == 0)) {
        outLog("on_tree_selection_changed: no C_TYPE ir C_NAME item");
        nameEd->setText("");
        return;
    }
    QString path = pathEd->text();
    int ind, siz;
    QString tmp;
    if (type == "D") {
        if (name == "..") {
            // go up one level if possible
            if (path == ST_ROOT)
                return;
            tmp = path;
            siz = 0;
            ind = tmp.indexOf(QChar('/'));
            // march to last '/' in path
            while (ind >= 0) {
                tmp = tmp.mid(ind + 1);
                siz = tmp.length();
                ind = tmp.indexOf(QChar('/'));
            }
            if (siz) {
                path.chop(siz+1);   // remove last entry, and separator
                if (path.length() == 0)
                    path = ST_ROOT;
                pathEd->setText(path);
            }
#ifdef Q_OS_WIN
            else {
                // up from 'C:' is a list of active DRIVES
                //QFileInfoList dList = QDir::drives();
                int cnt = getDriveCount();
                if (cnt > 1) {
                    path = ST_DRIVES;
                    pathEd->setText(path);
                }
                // ***TBD*** Perhaps some WARNING that we are on the ONLY
                // valid readable DRIVE, and maybe should NOT add a '..'
                // token in this case
            }
#endif
        } else {
            // go down into this directory
            if (path == ST_ROOT)
                path= "";
            path.append("/"+name);
            pathEd->setText(path);
        }
        fill_tree(); // and reset the LIST
        on_path_change(path);
#ifdef Q_OS_WIN
    } else if (type == "Dr") {
        // have selected a DRIVE
        if ((name.length() > 2)&&(name.at(2) == QChar('/'))) {
            name.chop(1);   // drop the trailing
        }
        pathEd->setText(name);  // set this DRIVE
        fill_tree();
        on_path_change(name);
#endif
    } else if (type == "F") {
        // file selected
        nameEd->setText(name);
        on_name_change(name);
    }
}

void fileDialog::on_name_change(QString in)
{
    QString txt(in);
    QString dir(pathEd->text());
    // ***TBD*** Could check if this is one of the special tokens,
    // ST_ROOT or ST_DRIVES, but it is unlikely would even match
    // to an existing file...
    QFile file(dir+"/"+txt);
    QString style("color:red");
    if (file.exists()) {
        style = "color:green";
    }
    nameLab->setStyleSheet(style);
}

void fileDialog::on_path_change(QString in)
{
    // FORCE the separator ALWAYS to '/', since manual user input allowed
    QString txt(in);
    if (util_ensureUnixPathSep(txt))
        pathEd->setText(txt);   // put back correct string
    QDir dir(txt);
    QString style("color:red");
    if (dir.exists() || (txt == ST_ROOT)
#ifdef Q_OS_WIN
        || (txt == ST_DRIVES)
#endif
        ) {
        style = "color:green";
        on_name_change(nameEd->text());
        fill_tree();
    }
    pathLab->setStyleSheet(style);
}

void fileDialog::on_ok()
{
    close();
}

void fileDialog::on_cancel()
{
    nameEd->setText("");
    got_cancel = true;
    close();
}

// pass back the RESULTS
QString fileDialog::getFileName()
{
    if (got_cancel)
        return "";  // user cancelled, to info NOT valid
    QString name = nameEd->text();
    QString path = pathEd->text();
    if (name.length() == 0)
        return name;
    // ***TBD*** Maybe this should be an OPTION only
    QString filt = filtLabel2->text();
    if ((optionFlag & flg_AddExtent) &&     // is the flag ON
        (filt_count == 1) && (filt.length() > 2) && // check for valid 'filter' item
        (filt.at(0) == QChar('*')) && (filt.at(1) == QChar('.'))) {
        // user gave ONE filter, and it conforms to the norm... *.extension
        filt = filt.right(filt.length()-1);    // get the '.extension'
        Qt::CaseSensitivity cs = Qt::CaseSensitive;
#ifdef Q_OS_WIN
        cs = Qt::CaseInsensitive;
#endif
        int ind = name.indexOf(filt,0,cs);     // see if it has this extension
        if (ind != (name.length() - filt.length()))
            name.append(filt);  // append the SINGLE FILTER
    }
    if (path == ST_ROOT)
        return "/"+name;
    return path+"/"+name;
}


// eof - fileDialog.cpp
