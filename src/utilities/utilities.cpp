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
#include "xobjects/xsettings.h"

#if 0
/*
  ZIP FILE FUNCTION
 */
#define MX_GZ_BUFFER 8*1024  // read zlib internal uses 8K buffer, so...
// #define MX_GZ_BUFFER 256    // little buffer - for testing logic only
static char _s_gz_buf[MX_GZ_BUFFER+2]; // NOTE: the +2 for zero termination
static int gz_in_buffer = 0;
static int gz_in_offset = 0;
QString gzreadLine(gzFile file)
{
    QString data;
    QString blk;
    int i, len;
    char c;
    char * cp = _s_gz_buf;
    char * bgn;
    data = "";
    if (!file)
        return data;
    while (!gzeof(file)) {
        if (gz_in_offset == 0) {
            gz_in_buffer = gzread(file, cp, MX_GZ_BUFFER);
        }
        if (gz_in_buffer) {
            c = 0;
            bgn = &cp[gz_in_offset];
            for (i = gz_in_offset; i < gz_in_buffer; i++) {
                c = cp[i];
                if (c == '\n') {
                    break;
                }
            }
            if (c == '\n') {
                int j = i;
                while (j >= gz_in_offset) {
                    if (cp[j] > ' ')
                        break;  // stop at better than space
                    cp[j] = 0;  // trim tail of data
                    j--;        // backup
                }
                blk = bgn;              // get the BEGIN pointer
                data.append(blk);       // add the data
                gz_in_offset = i + 1;   // update next input
                if (gz_in_offset >= gz_in_buffer)
                    gz_in_offset = 0;   // deal with wrap
                break;  // RETURN the accumuated data
            }
            cp[i] = 0;   // NOTE: space has been made for this OVERRUN
            blk = bgn;    // data to QString
            data.append(blk); // accumulate data
            gz_in_offset = 0;   // restart offset
        } else
            break;
    }
    len = data.length(); // just for DEBUG
    bgn = &_s_gz_buf[gz_in_offset];
    i = gz_in_offset;
    return data;
}
#endif

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
	return util_createLogFile(XSettings::log_file_path());
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
