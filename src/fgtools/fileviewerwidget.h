#ifndef FILEVIEWERWIDGET_H
#define FILEVIEWERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>

/*! @brief Show a files contents, raw and pretty.
 *
 * Currently this is just a raw text viewer.
 *
 * Idea is to be able to just read a file and view it..
 * Initiall this is for text and a view of the raw text..
 * Future idea is to have a clever viewer that detect stuff..
 * eg detect xml and parse into a tree as well as raw
 * json parsed into a tree as well as raw
 *
 * @todo watch the file for changes and reload (checkbox) and info on last updated
 * @todo this we need to create a model.. and a parser.. so we wait for qt5..
*/
class FileViewerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileViewerWidget(QWidget *parent = 0);

    QLabel *lblFileName;
    QTextEdit *txtEdit;

    void setFile(QString file_name);

    void moveEvent(QMoveEvent *ev);
signals:

public slots:

};

#endif // FILEVIEWERWIDGET_H

