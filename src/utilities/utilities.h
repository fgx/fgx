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


#endif // FGX_UTILITIES_H
// eof - utilities.h
