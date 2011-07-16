/* ********************************************************
   *  utilities.cpp
   *  for fgx
   *
   *  Created by Geoff R. Mclane, Paris
   *  © 2011 GPL2 (or later)
   *
   ******************************************************** */

#include <QMutex>
#include "utilities.h"
#include "xobjects/mainobject.h"


/*
    LOG FILE FUNCTIONS
 */

static QString m_logFile;
static QMutex m_logMutex;

// set log file name
void util_setLogFile(QString file)
{
    m_logFile = file; // set name
}

// create a log file and set name
bool util_createLogFile(QString file)
{
    bool bret = false;
    QFile data(file);
    if (data.open(QIODevice::ReadWrite | QIODevice::Truncate | QFile::Text)) {
        QTextStream out(&data);
        QString dt = util_getDateTimestg();
        out << dt << " - ";
        out.flush();
        data.close();
        bret = true;    // log is ok
        util_setLogFile(file); // set name
    }
    if (bret)
        outLog("Commenced log");
    return bret;
}

bool util_setStdLogFile()
{
	/*
    QString dir;
    QString file;
    QString log;

#if defined(Q_OS_WIN)
	dir  = "C:/Windows/Temp";
	file = "fgx-log.txt";
#elif defined(Q_OS_MAC)
   dir.append(QDir::homePath());
   dir.append("/Library/Logs");
   file = "fgx.log";
#elif defined(Q_OS_UNIX)
   dir  = "/tmp";
   file = "fgx.log";
#else
#error Unknown OS - Add default log dir and file for this OS!
#endif
   log = dir + "/" + file;
   */
	return util_createLogFile(MainObject::log_file_path());
}

// append to LOG file, after acquiring the mutext
void outLog(QString s, int flag)
{
    if (m_logFile.size()) {
        QFile data(m_logFile);
        m_logMutex.lock();  // grap the mutex
        if (data.open(QFile::WriteOnly | QFile::Append | QFile::Text)) {
            QTextStream out(&data);
            if (flag & olflg_AddTm)
                out << util_getTimestg()+" - ";
            out << s;
            if (flag & olflg_AddLE)
                out << "\n";
            data.close(); // ensure it get to disk
        }
        m_logMutex.unlock(); // release the mutex
    }
}

/*
    Utility function that recursively searches, if desired, for files per filters.
*/
QStringList findFiles(const QString &startDir, QStringList filters, bool recurse)
{
    QStringList names;
    QDir dir(startDir);

    // 1: Get the files matching the filter
    foreach (QString file, dir.entryList(filters, QDir::Files))
        names += startDir + "/" + file;

    if (recurse) {
        // 2: If recursive, get ALL directories, and try for more files
        foreach (QString subdir, dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot))
            names += findFiles(startDir + "/" + subdir, filters, recurse);
    }
    return names;
}

// given millisecond, return appropriate (nice) string, with units
QString getElapTimeStg(int ms)
{
    QString fmt = "";
    if (ms < 1000) {
        fmt.sprintf("%d ms", ms);
    } else {
        int secs = ms / 1000;
        ms = ms % 1000;
        if (secs < 60) {
            if (ms)
                fmt.sprintf("%d.%03d secs", secs, ms);
            else
                fmt.sprintf("%d secs", secs);
        } else {
            int mins = secs / 60;
            secs = secs % 60;
            if (mins < 60) {
                if (ms)
                    fmt.sprintf("%d:%02d.%03d mins", mins, secs, ms);
                else if (secs)
                    fmt.sprintf("%d:%02d mins", mins, secs);
                else
                    fmt.sprintf("%d mins", mins);
            } else {
                int hrs = mins / 60;
                mins = mins % 60;
                if (ms)
                    fmt.sprintf("%d:%02d:%02d.%03d hrs", hrs, mins, secs, ms);
                else if (secs)
                    fmt.sprintf("%d:%02d:%02d hrs", hrs, mins, secs);
                else if (mins)
                    fmt.sprintf("%d:%02d hrs", hrs, mins);
                else
                    fmt.sprintf("%d hrs", hrs);
            }
        }
    }
    return fmt;
}

QString util_getDateTimestg(void)
{
    QDateTime datetime = QDateTime::currentDateTime();
    QString dt = datetime.toString("yyyy/MM/dd HH:mm:ss");
    return dt;
}
QString util_getTimestg(void)
{
    QDateTime datetime = QDateTime::currentDateTime();
    QString dt = datetime.toString("HH:mm:ss");
    return dt;
}

// eof - utilities.cpp
