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
    bool result;
    bool runTest();
private:
    void init();
};


#endif // FGX_UTILITIES_H
// eof - utilities.h
