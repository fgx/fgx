/* ********************************************************
   *  utilities.h
   *  for fgx
   *
   *  Created by Geoff R. Mclane, Paris
   *  © 2011 GPL2 (or later)
   *
   ******************************************************** */
#ifndef FGX_UTILITIES_H
#define FGX_UTILITIES_H

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QDateTime>
#include <QFileDialog>

// #include "zlib/fgx_zlib.h"
/* ZIP FILE */
// extern QString gzreadLine(gzFile file);

/* LOG FILE FUNCTIONS */

// out a QString to the current log
// flag - bit 1 = add line term (default)
//        bit 2 = prepend time
#define olflg_AddLE 0x00000001
#define olflg_AddTm 0x00000002
extern void outLog(QString s, int flag = olflg_AddLE);
extern void util_setLogFile(QString file);
extern bool util_createLogFile(QString file);
extern bool util_setStdLogFile();

/* GET FILE LIST per filter */
extern QStringList findFiles(const QString &startDir, QStringList filters, bool recurse = false);
/* GET DIRECTORY list */
extern QStringList findDirs(QString startDir, bool recurse = false);

// given millisecond, return appropriate (nice) string, with units
extern QString getElapTimeStg(int ms);
extern QString util_getDateTimestg(void); // "yyyy/MM/dd HH:mm:ss"
extern QString util_getTimestg(void);     // "HH:mm:ss"



//===================================================================
//= ICAO Radio
extern char * abc_to_morse(char ch);
extern char * abc_to_telephony(char ch);
extern char * abc_to_phonetic(char ch);
extern char telephony_to_abc(char * pch);
extern char *telephony_to_morse(char * pch);
extern char *telephony_to_phonetic(char * pch);

// simple class to 'test' a runtime
class testExe
{
public:
    testExe();
    testExe(QString ex);
    ~testExe();
    void setExe(QString ex) { exe = ex; }
    void setArgs(QStringList ag) { args = ag; }
    void setFinds(QStringList fnd) { finds = fnd; }
    void setTimeout(int secs) { timeOut = secs; }
    QString exe;
    QStringList args;
    QStringList finds;
    QStringList extraEnv;
    QString rtDir;
    QString outStd;
    QString outErr;
    QString stgResult;
    int errExit;
    int timeOut;
    int runTime;
    int flagOpts;
    int result;
    bool runTest();
private:
    void init();
};

// native versions
extern QString util_browseDirectory(QWidget * parent = 0,
                                    QString prompt = QString(),
                                    QString current = QString());
extern QString util_browseFile(QWidget * parent = 0,
                               QString prompt = QString(),
                               QString current = QString(),
                               QStringList filter = QStringList(),
                               int options = 0);
extern QString util_browseNewFile(QWidget * parent = 0,
                               QString prompt = QString(),
                               QString current = QString(),
                               QStringList filter = QStringList(),
                               int options = 0);

// simple dialog version
extern QString util_getFileName(QWidget *parent = 0, QString title = QString(), QString previous = QString(), QStringList filt = QStringList());
extern QString util_getDirName(QWidget *parent = 0, QString title = QString(), QString previous = QString());
extern QString util_getCurrentWorkDirectory();

extern bool util_ensureUnixPathSep(QString &txt); // return 'true' if changed
extern QString util_getBasePath(QString in);    // return path up to and including last '/'
extern QString util_getBaseName(QString in);    // return name after last '/'

// a distance estimate between two lat, lon positions, in km... using
#define EARTH_RAD 6370  // actually from 6,353 to 6,384 km
#define DEG2RAD 0.017453229251994329509  // est PI / 180
// FIX20110823 - dist_est_km() returned 'nan' if virtually the same point
// double dist_est_km(double lat1, double lon1, double lat2, double lon2)
extern double util_DistEst_km(double lat1, double lon1, double lat2, double lon2);

#endif // FGX_UTILITIES_H
// eof - utilities.h
